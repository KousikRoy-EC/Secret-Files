# nDPI Netfilter Kernel Integration Deep Dive

## 1. Architectural Overview and Kernel/Userspace Interaction

The project bridges the gap between the userspace `iptables` utility and the Linux kernel's networking stack by porting the nDPI (Deep Packet Inspection) library into the kernel. This allows Layer 7 protocol classification with **zero userspace round-trips** per packet.

### Userspace: The iptables Extension (`libxt_ndpi.so`)
The userspace component is an `xtables` shared library that provides two components:
*   **Match Extension (`-m ndpi`)**: Allows iptables to match packets based on protocol, hostname, or SSL certificate. 
    *   *Example*: `iptables -m ndpi --proto http -j DROP`
*   **Target Extension (`-j NDPI`)**: Allows iptables to perform actions on classified packets, such as applying `mark` (for policy routing) or `priority` (for QoS traffic shaping).
    *   *Example*: `iptables -j NDPI --ndpi-id --set-clsf`

When you add a rule via `iptables`, this library serializes the configuration into C structures (`struct xt_ndpi_mtinfo` and `struct xt_ndpi_tginfo`) and sends them to the kernel module via `setsockopt(SO_SET_REPLACE)`.

### Kernelspace: The Netfilter Module (`xt_ndpi.ko`)
The kernel module registers itself as an `xt_match` and `xt_target` within the netfilter framework. Instead of registering raw `nf_hook_ops` for the core interception, it relies on iptables traversing its rule chains. When a packet hits a rule utilizing `-m ndpi` or `-j NDPI`, the kernel netfilter framework invokes the module's match function (`ndpi_mt`) or target function (`ndpi_tg`).

Additionally, the module exposes a rich `procfs` interface (`/proc/net/xt_ndpi/`) for real-time configuration and flow dumping.

---

## 2. The Packet Processing Pipeline

When a packet arrives and hits the `ndpi_mt` match function, it goes through a highly optimized pipeline:

### Step 1: Early Validation & Fast-Path Caching
1.  **Sanity Checks**: Validates IP version, rejects fragmented packets, and ensures the Layer 4 header (TCP/UDP) is complete (`can_handle()`).
2.  **SKB Control Block Cache**: It checks the packet's Socket Buffer Control Block (`skb->cb`). The module embeds a `struct ndpi_cb` here. If a packet has already been processed by another nDPI rule in the same traversal, it instantly returns the cached classification without re-evaluating.

### Step 2: Connection Tracking (Statefulness)
nDPI needs to see multiple packets in a flow to classify it. The module makes the inspection **stateful** by piggybacking on the kernel's `nf_conntrack` (connection tracking) subsystem.
1.  It retrieves the conntrack entry for the packet (`nf_ct_get`).
2.  It uses either a **custom conntrack extension** (if the kernel is patched) or **conntrack labels** to allocate and attach a `struct nf_ct_ext_ndpi` state block to the connection.
3.  This state block holds a pointer to a `struct ndpi_flow_struct` (the core nDPI state for that specific connection).

### Step 3: Deep Packet Inspection
If the flow hasn't been classified yet:
1.  The module allocates/retrieves per-IP tracking nodes (`ndpi_id_struct`) using a Red-Black tree.
2.  It invokes the core nDPI library wrapper (`ndpi_process_packet()`).
3.  The library analyzes the payload. It uses a **patricia tree** for IP/port heuristic fallbacks, and an **Aho-Corasick automaton** for fast string matching against hostnames and TLS SNI fields.
4.  **Classification Window**: nDPI gives up if it cannot classify a flow within a certain number of packets (default is 20 for TCP/UDP). If max packets are reached, or the protocol is successfully identified, the module sets a `detect_done` flag.
5.  **Memory Optimization**: Once `detect_done` is set, the heavy `ndpi_flow_struct` is freed back to its slab cache. Subsequent packets in the flow are classified strictly via the cached conntrack state (`ct_ndpi->proto`).

### Step 4: Matching and Action
The classified protocol (e.g., `NDPI_PROTOCOL_BITTORRENT`) is compared against the bitmask provided by the iptables rule. If it matches, the packet is processed according to the iptables chain policy (e.g., DROPPED, ACCEPTED, MARKED).

---

## 3. Flow Tracking and Netfilter Hooks

While classification is triggered by iptables rules, the module optionally registers true netfilter hooks (`nf_hook_ops`) for **NAT detection**. 
If flow accounting is enabled (`ndpi_enable_flow=1`), the module registers hooks at `NF_INET_PRE_ROUTING`, `NF_INET_POST_ROUTING`, and `NF_INET_LOCAL_IN`. These run just after the kernel's NAT processing to compare the original conntrack tuple against the reply tuple, allowing the module to accurately log `SNAT` and `DNAT` IP addresses and ports in its flow dump.

---

## 4. OpenWrt BSP Packaging

To integrate this into a Qualcomm SDK OpenWrt device, the system uses the OpenWrt Buildroot:
*   The userspace library is packaged via an OpenWrt `Makefile` definition. It defines dependencies (`libpcap`, `libjson-c`) and cross-compiles the `libxt_ndpi.so` shared library.
*   The kernel module is typically integrated as a kernel package (`kmod-ndpi-netfilter`), which requires the target's specific Linux kernel headers and toolchain.
*   **Kernel Patches**: The repository includes specific patches for various kernel versions (v3.4 - v4.15). On older kernels, these patches modify `nf_conntrack_extend.c/h` to add `CONFIG_NF_CONNTRACK_CUSTOM`, allowing nDPI to carve out dedicated memory inside the kernel's conntrack structures.

---

## 5. Technical Trade-offs and Constraints

When defending this architectural design in an interview, be prepared to discuss these critical trade-offs:

### 1. In-Kernel Execution vs. Userspace (NFQUEUE)
*   **Advantage**: Extreme performance. By doing DPI in the kernel, you avoid context switches, memory copies (`skb` to userspace buffers), and IPC overhead associated with passing packets to a userspace daemon (like Suricata/Snort via `NFQUEUE`). This results in **zero userspace round-trips**.
*   **Trade-off (Constraint)**: The Linux kernel environment is highly restrictive.
    *   No floating-point operations are allowed.
    *   Memory allocation in the packet path must be non-blocking (`GFP_ATOMIC`), which can fail under heavy memory pressure. The module heavily relies on pre-allocated Slab Caches (`kmem_cache`) for flow structures to mitigate this.
    *   Any bug, buffer overflow, or unhandled exception in the nDPI library parsing logic will result in a **Kernel Panic**, crashing the entire router. 

### 2. Memory Footprint
*   **Advantage**: Stateful classification is highly accurate.
*   **Trade-off (Constraint)**: Tracking state for every active connection requires significant RAM. OpenWrt devices (like Qualcomm SoCs) often have limited memory (e.g., 128MB or 256MB). The `ndpi_flow_struct` is memory-heavy.
*   **Mitigation**: The module mitigates this by aggressively freeing the DPI flow state the moment classification succeeds or fails (after ~20 packets), retaining only a lightweight protocol ID in the conntrack extension.

### 3. CPU Overhead vs. Early Drop
*   **Advantage**: Layer 7 filtering allows granular control (e.g., blocking BitTorrent or specific SNIs).
*   **Trade-off (Constraint)**: DPI string matching (Aho-Corasick) and heuristic analysis are CPU intensive. 
*   **Mitigation**: The `skb->cb` fast-path caching ensures that once a packet is identified, it bypasses the DPI engine instantly. 

### 4. Encrypted Traffic (TLS/HTTPS)
*   **Constraint**: nDPI cannot decrypt HTTPS payload. 
*   **Mitigation**: It relies heavily on analyzing the initial TLS handshake (specifically the Client Hello Server Name Indication - SNI - and the Server Certificate). Once the handshake is complete and the cipher changes, nDPI cannot inspect further data. The classification must happen in those first few packets.

### 5. Asymmetric Routing
*   **Constraint**: If the router only sees one half of the traffic (e.g., asymmetric routing), nDPI classification accuracy drops significantly, as it expects to monitor the stateful bi-directional handshake (like TCP SYN, SYN-ACK).








## Q1: Why Not Use Raw `nf_hook_ops` for Inspection? Why iptables?

### The Two Approaches

There are two ways to insert code into the netfilter packet path:

| Approach                 | API                                                | How it works                                                                                                                                |
|--------------------------|----------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------|
| **Raw hook**             | `nf_register_net_hook()` with `struct nf_hook_ops` | Your callback runs on **every single packet** that hits the hookpoint. No way for the admin to selectively enable/disable.                  |
| **xtables match/target** | `xt_register_match()` / `xt_register_target()`     | Your callback runs **only when an iptables rule references it**. The admin writes rules like `-m ndpi --proto tls -j NDPI --set-mark 0x10`. |

### Why xtables Was Chosen (5 Reasons)

**1. Selective Activation (Performance)**

With a raw hook, **every packet on the box** hits your callback — even traffic you don't care about (management SSH, DNS to the router itself, DHCP, etc.). On a Qualcomm IPQ807x handling 500k+ pps, this is devastating.

With xtables, the admin writes:
```bash
# Only inspect forwarded traffic, skip router-local traffic
iptables -A FORWARD -m ndpi --proto unknown -j NDPI --set-mark ...
```
Router management traffic in INPUT/OUTPUT chains never touches nDPI at all.

**2. Built-in User-Space Configuration Interface**

A raw hook gives you no admin interface. You'd need to build your own:
- A custom netlink socket, or
- A procfs/sysfs write handler, or  
- A char device with custom ioctls

With xtables, you get **iptables CLI for free**. The user-space library `libxt_ndpi.so` (in the `ipt/` directory of this project) handles argument parsing and communicates with the kernel via the existing `setsockopt(SO_SET_REPLACE)` mechanism (more on this below).

**3. Composability with Other Rules**

iptables rules are evaluated **sequentially** in a chain. This lets the admin compose nDPI with other modules:
```bash
# First: rate-limit before DPI (save CPU)
iptables -A FORWARD -m hashlimit --hashlimit-above 10000/sec -j DROP

# Then: classify with nDPI
iptables -A FORWARD -m ndpi --proto bittorrent -j DROP

# Then: QoS mark
iptables -A FORWARD -m ndpi --proto voip -j DSCP --set-dscp 46
```

With a raw hook, you'd have to implement all this priority/ordering logic yourself.

**4. Per-Rule Protocol Selection**

Each iptables rule carries its own `struct xt_ndpi_mtinfo` containing a `NDPI_PROTOCOL_BITMASK`. One rule can match TLS, another can match BitTorrent, a third can match HTTP. With a raw hook, you'd need your own config structure to specify what to match.

**5. Familiar Admin Workflow on OpenWrt**

OpenWrt admins know iptables. They configure it via UCI/LuCI. A raw hook module would require a custom LuCI page, custom UCI schema, and custom init scripts — huge BSP integration cost.

### When Raw Hooks ARE Used (NAT Detection)

The module **does** use raw `nf_hook_ops` for one specific purpose: NAT address detection (explained in Q8 below). This is because NAT detection must run at a precise hookpoint **after** the kernel's NAT engine has rewritten addresses, and it must run on every packet — not just those matched by an iptables rule.

---

## Q2: How Does a Packet Travel Through iptables?

### The Full Path (Step by Step)

```
                        NETWORK INTERFACE (NIC)
                               │
                               ▼
                    ┌─────────────────────┐
                    │    NF_INET_PRE_ROUTING    │  ← Hook point 0
                    │                     │
                    │  raw table          │  ← CT --notrack decisions
                    │  conntrack (defrag) │  ← nf_conntrack_in()
                    │  mangle table       │
                    │  nat table (DNAT)   │
                    └────────┬────────────┘
                             │
                     ┌───────┴───────┐
                     │  Routing      │
                     │  Decision     │
                     └───┬───────┬───┘
                         │       │
              ┌──────────┘       └──────────┐
              ▼                              ▼
    ┌──────────────────┐          ┌──────────────────┐
    │  NF_INET_LOCAL_IN │         │  NF_INET_FORWARD  │  ← Hook point 2
    │                  │          │                    │
    │  mangle table    │          │  mangle table      │
    │  filter table ◄──┼──────── │  filter table  ◄───┼─── YOUR "-m ndpi" RULE
    │  nat table       │          │                    │        IS TYPICALLY HERE
    └────────┬─────────┘          └────────┬───────────┘
             │                              │
             ▼                              ▼
        Local Process              ┌──────────────────┐
        (SSH, DNS, etc.)           │ NF_INET_POST_ROUTING│  ← Hook point 4
                                   │                      │
                                   │  mangle table        │
                                   │  nat table (SNAT)    │
                                   │  conntrack (confirm)  │
                                   └──────────┬───────────┘
                                              │
                                              ▼
                                        NETWORK INTERFACE (OUT)
```

### Inside a Table: How Rules Are Evaluated

When a packet enters a table (e.g., filter/FORWARD), the kernel function `ipt_do_table()` executes:

```c
// Simplified pseudocode of ipt_do_table():
for (rule = chain->first_rule; rule != chain->end; rule = next_rule) {
    
    // Step 1: Check standard matches (source IP, dest IP, interface, protocol)
    if (!ip_packet_match(ip_hdr, rule->ip))
        continue;  // skip this rule
    
    // Step 2: Check extension matches (e.g., -m ndpi, -m conntrack, -m limit)
    for (each extension_match in rule->matches) {
        bool matched = extension_match->match(skb, par);
        //                                    ▲
        //                      THIS calls ndpi_mt() for "-m ndpi"
        if (!matched) goto next_rule;
    }
    
    // Step 3: All matches passed — execute the target
    verdict = rule->target->target(skb, par);
    //                              ▲
    //                THIS calls ndpi_tg() for "-j NDPI"
    
    // Step 4: Act on verdict
    if (verdict == NF_ACCEPT) return NF_ACCEPT;
    if (verdict == NF_DROP)   return NF_DROP;
    if (verdict == XT_CONTINUE) continue;  // try next rule
}
```

> [!IMPORTANT]
> **Key insight for interviews**: `ndpi_mt()` is called as a **match function** — it returns `true/false`. It doesn't decide accept/drop. The **target** (`-j DROP`, `-j ACCEPT`, `-j NDPI`) decides the packet's fate. This separation of "identification" from "action" is the core xtables design philosophy.

### Packet Traversal in Our Module's Context

A typical OpenWrt DPI rule set:

```bash
# Rule 1: Classify all forwarded traffic
iptables -t mangle -A FORWARD -m ndpi --proto all -j NDPI --ndpi-id --set-mark

# Rule 2: Drop BitTorrent
iptables -A FORWARD -m ndpi --proto bittorrent -j DROP

# Rule 3: QoS for VoIP
iptables -t mangle -A FORWARD -m ndpi --proto skype,zoom -j DSCP --set-dscp 46
```

For a single HTTPS packet, the call sequence is:
```
1. ipt_do_table() → evaluates Rule 1
   └── ndpi_mt(skb, par)  → SLOW PATH: calls ndpi_process_packet()
       └── returns true (--proto all always matches if not error)
   └── ndpi_tg(skb, par)  → writes skb->mark from protocol ID
   
2. ipt_do_table() → evaluates Rule 2
   └── ndpi_mt(skb, par)  → FAST PATH: skb->cb[] cache hit!
       └── returns false (proto is TLS, not bittorrent)
   └── (target not invoked, skip)

3. ipt_do_table() → evaluates Rule 3
   └── ndpi_mt(skb, par)  → FAST PATH: skb->cb[] cache hit!
       └── returns false (proto is TLS, not skype/zoom)
   └── (target not invoked, skip)
```

Notice: **Rule 2 and 3 are nearly free** because the `ndpi_cb` cache in `skb->cb[]` avoids re-running the DPI engine. This is why the `magic` sentinel and `last_ct` pointer exist.

---

## Q3: What Is `setsockopt(SO_SET_REPLACE)` and Why Is It Used?

### The Problem It Solves

iptables rules live **in kernel memory** as binary blobs (`struct ipt_replace`). When an admin types:
```bash
iptables -A FORWARD -m ndpi --proto tls -j DROP
```

The user-space `iptables` binary needs to:
1. Read the current ruleset from the kernel
2. Append the new rule to the in-memory copy
3. Send the entire modified ruleset back to the kernel

### How It Works

```
┌──────────────────────────────────────┐
│         USER SPACE                    │
│                                       │
│  iptables binary                      │
│  ├── libxtables (core)                │
│  ├── libxt_ndpi.so (our extension)    │
│  │   └── parses "--proto tls"         │
│  │   └── fills struct xt_ndpi_mtinfo  │
│  │       with protocol bitmask        │
│  │                                    │
│  └── setsockopt(raw_fd,              │
│          IPPROTO_IP,                  │
│          IPT_SO_SET_REPLACE,   ◄──────┼── THIS syscall
│          &ipt_replace_blob,           │
│          sizeof(blob));               │
│                                       │
└──────────────────┬───────────────────┘
                   │  syscall boundary
                   ▼
┌──────────────────────────────────────┐
│         KERNEL SPACE                  │
│                                       │
│  do_ipt_set_ctl()                     │
│  └── IPT_SO_SET_REPLACE handler       │
│      ├── copy_from_user(blob)         │
│      ├── translate_table()            │
│      │   ├── Validates all rules      │
│      │   ├── For each match:          │
│      │   │   └── match->checkentry()  │
│      │   │       └── ndpi_mt_check()  │ ← YOUR module's checkentry
│      │   │           └── validates    │
│      │   │               bitmask,     │
│      │   │               compiles     │
│      │   │               regex if     │
│      │   │               --hostname   │
│      │   │               is used      │
│      │   └── For each target:         │
│      │       └── target->checkentry() │
│      │           └── ndpi_tg_check()  │
│      ├── xt_replace_table()           │
│      │   └── Atomically swaps old     │
│      │       table with new one       │
│      │       (RCU protected)          │
│      └── Old table freed after        │
│          RCU grace period             │
└──────────────────────────────────────┘
```

### In Simple Terms

`setsockopt(IPT_SO_SET_REPLACE)` is the **system call** through which the user-space `iptables` command pushes the entire rule table (including our `xt_ndpi_mtinfo` struct with the protocol bitmask) into the kernel. The kernel validates it, calls our `ndpi_mt_check()`, and atomically replaces the live rule table.

> [!NOTE]
> Modern kernels also support `nftables` which uses **netlink sockets** (`AF_NETLINK`) instead of `setsockopt`. But OpenWrt with Qualcomm SDK typically still uses iptables.

---

## Q4: How Does `nf_conntrack` Track Packets? How Does `nf_ct_get()` Work Per-Packet?

### Conntrack Entry Creation (First Packet of a Flow)

```
Packet arrives → NF_INET_PRE_ROUTING hook
                       │
                       ▼
              nf_conntrack_in()
              │
              ├── Extract 5-tuple: {src_ip, dst_ip, src_port, dst_port, protocol}
              │
              ├── Hash the 5-tuple → lookup in global conntrack hash table
              │   └── nf_conntrack_find_get()
              │
              ├── NOT FOUND (first packet of new flow):
              │   ├── Allocate new struct nf_conn (from slab cache)
              │   ├── Initialize:
              │   │   ├── tuplehash[IP_CT_DIR_ORIGINAL] = {src→dst}
              │   │   ├── tuplehash[IP_CT_DIR_REPLY]    = {dst→src} (reversed)
              │   │   ├── status = IPS_NEW
              │   │   ├── timeout = protocol-specific (TCP: 120s, UDP: 30s)
              │   │   └── Extensions area (empty initially)
              │   │
              │   ├── Store pointer in skb:
              │   │   skb->_nfct = (unsigned long)ct | ctinfo;
              │   │   //  ▲ pointer + direction bits packed into one word
              │   │
              │   └── ct is UNCONFIRMED at this point
              │
              └── FOUND (subsequent packet of existing flow):
                  ├── Increment refcount
                  ├── Determine direction: ORIGINAL or REPLY
                  ├── ctinfo = IP_CT_ESTABLISHED + direction
                  └── Store in skb->_nfct (same packing)
```

### How `nf_ct_get()` Works — It's Almost Free

```c
// This is what nf_ct_get() actually does (simplified):
static inline struct nf_conn *nf_ct_get(const struct sk_buff *skb,
                                        enum ip_conntrack_info *ctinfo)
{
    unsigned long nfct = skb->_nfct;          // Just read a word from skb
    *ctinfo = nfct & 7;                        // Low 3 bits = direction/state
    return (struct nf_conn *)(nfct & ~7UL);    // Mask off low bits = pointer
}
```

**This is a pointer dereference, not a hash lookup!** Conntrack already resolved the flow during `nf_conntrack_in()` in the PREROUTING hook. By the time our `ndpi_mt()` runs in the FORWARD chain, the `struct nf_conn *` is already stashed in `skb->_nfct`.

**Cost: ~2 CPU instructions** (load + mask). Zero hash computation.

### The `ctinfo` Values

| Value                     | Meaning                                      | When                                |
|---------------------------|----------------------------------------------|-------------------------------------|
| `IP_CT_NEW`               | First packet of a new flow                   | SYN                                 |
| `IP_CT_ESTABLISHED`       | Packet in original direction, flow confirmed | After SYN-ACK                       |
| `IP_CT_ESTABLISHED_REPLY` | Packet in reply direction                    | Server → Client                     |
| `IP_CT_RELATED`           | Related flow (e.g., FTP data)                | New connection triggered by another |
| `IP_CT_UNTRACKED`         | Explicitly not tracked                       | `-j CT --notrack`                   |

### Why This Matters for nDPI

```c
// In ndpi_mt():
ct = nf_ct_get(skb, &ctinfo);       // Get the conntrack entry (nearly free)
if (ct == NULL) break;                // No conntrack → can't do per-flow DPI
if (ctinfo == IP_CT_UNTRACKED) break; // Admin said don't track → skip DPI

// Determine packet direction for nDPI
ct_dir = CTINFO2DIR(ctinfo) != IP_CT_DIR_ORIGINAL;
// Pass direction to nDPI so it knows which side is client vs server
```

Without conntrack, nDPI would have no concept of a "flow." It would see each packet independently and couldn't accumulate state across multiple packets (which is essential for TLS ClientHello parsing, HTTP header reassembly, etc.).

---

## Q5: What Is a Custom Conntrack Extension?

### Background: Conntrack Extensions System

`struct nf_conn` is the core conntrack structure, but it's kept small. Additional per-flow data is stored as **extensions** — dynamically attached blobs:

```
struct nf_conn
├── tuplehash[2]          (the 5-tuple, both directions)
├── status                (IPS_CONFIRMED, IPS_ASSURED, etc.)
├── timeout
├── ct_general.use        (refcount)
│
└── ext ──────────────► struct nf_ct_ext (extension area)
                         ├── [NF_CT_EXT_HELPER]   → conntrack helper data
                         ├── [NF_CT_EXT_NAT]      → NAT info
                         ├── [NF_CT_EXT_ACCT]      → byte/packet counters
                         ├── [NF_CT_EXT_ECACHE]    → event cache
                         ├── [NF_CT_EXT_LABELS]    → conntrack labels ← HIJACKED by nDPI
                         └── [NF_CT_EXT_SYNPROXY]  → SYN proxy data
```

### The Two Strategies in This Module

#### Strategy 1: Hijack `NF_CT_EXT_LABELS` (Default — No Kernel Patch)

The `NF_CT_EXT_LABELS` extension was designed for labeling flows (used by OVS/OpenFlow). The module repurposes this slot:

```c
// At init:
nf_ct_ext_id_ndpi = NF_CT_EXT_LABELS;  // Reuse the labels slot

// When accessing:
struct nf_ct_ext_labels *ct_label = __nf_ct_ext_find(ct, nf_ct_ext_id_ndpi);
```

The `nf_ct_ext_labels` struct is only 8/16 bytes (32/64-bit). It stores a **magic number** (0xa55a) and a **pointer** to the real nDPI data allocated separately:

```c
struct nf_ct_ext_labels {
    uint8_t   words, pad1;
    uint16_t  magic;            // 0xa55a = "this is ours"
    struct nf_ct_ext_ndpi *ndpi_ext;  // pointer to separately allocated struct
};
```

**Why the indirection?** Because the labels extension has a fixed small size. The actual `nf_ct_ext_ndpi` is ~200 bytes and is allocated from a dedicated `kmem_cache` (`ct_info_cache`).

**Why the magic number?** Because other modules (OVS) might also use `NF_CT_EXT_LABELS` on the same conntrack entry. The magic distinguishes "this is an nDPI label" from "this is a real conntrack label."

#### Strategy 2: True Custom Extension (`CONFIG_NF_CONNTRACK_CUSTOM` — Requires Kernel Patch)

If the kernel is patched to support custom extension registration:

```c
// A new extension type is registered:
static struct nf_ct_ext_type ndpi_extend = {
    .destroy = nf_ndpi_free_flow,           // called when ct is destroyed
    .len     = sizeof(struct nf_ct_ext_labels),
    .align   = __alignof__(uint32_t),
};

// Registered at init:
nf_ct_ext_id_ndpi = nf_ct_extend_custom_register(&ndpi_extend, 0x4e445049);
// 0x4e445049 = "NDPI" in ASCII hex

// When a new flow is seen, add extension:
nf_ct_ext_add(ct, nf_ct_ext_id_ndpi, GFP_ATOMIC);
```

**Advantages of Strategy 2:**
- No collision with OVS/conntrack labels
- Kernel automatically calls `.destroy` when conntrack entry is freed (clean lifecycle)
- No magic number hack needed

**Disadvantage:**
- Requires patching the kernel source (`nf_conntrack_extend.c`) — not possible with stock OpenWrt or Qualcomm SDK kernels without custom patches

> [!TIP]
> **Interview tip**: If asked "why not always use custom extensions?" — the answer is **portability**. The labels-hijack approach works on any stock kernel ≥3.8 without patches. For a BSP package targeting multiple Qualcomm SoCs across different SDK versions, this portability is essential.

---

## Q6: Why Free `ndpi_flow_struct` After `detect_done`? What If the Protocol Changes?

### Why Free It — Memory Pressure

```
sizeof(ndpi_flow_struct) ≈ 5,400 bytes  (printed at module load as ndpi_size_flow_struct)
sizeof(nf_ct_ext_ndpi)   ≈   200 bytes  (the persistent per-flow state)
```

On an embedded Qualcomm router with 256MB RAM handling 50,000 concurrent flows:

|               | With flow struct alive          | After freeing flow struct |
|---------------|---------------------------------|---------------------------|
| Per-flow cost | 5,400 + 200 = **5,600 bytes**   | **200 bytes**             |
| 50k flows     | **280 MB** (exceeds total RAM!) | **10 MB**                 |

The `ndpi_flow_struct` contains:
- Packet buffers for protocol dissection (TLS reassembly buffers, HTTP line buffers)
- Per-protocol state machines (TCP sequence tracking, STUN transaction IDs)
- Internal nDPI automata state

**None of this is needed after classification.** Once we know "this is TLS/YouTube," we only need the protocol ID, the SNI string, and the host string — all of which are copied out before the flow struct is freed.

### The Freeing Code Path

```c
// In ndpi_mt(), after ndpi_process_packet() returns a known protocol:
if (r_proto != NDPI_PROTOCOL_UNKNOWN) {
    proto = ct_ndpi->proto;                    // Protocol ID saved in ct_ndpi
    if (test_flow_yes(ct_ndpi))
        ndpi_host_ssl(ct_ndpi);                // Copy SNI/host strings BEFORE freeing
    atomic_inc(&n->protocols_cnt[proto.app_protocol]);
}

// Later, if packet_counter exceeds threshold OR detection succeeded:
// The flow struct is freed:
if (ct_ndpi->flow->packet_counter > max_packet_unk_tcp) {
    set_detect_done(ct_ndpi);                  // Mark: "don't run DPI again"
    __ndpi_free_ct_flow(ct_ndpi);              // Free the 5KB flow struct
}

static inline void __ndpi_free_ct_flow(struct nf_ct_ext_ndpi *ct_ndpi) {
    if (ct_ndpi->flow != NULL) {
        ndpi_free_flow(ct_ndpi->flow);         // nDPI library cleanup
        kmem_cache_free(osdpi_flow_cache, ct_ndpi->flow);  // Return to slab
        ct_ndpi->flow = NULL;
        COUNTER(ndpi_flow_d);
        module_put(THIS_MODULE);               // Allow module unload
    }
}
```

### What If the Protocol Changes Mid-Flow?

**Short answer: It can't, and here's why.**

In the TCP/IP model, the application protocol is established during the **connection setup phase**:

```
TCP SYN → SYN-ACK → ACK → [ClientHello / HTTP GET / etc.] → Protocol identified
                                                              │
                                                              ▼
                                                      All subsequent packets
                                                      are the SAME protocol
```

A TCP connection that starts as TLS **cannot** become HTTP mid-stream. The protocol is determined by:
- The port (initial hint)
- The first few application-layer bytes (deterministic)
- TLS: ClientHello → ServerHello → encrypted (stays TLS forever)
- HTTP: `GET / HTTP/1.1\r\n` → response → stays HTTP forever

**Even multiplexed protocols are handled:**
- HTTP/2 over TLS: classified as `TLS.HTTP` (master=TLS, app=HTTP)
- QUIC: classified as `QUIC.YouTube` (master=QUIC, app=YouTube)

**The `check_extra_packets` mechanism:**

Some protocols need a few more packets **after** initial detection to refine the classification (e.g., TLS detected after ClientHello, but sub-protocol like YouTube needs to see the ServerHello certificate). In this case:

```c
// Even after proto != UNKNOWN, flow is kept alive if:
if (ct_ndpi->proto.app_protocol != NDPI_PROTOCOL_UNKNOWN &&
    ct_ndpi->flow->check_extra_packets) {
    // Keep flow struct alive, continue running DPI
    // nDPI may upgrade "TLS.Unknown" → "TLS.YouTube"
}
// Only after check_extra_packets becomes false OR max packets reached:
// → set_detect_done() + __ndpi_free_ct_flow()
```

> [!WARNING]
> **Edge case**: If a malicious actor sends a valid TLS ClientHello and then switches to raw TCP binary protocol, nDPI has already classified the flow as TLS. The subsequent non-TLS data is simply forwarded as TLS. This is **not a bug** — it's by design. In-band protocol changes within a single TCP connection are not a real-world scenario for legitimate traffic.

---

## Q7: How Does the Bitmask Protocol Matching Work?

### The Data Structure

When the admin writes:
```bash
iptables -m ndpi --proto tls,http,quic -j DROP
```

The user-space `libxt_ndpi.so` translates this into a bitmask inside `struct xt_ndpi_mtinfo`:

```c
struct xt_ndpi_mtinfo {
    NDPI_PROTOCOL_BITMASK flags;    // ← This is the bitmask
    unsigned short int invert:1, error:1, m_proto:1, p_proto:1, 
                       have_master:1, host:1, ssl:1, re:1, empty:1;
    char hostname[...];
    void *reg_data;
};
```

### What Is `NDPI_PROTOCOL_BITMASK`?

It's a fixed-size bit array where **each bit position represents one nDPI protocol ID**:

```
NDPI_PROTOCOL_BITMASK = array of unsigned long integers

Bit 0  = NDPI_PROTOCOL_UNKNOWN      (0)
Bit 7  = NDPI_PROTOCOL_HTTP         (7)
Bit 91 = NDPI_PROTOCOL_TLS          (91)
Bit 188 = NDPI_PROTOCOL_QUIC        (188)
...
Bit N  = NDPI_NUM_BITS - 1          (max ~300+)

Total size: NDPI_NUM_BITS / 8 bytes ≈ 40 bytes (for ~320 protocols)
```

The command `--proto tls,http,quic` sets bits 91, 7, and 188 to `1` in the bitmask. All other bits are `0`.

### How Matching Works in `ndpi_mt()`

After the DPI engine determines the protocol (e.g., `app_protocol = 91 (TLS)`, `master_protocol = 0 (UNKNOWN)`), the match function checks:

```c
// Simplified from ndpi_mt() lines 1508-1534:

// Case 1: Only checking app_protocol (--proto X)
if (!info->m_proto && info->p_proto) {
    result = NDPI_COMPARE_PROTOCOL_TO_BITMASK(info->flags, proto.app_protocol) != 0;
    // Translates to: is bit 91 set in flags? → YES → match!
}

// Case 2: Only checking master_protocol (--master-proto X)
if (info->m_proto && !info->p_proto) {
    result = NDPI_COMPARE_PROTOCOL_TO_BITMASK(info->flags, proto.master_protocol) != 0;
}

// Case 3: Checking both (default --proto behavior)
if (proto.app_protocol != NDPI_PROTOCOL_UNKNOWN) {
    result = NDPI_COMPARE_PROTOCOL_TO_BITMASK(info->flags, proto.app_protocol) != 0;
    if (proto.master_protocol != NDPI_PROTOCOL_UNKNOWN)
        result |= NDPI_COMPARE_PROTOCOL_TO_BITMASK(info->flags, proto.master_protocol) != 0;
}
```

### What `NDPI_COMPARE_PROTOCOL_TO_BITMASK` Actually Does

```c
// This macro checks if bit N is set in the bitmask:
#define NDPI_COMPARE_PROTOCOL_TO_BITMASK(bmask, protocol) \
    (bmask.fds_bits[(protocol) / (sizeof(unsigned long)*8)] & \
     (1UL << ((protocol) % (sizeof(unsigned long)*8))))
```

This is a **single array index + bitwise AND** — O(1), constant time, ~3 CPU instructions.

### Example Walkthrough

```
Admin rule:    -m ndpi --proto tls,http
Bitmask:       flags.fds_bits[...] has bits 7 (HTTP) and 91 (TLS) set

Packet arrives, DPI classifies as: app_protocol=91 (TLS), master_protocol=0

Check: NDPI_COMPARE_PROTOCOL_TO_BITMASK(flags, 91)
  → flags.fds_bits[91/64] & (1UL << (91 % 64))
  → flags.fds_bits[1] & (1UL << 27)
  → Non-zero → MATCH!

Result: ndpi_mt() returns true → the -j DROP target executes → packet dropped
```

### The Invert Flag

```bash
iptables -m ndpi ! --proto tls -j ACCEPT
```

The `!` sets `info->invert = 1`, and the final return is XORed:

```c
return (result & host_match) ^ (info->invert != 0);
// If result=true and invert=true → false (don't match TLS → accept everything else)
```

---

## Q8: What Are the NAT Detection Hooks and Why Are They Separate?

### The Problem

When Linux applies NAT (SNAT/DNAT), it **rewrites packet headers** between hookpoints:

```
Incoming packet:  src=1.2.3.4:5000 → dst=ROUTER_PUBLIC_IP:80
                          │
              NF_INET_PRE_ROUTING
                          │
              NAT (DNAT): dst → 192.168.1.100:8080    ← Address rewritten HERE
                          │
              FORWARD chain → ndpi_mt() sees:
                  src=1.2.3.4:5000 → dst=192.168.1.100:8080
                  (post-NAT addresses)
                          │
              NF_INET_POST_ROUTING
                          │
              NAT (SNAT): src → 10.0.0.1:12345         ← Another rewrite HERE
```

**The DPI engine sees post-NAT addresses**, but for flow accounting/logging, we need to know **both** the original AND NATted addresses (to tell the admin "this YouTube flow from LAN client 192.168.1.50 consumed 500MB").

### How Conntrack Stores NAT Information

Conntrack stores the **original** tuple and the **reply** tuple:

```c
ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple = {src=1.2.3.4:5000, dst=ROUTER:80}
ct->tuplehash[IP_CT_DIR_REPLY].tuple    = {src=192.168.1.100:8080, dst=1.2.3.4:5000}
//                                          ▲ This is the DNAT'd address
```

If DNAT was applied, `original.dst ≠ reply.src`. If SNAT was applied, `original.src ≠ reply.dst`.

### What the NAT Hooks Do

The three hooks registered by `nf_nat_ipv4_ops[]` run the function `ndpi_nat_do_chain()`:

```c
static unsigned int ndpi_nat_do_chain(void *priv, struct sk_buff *skb,
                                       const struct nf_hook_state *state)
{
    // 1. Check if this packet belongs to a flow we're tracking
    c_proto = skb_get_cproto(skb);
    if (c_proto->magic != NDPI_ID) break;                    // Not our packet
    if (ct_proto_get_flow_nat(c_proto) != FLOW_NAT_START) break;  // Already done

    // 2. Get conntrack entry
    ct = nf_ct_get(skb, &ctinfo);
    ct_ndpi = nf_ct_ext_find_ndpi(ct);

    // 3. Compare original tuple vs reply tuple to detect NAT
    spin_lock_bh(&ct_ndpi->lock);
    if (!test_nat_done(ct_ndpi)) {
        ndpi_nat_detect(ct_ndpi, ct);      // ← THE COMPARISON
        
        if (state->hook != NF_INET_PRE_ROUTING) {
            // We've seen both PRE and POST routing → NAT detection complete
            set_nat_done(ct_ndpi);
            ct_proto_set_flow_nat(c_proto, FLOW_NAT_END);
        }
    }
    spin_unlock_bh(&ct_ndpi->lock);

    return NF_ACCEPT;  // Always accept — we're just observing
}
```

### The `ndpi_nat_detect()` Function

```c
static void ndpi_nat_detect(struct nf_ct_ext_ndpi *ct_ndpi, struct nf_conn *ct) {
    // Compare addresses stored at flow init vs reply tuple
    
    // DNAT detection: original.dst != reply.src?
    ns_ip = memcmp(&ct_ndpi->flinfo.ip_d, &reply_tuple->src.u3, addr_size);
    ns_port = ct_ndpi->flinfo.dport != reply_tuple->src.u.tcp.port;
    
    if (ns_ip || ns_port) {
        set_dnat(ct_ndpi);                              // Flag: DNAT detected
        memcpy(&ct_ndpi->flinfo.ip_dnat, ...);          // Save translated dst IP
        ct_ndpi->flinfo.dport_nat = reply_tuple->src.u.tcp.port;  // Save translated port
    }
    
    // SNAT detection: original.src != reply.dst?
    nd_ip = memcmp(&ct_ndpi->flinfo.ip_s, &reply_tuple->dst.u3, addr_size);
    nd_port = ct_ndpi->flinfo.sport != reply_tuple->dst.u.tcp.port;
    
    if (nd_ip || nd_port) {
        set_snat(ct_ndpi);                              // Flag: SNAT detected
        memcpy(&ct_ndpi->flinfo.ip_snat, ...);          // Save translated src IP
        ct_ndpi->flinfo.sport_nat = reply_tuple->dst.u.tcp.port;
    }
}
```

### Why Three Separate Hook Points?

| Hook                                              | Priority         | Purpose                                     |
|---------------------------------------------------|------------------|---------------------------------------------|
| `NF_INET_PRE_ROUTING` at `NF_IP_PRI_NAT_DST + 1`  | After DNAT       | See the DNAT'd destination before routing   |
| `NF_INET_POST_ROUTING` at `NF_IP_PRI_NAT_SRC + 1` | After SNAT       | See the SNAT'd source before egress         |
| `NF_INET_LOCAL_IN` at `NF_IP_PRI_NAT_SRC + 1`     | After local SNAT | Catch traffic destined to the router itself |

The `+1` in priority ensures these hooks run **just after** the kernel's NAT engine, so the conntrack reply tuple is already populated with NATted addresses.

### Why This Can't Be Done in xtables

The xtables match (`ndpi_mt`) runs in the **FORWARD** chain's filter table, which is **between** PRE_ROUTING and POST_ROUTING. At that point:
- DNAT has already been applied (PRE_ROUTING/nat finished)
- SNAT has NOT yet been applied (POST_ROUTING/nat hasn't run)

So the match function only sees **half** the NAT picture. The separate hooks at PRE_ROUTING+1 and POST_ROUTING+1 see the full picture at each stage.

### When Are These Hooks Registered?

**Only when flow accounting is enabled:**

```c
// In ndpi_net_init():
if (ndpi_enable_flow &&
    nf_register_net_hooks(net, nf_nat_ipv4_ops, ARRAY_SIZE(nf_nat_ipv4_ops)))
    break;  // registration failed

// ndpi_enable_flow is set via module parameter:
// insmod xt_ndpi.ko ndpi_enable_flow=1
```

If flow accounting is off (default), these hooks are never registered, and no NAT detection overhead exists.

---

## Summary: Quick-Reference Answer Table

| Question                      | One-Line Answer                                                                                  |
|-------------------------------|--------------------------------------------------------------------------------------------------|
| Why xtables, not raw hooks?   | Selective activation, iptables CLI for free, composable with other modules                       |
| How packet travels iptables?  | `nf_hook` → `ipt_do_table()` → sequential rule eval → `match()` → `target()`                     |
| What is `SO_SET_REPLACE`?     | Syscall that user-space `iptables` uses to push the binary rule blob into the kernel             |
| How conntrack tracks packets? | First packet: 5-tuple hash → `nf_conn` alloc → stash in `skb->_nfct`                             |
| How `nf_ct_get()` works?      | Reads `skb->_nfct`, masks low 3 bits = ctinfo, rest = pointer. ~2 instructions.                  |
| What is custom CT extension?  | Mechanism to attach per-flow data to `struct nf_conn` via registered extension slots             |
| Why free flow struct?         | It's ~5KB vs ~200B for ct_ndpi. 50k flows = 280MB vs 10MB. Protocol can't change mid-connection. |
| How bitmask matching works?   | Bit N = protocol N. Check with array index + bitwise AND. O(1), ~3 instructions.                 |
| What are NAT hooks for?       | Compare original vs reply conntrack tuples to detect SNAT/DNAT addresses for flow logs           |
