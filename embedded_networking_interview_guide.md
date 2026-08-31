# Embedded Networking Interview Master Reference
## For WLAN / Embedded Software Developer Roles

> [!NOTE]
> **Depth Legend** used throughout this document:
> - 🔴 **Deep Dive** — Expect multi-level follow-ups. You must know packet flows, state machines, and tradeoffs.
> - 🟡 **Solid Understanding** — Know concepts, one example, and embedded-specific implications.
> - 🟢 **Surface Familiarity** — Know definitions and "when/why." Rarely grilled beyond basics.
>
> **[MISSING]** tags mark topics that were **not present** in your original notes and have been added.

---

## Table of Contents

1. [Layer 1 & 2 Fundamentals (Physical & Data Link)](#1-layer-1--2-fundamentals-physical--data-link-)
2. [802.11 / WLAN — Deep Dive (Your Specialty)](#2-80211--wlan--deep-dive-)
3. [Layer 3: Network Layer (IP & Routing)](#3-layer-3-network-layer-ip--routing-)
4. [Layer 4: Transport Layer (TCP & UDP)](#4-layer-4-transport-layer-tcp--udp-)
5. [Sockets & Network Programming](#5-sockets--network-programming-)
6. [Application Layer Protocols](#6-application-layer-protocols-)
7. [Network Security](#7-network-security-)
8. [Performance & Troubleshooting](#8-performance--troubleshooting-)

---

## 1. Layer 1 & 2 Fundamentals (Physical & Data Link) 🟡

### 1.1 OSI vs TCP/IP Model

| OSI Layer | TCP/IP Layer | Embedded Examples | PDU |
|-----------|-------------|-------------------|-----|
| 7. Application | Application | MQTT, HTTP, CoAP, DNS, DHCP | Data |
| 6. Presentation | Application | TLS/SSL, JSON, Protobuf | Data |
| 5. Session | Application | Socket sessions, RPC | Data |
| 4. Transport | Transport | TCP, UDP | Segment/Datagram |
| 3. Network | Internet | IP, ICMP, ARP, IGMP | Packet |
| 2. Data Link | Network Access | Ethernet MAC, Wi-Fi (802.11), PPP | Frame |
| 1. Physical | Network Access | Ethernet PHY, RS-485, RF (2.4/5/6 GHz) | Bits |

**Interview answer:** "The OSI model is a 7-layer theoretical reference. In practice, embedded systems use the TCP/IP 4-layer model. Layers 5–7 collapse into the Application layer. The key insight for embedded is that layers 1–2 are where hardware meets software — that's where your MAC/PHY driver sits."

**Gotcha:** "Which layer does ARP operate at?" → ARP is tricky. It maps Layer 3 (IP) to Layer 2 (MAC) addresses. It uses Ethernet frames directly (no IP header), so it's technically a Layer 2.5 protocol.

### 1.2 Ethernet Frame Structure

```
┌──────────┬──────────┬──────────┬──────────┬──────────────┬─────┐
│ Preamble │ Dest MAC │ Src MAC  │ Type/Len │   Payload    │ FCS │
│  8 bytes │ 6 bytes  │ 6 bytes  │ 2 bytes  │ 46-1500 bytes│4 B  │
└──────────┴──────────┴──────────┴──────────┴──────────────┴─────┘

Type field values:
  0x0800 = IPv4
  0x0806 = ARP
  0x86DD = IPv6
  0x8100 = 802.1Q VLAN tag
```

**FCS (Frame Check Sequence):** CRC-32 computed by the MAC hardware. If the CRC doesn't match on receive, the frame is silently dropped (no error notification — reliability is left to upper layers).

### 1.3 MAC Addressing

**Definition:** A 48-bit (6-byte) hardware address burned into the NIC, written as `AA:BB:CC:DD:EE:FF`. The first 3 bytes are the OUI (Organizationally Unique Identifier — identifies the vendor), the last 3 are vendor-assigned.

**Special addresses:**

| Address | Meaning |
|---------|---------|
| `FF:FF:FF:FF:FF:FF` | Broadcast — all devices on the LAN |
| `01:00:5E:xx:xx:xx` | IPv4 Multicast |
| `33:33:xx:xx:xx:xx` | IPv6 Multicast |
| Bit 0 of first byte = 1 | Multicast bit (group address) |
| Bit 1 of first byte = 1 | Locally administered (e.g., VMs, randomized MACs) |

**Embedded relevance:** Wi-Fi chips support MAC address randomization for privacy (required in modern mobile devices). Your driver may need to handle changing MACs during scans vs association.

### 1.4 Ethernet PHY, MAC, and Interfaces

**MAC (Media Access Control):** Usually inside the MCU/SoC. Handles framing, CRC, addressing, collision handling (CSMA/CD for half-duplex).

**PHY (Physical Layer Transceiver):** Usually an external chip (LAN8720, DP83848). Converts digital MAC signals to electrical signals for the wire. Handles line coding, auto-negotiation, link detection.

**MAC-to-PHY Interfaces:**

| Interface | Pins | Speed | Notes |
|-----------|------|-------|-------|
| MII | 16 | 10/100 Mbps | Legacy, pin-heavy |
| RMII | 9 | 10/100 Mbps | Preferred for pin-constrained MCUs (50 MHz clock) |
| RGMII | 12 | 10/100/1000 Mbps | Gigabit, uses DDR clocking |
| SGMII | 4 (serial) | 10/100/1000 Mbps | Serial interface, used in SoCs |
| MDIO | 2 (clk+data) | Management | Configures PHY registers (speed, duplex, status) |

**Auto-Negotiation:** Two PHYs exchange capability advertisements (Base Pages) and agree on the highest common mode. A **duplex mismatch** (one side forced full-duplex, other auto-negotiated half) causes massive packet loss — a classic embedded networking debug scenario.

### 1.5 Hub vs Switch vs Router

| Device | Layer | Forwards by | Collision Domains | Broadcast Domains |
|--------|-------|-------------|-------------------|-------------------|
| Hub | L1 | Nothing — floods all ports | 1 (shared) | 1 |
| Switch | L2 | MAC address (CAM table) | 1 per port (isolated) | 1 |
| Router | L3 | IP address (routing table) | 1 per port | 1 per port (breaks broadcasts) |

**Switch MAC learning:** When a frame arrives on port 3 with source MAC `AA:BB:CC:DD:EE:FF`, the switch records: "MAC `AA:BB:CC` is on port 3." If the destination MAC isn't in the table, the switch **floods** the frame to all ports except the source (unknown unicast flooding).

### 1.6 Collision Domain vs Broadcast Domain — **[MISSING]**

**Collision domain:** A network segment where simultaneous transmissions cause collisions. Each switch port is its own collision domain (full-duplex eliminates collisions entirely). Hubs share one collision domain across all ports.

**Broadcast domain:** A network segment where a broadcast frame reaches all devices. A switch is one broadcast domain (broadcasts flood all ports). Only a **router** (or VLAN) breaks broadcast domains.

**Embedded relevance:** In embedded industrial networks, excessive broadcasts (ARP storms, DHCP, mDNS) can saturate a constrained device's receive path. VLANs isolate broadcast domains.

```
Interview question: "How many collision domains and broadcast domains
in a network with 1 router, 2 switches, and 10 hosts?"

Answer: Each switch port = 1 collision domain, so at least 12 collision domains
        (10 hosts + 2 switch-to-router links).
        The router breaks broadcast domains: 2 broadcast domains (one per switch).
```

### 1.7 VLANs (802.1Q)

**Definition:** Logically partitioning a physical switch into multiple broadcast domains. A 4-byte tag is inserted into the Ethernet frame containing a 12-bit VLAN ID (0–4095).

```
Standard frame:  [Dst MAC][Src MAC][Type][Payload][FCS]
Tagged frame:    [Dst MAC][Src MAC][0x8100][VLAN Tag][Type][Payload][FCS]
                                    ↑ TPID    ↑ 12-bit VLAN ID + 3-bit priority (PCP)
```

**Port types:**
- **Access port:** Carries traffic for one VLAN. Frames enter/exit untagged. The switch adds/removes the tag internally.
- **Trunk port:** Carries traffic for multiple VLANs. Frames are tagged on the wire.

**Embedded relevance:** In automotive/industrial Ethernet, VLANs separate safety-critical traffic from infotainment. In WLAN, VLANs map to different SSIDs (e.g., guest vs corporate networks on the same AP).

### 1.8 CSMA/CD vs CSMA/CA — **[MISSING]**

| Protocol | Used By | How it handles collisions |
|----------|---------|--------------------------|
| CSMA/CD (Collision Detection) | Wired Ethernet (half-duplex) | Listen before sending. If collision detected during transmission, stop and retry after random backoff. Modern full-duplex Ethernet doesn't use this. |
| CSMA/CA (Collision Avoidance) | Wi-Fi (802.11) | Listen before sending. Wait for the medium to be clear + a random backoff. Uses ACKs to confirm reception. Optional RTS/CTS for hidden node problem. |

**Why CD doesn't work for wireless:** In wireless, a station can't reliably detect collisions during transmission because the outgoing signal drowns out incoming signals (near-far problem). So Wi-Fi avoids collisions proactively rather than detecting them.

### 1.9 Flow Control (IEEE 802.3x)

**PAUSE frames:** If a receiver's buffer is filling up, it sends a PAUSE frame telling the sender to stop for a specified time (in quanta of 512 bit-times).

**Embedded relevance:** On resource-constrained MCUs with small DMA buffers, PAUSE frames prevent packet loss during burst traffic. Check that your PHY/MAC supports and enables 802.3x flow control.

---

## 2. 802.11 / WLAN — Deep Dive 🔴

> [!IMPORTANT]
> **This section is almost entirely [MISSING] from your notes.** Your original notes only covered SSID, RSSI, STA/AP modes, PSK, and WPA2/WPA3 basics. For a WLAN-focused embedded role, you need deep knowledge of everything below. This is where interviewers will spend 50%+ of their time.

### 2.1 802.11 Standards Overview

| Standard | Band | Max PHY Rate | Channel Width | Key Technology | Year |
|----------|------|-------------|---------------|----------------|------|
| 802.11a | 5 GHz | 54 Mbps | 20 MHz | OFDM | 1999 |
| 802.11b | 2.4 GHz | 11 Mbps | 22 MHz | DSSS/CCK | 1999 |
| 802.11g | 2.4 GHz | 54 Mbps | 20 MHz | OFDM | 2003 |
| 802.11n (Wi-Fi 4) | 2.4/5 GHz | 600 Mbps | 20/40 MHz | MIMO, HT | 2009 |
| 802.11ac (Wi-Fi 5) | 5 GHz | 6.93 Gbps | 20/40/80/160 MHz | MU-MIMO (DL), VHT | 2013 |
| 802.11ax (Wi-Fi 6/6E) | 2.4/5/6 GHz | 9.6 Gbps | 20/40/80/160 MHz | OFDMA, BSS Coloring, TWT | 2020 |
| 802.11be (Wi-Fi 7) | 2.4/5/6 GHz | 46 Gbps | Up to 320 MHz | MLO, 4096-QAM, EHT | 2024 |

### 2.2 WLAN Architecture & Terminology

| Term | Definition |
|------|-----------|
| **STA** (Station) | Any device with a WLAN interface (client or AP) |
| **AP** (Access Point) | Station that provides access to the DS (Distribution System) |
| **BSS** (Basic Service Set) | An AP + its associated STAs. Identified by BSSID (= AP's MAC) |
| **ESS** (Extended Service Set) | Multiple BSSs with the same SSID, enabling roaming |
| **IBSS** | Ad-hoc network (peer-to-peer, no AP) |
| **DS** (Distribution System) | The backbone connecting APs (usually wired Ethernet) |
| **SSID** | Human-readable network name (up to 32 bytes) |
| **BSSID** | MAC address of the AP's radio interface |
| **RSSI** | Received Signal Strength Indicator (dBm). -30 = excellent, -70 = fair, -85 = poor |

### 2.3 802.11 Frame Format — **[MISSING]**

```
┌──────────┬────────────┬───────┬───────┬───────┬───────┬──────┬──────┬─────────┬─────┐
│Frame Ctrl│Duration/ID │Addr 1 │Addr 2 │Addr 3 │Seq Ctl│Addr 4│ QoS  │  Body   │ FCS │
│ 2 bytes  │  2 bytes   │6 bytes│6 bytes│6 bytes│2 bytes│6 B   │2 B   │0-2304 B │4 B  │
└──────────┴────────────┴───────┴───────┴───────┴───────┴──────┴──────┴─────────┴─────┘

Frame Control subfields:
  - Type (2 bits):    00=Management, 01=Control, 10=Data
  - Subtype (4 bits): Specific frame type
  - To DS / From DS:  Direction of the frame
  - Protected Frame:  1 = frame body is encrypted
```

**Address fields vary by direction:**

| To DS | From DS | Addr 1 (RA) | Addr 2 (TA) | Addr 3 | Meaning |
|-------|---------|-------------|-------------|--------|---------|
| 0 | 0 | DA | SA | BSSID | IBSS (ad-hoc) |
| 1 | 0 | BSSID | SA | DA | STA → AP (client to AP) |
| 0 | 1 | DA | BSSID | SA | AP → STA (AP to client) |
| 1 | 1 | RA | TA | DA | WDS (AP to AP, 4 addresses used) |

**Frame types:**

| Type | Subtype Examples | Purpose |
|------|-----------------|---------|
| Management | Beacon, Probe Req/Resp, Auth, Assoc Req/Resp, Deauth, Disassoc, Action | Network discovery, joining, leaving |
| Control | ACK, RTS, CTS, Block ACK, CF-End | Medium access control |
| Data | Data, Null Data, QoS Data, QoS Null | Actual payload transport |

### 2.4 Scanning — **[MISSING]**

**Passive scanning:** STA listens on each channel for Beacon frames from APs. Slower but required on DFS channels (where active probing is restricted to avoid interfering with radar).

**Active scanning:** STA sends Probe Request on each channel and listens for Probe Responses. Faster but generates more traffic.

```
Active Scan flow:
STA ──Probe Request (broadcast)──→  All APs on channel
STA ←──Probe Response──────────── AP1 (matching SSID)
STA ←──Probe Response──────────── AP2 (matching SSID)
// STA builds a scan list, picks the best AP (RSSI, load, etc.)
```

**Embedded relevance:** Scan time directly impacts connection latency and power consumption. Optimizations include specifying target channels, setting minimum dwell time, and aborting early when a good candidate is found.

### 2.5 Association & Authentication Flow — **[MISSING]**

**Open System Authentication + WPA2-PSK Association:**

```
Step 1: STA discovers AP (via scanning)

Step 2: 802.11 Authentication (Open System — always succeeds)
  STA ──Auth Request (Open)──→ AP
  STA ←──Auth Response (OK)── AP

Step 3: Association
  STA ──Association Request──→ AP
       (includes: SSID, supported rates, HT/VHT/HE capabilities,
        RSN IE with selected cipher suite)
  STA ←──Association Response── AP
       (includes: Association ID (AID), supported rates)

Step 4: 4-Way Handshake (EAPOL, for key establishment)
  (See §2.6)

Step 5: STA is now fully connected — data frames can flow
```

**State machine:**
```
State 1: Unauthenticated, Unassociated
   ↓ (Auth Request/Response)
State 2: Authenticated, Unassociated
   ↓ (Assoc Request/Response)
State 3: Authenticated, Associated
   ↓ (4-Way Handshake completes)
State 4: Connected (encryption active)
```

**Interview question:** "What's the difference between Authentication and Association in 802.11?" → Authentication is the identity verification step (trivial in Open System, meaningful in 802.1X/EAP). Association is the AP granting the STA permission to join the BSS and allocating resources (AID). They're separate steps.

### 2.6 WPA2 4-Way Handshake — **[MISSING]**

**Purpose:** Establishes per-session encryption keys (PTK — Pairwise Transient Key) between the STA and AP, and optionally delivers the GTK (Group Temporal Key) for broadcast/multicast.

**Prerequisites:** Both sides know the PMK (Pairwise Master Key):
- WPA2-PSK: `PMK = PBKDF2(passphrase, SSID, 4096 iterations, 256 bits)`
- WPA2-Enterprise: PMK derived from EAP authentication via RADIUS

```
Step 1: AP → STA: EAPOL Message 1
  Contains: ANonce (AP's random nonce)
  STA now has: PMK + ANonce + SNonce (self-generated) + MAC addresses
  STA computes: PTK = PRF(PMK, ANonce, SNonce, AA, SPA)
  PTK splits into: KCK (Key Confirmation) + KEK (Key Encryption) + TK (Temporal Key)

Step 2: STA → AP: EAPOL Message 2
  Contains: SNonce + MIC (Message Integrity Check, computed using KCK)
  AP now has: PMK + ANonce + SNonce + MAC addresses
  AP computes: PTK (same as STA computed)
  AP verifies: MIC — if correct, STA knows the correct PMK (authentication proof)

Step 3: AP → STA: EAPOL Message 3
  Contains: GTK (encrypted with KEK) + MIC + Install flag
  AP installs the PTK

Step 4: STA → AP: EAPOL Message 4
  Contains: ACK (MIC'd)
  STA installs the PTK
  Both sides now encrypt data frames using the TK portion of PTK
```

**Key hierarchy:**
```
PSK / EAP
   ↓
PMK (Pairwise Master Key, 256-bit)
   ↓  PRF-384 or PRF-512
PTK (Pairwise Transient Key)
   ├── KCK (128-bit): Key Confirmation Key — for MIC in EAPOL
   ├── KEK (128-bit): Key Encryption Key — encrypts GTK delivery
   └── TK  (128/256-bit): Temporal Key — actual data encryption (AES-CCMP)
```

> [!CAUTION]
> **Interview gotcha:** "What happens if message 3 is replayed?" → This is the **KRACK attack** (Key Reinstallation Attack, 2017). If message 3 is replayed, the STA reinstalls the PTK and resets the nonce counter, allowing an attacker to decrypt/forge frames. The fix: STA must not reinstall an already-installed key.

### 2.7 Beacons & TIM/DTIM — **[MISSING]**

**Beacon:** A management frame broadcast by the AP at regular intervals (default: 100 TU = ~102.4 ms). Contains: SSID, supported rates, channel, timestamp, capabilities, RSN IE (security), TIM.

**TIM (Traffic Indication Map):** A bitmap in the Beacon indicating which power-saving STAs have buffered unicast frames waiting at the AP. A sleeping STA wakes up for beacons, checks the TIM; if its AID bit is set, it sends PS-Poll to retrieve buffered frames.

**DTIM (Delivery TIM):** Every Nth beacon (DTIM interval, typically 1–3) is a DTIM beacon, which indicates pending broadcast/multicast frames. All sleeping STAs must wake for DTIM beacons.

```
Beacon timeline (DTIM interval = 3):
  B1(TIM) ── B2(TIM) ── B3(DTIM) ── B4(TIM) ── B5(TIM) ── B6(DTIM) ...

STA power-save strategy:
  - Deep sleepers: Wake only for DTIM beacons (miss unicast TIM → higher latency)
  - Light sleepers: Wake for every beacon, check TIM
  - Latency-critical: Stay awake (no power save)
```

### 2.8 Power Save Mechanisms — **[MISSING]**

| Mechanism | Standard | How it works | Latency tradeoff |
|-----------|----------|-------------|-------------------|
| Legacy PS (PS-Poll) | 802.11 | STA sleeps, wakes at beacons, polls AP for buffered frames | Beacon-interval latency (~100ms) |
| U-APSD | 802.11e | STA uses trigger frames to retrieve buffered frames on specific access categories | Per-AC control, better for VoIP |
| TWT (Target Wake Time) | 802.11ax (Wi-Fi 6) | STA and AP negotiate specific wake times; STA sleeps between | Predictable, excellent for IoT sensors |

**TWT (Target Wake Time) — important for embedded IoT:**

```
STA ──TWT Setup Request──→ AP
      (I want to wake every 10 seconds, for 5ms each time)
STA ←──TWT Setup Response── AP
      (Agreed: your TWT schedule starts at time T)

// STA sleeps deeply between TWT windows
// At each TWT window: STA wakes → exchanges data → sleeps
```

**Embedded relevance:** TWT is a game-changer for battery-powered Wi-Fi IoT devices. Previously, Wi-Fi was power-hungry compared to BLE/Zigbee. TWT enables Wi-Fi devices to achieve months of battery life.

### 2.9 QoS & WMM (Wi-Fi Multimedia) — **[MISSING]**

**Problem:** Wi-Fi is a shared medium. A bulk file download shouldn't starve a VoIP call.

**WMM defines 4 Access Categories (AC):**

| Access Category | 802.11e AC | Maps to IP DSCP | Typical Traffic | Priority |
|----------------|------------|-----------------|-----------------|----------|
| Voice (VO) | AC_VO | EF (46) | VoIP | Highest |
| Video (VI) | AC_VI | AF41 (34) | Video streaming | High |
| Best Effort (BE) | AC_BE | CS0 (0) | Web, email | Normal |
| Background (BK) | AC_BK | CS1 (8) | Bulk transfers | Lowest |

**How QoS works at L2 (EDCA — Enhanced Distributed Channel Access):**
- Each AC has its own transmit queue with different parameters:
  - **AIFS (Arbitration IFS):** Higher-priority AC waits less before transmitting
  - **CWmin/CWmax:** Higher-priority AC has smaller contention window → wins the channel faster
  - **TXOP Limit:** How long a station can hold the channel once it wins access

```
Frame priority flow:
  IP packet with DSCP=46 (VoIP)
    → classified as AC_VO
    → goes into Voice queue
    → uses short AIFS + small CW
    → gets on-air faster than a background download in AC_BK queue
```

**Embedded relevance:** In WLAN driver development, you implement the queue mapping logic and program EDCA parameters into the hardware. Misconfigured QoS means VoIP crackles while someone downloads a file.

### 2.10 Rate Adaptation (Rate Control) — **[MISSING]**

**Definition:** Dynamically selecting the optimal MCS (Modulation and Coding Scheme) based on channel conditions. Higher MCS = faster but less reliable.

**How it works conceptually:**
```
Good channel (close to AP, no interference):
  → Use high MCS (e.g., MCS 9 = 256-QAM, 5/6 coding) → fast but fragile

Poor channel (far from AP, interference):
  → Fall back to low MCS (e.g., MCS 0 = BPSK, 1/2 coding) → slow but robust

Algorithm observes: TX success/failure rate, RSSI, SNR
  If many retries → decrease MCS
  If consistently succeeding → increase MCS
```

**Common algorithms:**
- **Minstrel (Linux mac80211):** Statistics-based. Tries different rates periodically and picks the one with the highest throughput.
- **Vendor-proprietary:** Qualcomm, Broadcom, Intel each have their own firmware-level rate control.

**Embedded relevance:** As a WLAN driver engineer, you may implement or tune rate adaptation. It directly impacts throughput and user experience. Aggressive algorithms may cause instability; conservative ones waste capacity.

### 2.11 MIMO & Beamforming — **[MISSING]**

**MIMO (Multiple Input Multiple Output):**
- Uses multiple antennas at both transmitter and receiver
- **Spatial Multiplexing:** Send different data streams on different antennas simultaneously → multiply throughput
- **Spatial Diversity:** Send the same data on multiple antennas → improve reliability
- **MU-MIMO:** AP transmits to multiple STAs simultaneously (Wi-Fi 5 DL only, Wi-Fi 6 adds UL)

**Beamforming:**
- The AP uses multiple antennas to focus the RF signal toward a specific STA (instead of radiating equally in all directions)
- **Explicit Beamforming:** STA sends channel sounding feedback (NDP — Null Data Packet protocol) so the AP can compute the steering matrix
- **Result:** Higher SNR at the target STA → higher MCS → better throughput

```
Beamforming sounding sequence:
  AP ──NDPA (NDP Announcement)──→ STA
  AP ──NDP (Null Data Packet)───→ STA
  STA measures channel and computes feedback matrix
  STA ──Compressed Beamforming Report──→ AP
  AP applies steering matrix to subsequent transmissions
```

### 2.12 OFDMA (Wi-Fi 6) — **[MISSING]**

**Definition:** Orthogonal Frequency Division Multiple Access. Divides a channel into smaller sub-channels called **Resource Units (RUs)** and assigns different RUs to different STAs simultaneously.

**Before OFDMA (Wi-Fi 5):** One STA uses the entire channel bandwidth at a time (even for a tiny ACK).

**With OFDMA (Wi-Fi 6):** AP can schedule multiple STAs in the same TXOP, each using a subset of subcarriers.

```
Wi-Fi 5 (one user per TXOP):
  |<──────── 80 MHz channel ──────────>|
  |            STA A data              |
  |            STA B data              | (must wait)

Wi-Fi 6 (OFDMA, multiple users per TXOP):
  |<──────── 80 MHz channel ──────────>|
  | STA A (26-tone RU) | STA B (52-tone RU) | STA C (106-tone RU) |
  // All three transmit simultaneously!
```

**Embedded relevance:** Dramatically improves efficiency in dense deployments (stadiums, offices). As a driver engineer, you handle RU allocation, trigger frames, and multi-user scheduling.

### 2.13 BSS Coloring (Wi-Fi 6) — **[MISSING]**

**Problem:** In traditional 802.11, if a STA detects ANY signal above the carrier sense threshold on a channel, it defers transmission (even if the signal is from a neighboring, unrelated BSS). This causes unnecessary delays in dense deployments.

**Solution:** BSS Coloring assigns a 6-bit "color" (0–63) to each BSS (transmitted in every frame's PHY header and Beacon). A STA can now distinguish:
- **Intra-BSS (same color):** Must defer (avoid collision with my own AP)
- **Inter-BSS (different color):** Can potentially ignore and transmit (uses higher CCA threshold, e.g., -82 dBm vs -62 dBm)

**Result:** Significant improvement in spatial reuse and throughput in dense environments.

### 2.14 MLO (Multi-Link Operation) — Wi-Fi 7 — **[MISSING]**

**Definition:** A single logical connection operates across **multiple radio links** (different bands/channels) simultaneously.

```
Traditional Wi-Fi:
  STA ←──── Link 1 (5 GHz, 80 MHz) ────→ AP
  // Single link at a time

Wi-Fi 7 MLO:
  STA ←──── Link 1 (5 GHz, 80 MHz) ────→ AP (MLD)
  STA ←──── Link 2 (6 GHz, 160 MHz) ───→ AP (MLD)
  STA ←──── Link 3 (2.4 GHz, 20 MHz) ──→ AP (MLD)
  // All links active simultaneously or load-balanced

MLD = Multi-Link Device (the logical entity spanning multiple links)
```

**Benefits:**
- **Aggregation:** Higher total throughput (combine bandwidth across links)
- **Low latency:** Send time-critical traffic on whichever link is free first
- **Seamless handover:** If one link has interference, shift to another without reassociation

**Embedded relevance:** MLO fundamentally changes driver architecture. Instead of one MAC/PHY pair, you manage multiple links with a shared upper MAC. Association is per-MLD, but data flows across links. This is cutting-edge WLAN work.

### 2.15 Roaming & Fast BSS Transition (802.11r) — **[MISSING]**

**Problem:** When a STA moves between APs in an ESS, the full association + 4-way handshake takes hundreds of milliseconds — unacceptable for VoIP (>50ms handoff causes audible disruption).

**802.11r (Fast BSS Transition / FT):**
- Pre-distributes PMK-R1 keys to target APs via the DS
- Reduces transition to just Reassociation Request/Response (skips full 4-way handshake)
- Two modes:
  - **Over-the-Air:** STA sends FT Auth directly to target AP (via wireless)
  - **Over-the-DS:** STA sends FT Auth through current AP via the wired backbone

```
Without 802.11r (~300ms):
  Scan → Auth → Assoc → 4-Way Handshake → Connected

With 802.11r (~50ms):
  Scan → FT Auth (includes key derivation) → Reassoc → Connected
```

**802.11k (Radio Resource Management):** AP provides neighbor reports so the STA knows which channels to scan, reducing scan time.

**802.11v (BSS Transition Management):** AP can suggest (or steer) a STA to a better AP.

**"kv/r" (802.11k/v/r) roaming:** The three amendments work together for seamless enterprise roaming.

### 2.16 Hidden Node Problem & RTS/CTS — **[MISSING]**

**Hidden node problem:** STA A and STA C can both hear the AP, but can't hear each other. They may transmit simultaneously, causing collision at the AP.

```
        STA A ←──range──→ AP ←──range──→ STA C
        (A can't hear C)                (C can't hear A)
```

**Solution: RTS/CTS (Request to Send / Clear to Send)**

```
1. STA A ──RTS──→ AP        (includes duration of upcoming transmission)
2. AP ──CTS──→ All STAs     (broadcast; includes duration)
3. STA C hears CTS → sets NAV (Network Allocation Vector) → defers for duration
4. STA A ──Data──→ AP       (collision-free)
5. AP ──ACK──→ STA A
```

**Tradeoff:** RTS/CTS adds overhead (two extra frames per exchange). It's typically enabled only for frames above a threshold size (RTS Threshold). For small frames, the overhead exceeds the benefit.

### 2.17 Block ACK (802.11e/n) — **[MISSING]**

**Problem:** In 802.11a/g, every data frame requires an individual ACK. With high throughput (802.11n+), this ACK overhead is massive.

**Solution:** Send a burst of frames (A-MPDU — Aggregate MPDU), then the receiver sends a single **Block ACK** bitmap indicating which individual frames were received.

```
Sender: [Frame 0][Frame 1][Frame 2][Frame 3][Frame 4] ──→ Receiver
Receiver: ←── Block ACK: [1][1][0][1][1]
                               ↑ Frame 2 not received
Sender: retransmits only Frame 2
```

**A-MPDU vs A-MSDU:**

| Aggregation | Level | Max Size | Error handling |
|-------------|-------|----------|----------------|
| A-MPDU (Aggregate MAC PDU) | MAC | 65535 bytes (11n), much larger in 11ac/ax | Each subframe has its own FCS → only failed subframes retransmitted |
| A-MSDU (Aggregate MAC SDU) | Above MAC | 3839 or 7935 bytes | Single FCS for entire aggregate → one error = retransmit all |

**Embedded relevance:** A-MPDU is the primary throughput booster in modern Wi-Fi. Your driver manages the BA (Block ACK) session setup/teardown and the reorder buffer.

### 2.18 DFS (Dynamic Frequency Selection) — **[MISSING]**

**Definition:** On certain 5 GHz channels, Wi-Fi must detect and avoid radar signals (used by weather and military systems). If radar is detected, the AP must vacate the channel within 10 seconds (Channel Switch Announcement → move all STAs).

**Embedded relevance:** DFS channels offer more spectrum (less congestion) but require the device to implement radar detection. Many consumer APs avoid DFS channels for simplicity. Enterprise APs use them actively.

**Interview question:** "Why can't you actively scan on DFS channels?" → Because sending Probe Requests on a DFS channel without first listening for radar could interfere with protected systems. Passive scanning (listening for beacons) is required first.

---

## 3. Layer 3: Network Layer (IP & Routing) 🟡

### 3.1 IPv4 Header (Key Fields)

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version|  IHL  |Type of Service|          Total Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Identification        |Flags|     Fragment Offset     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Time to Live |    Protocol   |        Header Checksum        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       Source Address                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Destination Address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Key fields for embedded:
- TTL: Decremented by every router. 0 → drop + ICMP Time Exceeded
- Protocol: 1=ICMP, 6=TCP, 17=UDP
- Don't Fragment (DF) flag: Set → router drops + ICMP "Frag Needed" if packet > MTU
- ToS/DSCP: QoS markings (mapped to WMM ACs at the WLAN layer)
```

### 3.2 Subnetting & CIDR

**Definition:** Subnetting divides an IP network into smaller segments. CIDR notation (`/24`) specifies how many bits are the network portion.

```
Example: 192.168.1.50/24
  Network:   192.168.1.0
  Broadcast: 192.168.1.255
  Host range: .1 to .254 (254 usable)
  Subnet mask: 255.255.255.0

Quick /prefix to hosts:
  /24 → 254 hosts    /25 → 126 hosts
  /26 → 62 hosts     /27 → 30 hosts
  /28 → 14 hosts     /30 → 2 hosts (point-to-point link)
```

**How a host decides if a destination is local or remote:**
```
if (dest_IP & my_subnet_mask) == (my_IP & my_subnet_mask):
    // Same subnet → ARP for dest_IP's MAC → send directly
else:
    // Different subnet → ARP for Default Gateway's MAC → send to router
```

### 3.3 ARP (Address Resolution Protocol)

**Definition:** Maps a known L3 address (IP) to an unknown L2 address (MAC) on the local subnet.

**Flow:**
```
1. Host A wants to send to 192.168.1.5 (on same subnet)
2. Host A checks ARP cache → not found
3. Host A broadcasts ARP Request: "Who has 192.168.1.5? Tell 192.168.1.1"
   (Dest MAC: FF:FF:FF:FF:FF:FF, EtherType: 0x0806)
4. All hosts receive it; only 192.168.1.5 responds
5. Host B (192.168.1.5) sends ARP Reply (unicast) with its MAC
6. Host A caches the mapping (typical timeout: 20–60 minutes)
```

**Gratuitous ARP:** A host ARPs for its own IP. Purposes:
- IP conflict detection (if someone replies, there's a duplicate)
- Update switches/ARP caches after NIC replacement or failover

**Proxy ARP:** A router answers ARP requests on behalf of hosts on another subnet. Common in WLAN when clients are on different subnets behind the same AP.

### 3.4 ICMP

| Type | Code | Name | Use |
|------|------|------|-----|
| 0 | 0 | Echo Reply | Ping response |
| 3 | 0 | Dest Unreachable: Network | Routing failure |
| 3 | 1 | Dest Unreachable: Host | Host down |
| 3 | 3 | Dest Unreachable: Port | No service listening (UDP) |
| 3 | 4 | Frag Needed, DF Set | Path MTU Discovery |
| 8 | 0 | Echo Request | Ping |
| 11 | 0 | Time Exceeded | TTL=0 (used by traceroute) |

**Path MTU Discovery (PMTUD):**
```
Host sends packet with DF (Don't Fragment) bit set.
If a router's outgoing link has a smaller MTU:
  Router drops packet → sends ICMP Type 3 Code 4 with the link's MTU
  Host reduces packet size and retries
  Repeat until the packet passes through all links
```

**Embedded gotcha:** Many firewalls block ICMP, breaking PMTUD. This causes "black hole" connections where TCP works for small packets but hangs for large transfers. Your embedded device should handle this gracefully (MSS clamping or fallback MTU).

### 3.5 IP Fragmentation — **[MISSING]**

**Definition:** When a packet exceeds a link's MTU, the router (or sender) splits it into fragments. Each fragment has the same Identification field, an offset, and a More Fragments (MF) flag.

```
Original packet: 4000 bytes (header=20, data=3980)
Link MTU: 1500 bytes

Fragment 1: [IP hdr | 1480 bytes data] Offset=0, MF=1
Fragment 2: [IP hdr | 1480 bytes data] Offset=185 (1480/8), MF=1
Fragment 3: [IP hdr | 1020 bytes data] Offset=370, MF=0
```

**Problems with fragmentation:**
- **Reassembly is at the destination only** (not at intermediate routers)
- If any fragment is lost, the entire packet is retransmitted
- Increases buffer pressure on the receiver
- Security risk (fragment overlap attacks)

**Embedded best practice:** Set DF bit and use PMTUD, or ensure MSS is set correctly to avoid fragmentation entirely. In wireless, fragmentation is especially costly because each fragment needs individual MAC-level retransmission.

### 3.6 Routing — **[MISSING]**

**Static routing:** Manually configured routes. Common in small embedded networks.
```bash
# Add a route: "To reach 10.0.0.0/24, go via gateway 192.168.1.1"
ip route add 10.0.0.0/24 via 192.168.1.1
```

**Dynamic routing protocols:**

| Protocol | Type | Algorithm | Convergence | Metric | Use |
|----------|------|-----------|-------------|--------|-----|
| RIP | Distance-vector | Bellman-Ford | Slow (30s updates) | Hop count (max 15) | Small networks, legacy |
| OSPF | Link-state | Dijkstra (SPF) | Fast | Cost (bandwidth-based) | Enterprise |
| BGP | Path-vector | Policy-based | Slow but stable | AS path + policies | Internet backbone |

**Distance-vector vs Link-state:**

| Aspect | Distance-Vector (RIP) | Link-State (OSPF) |
|--------|----------------------|-------------------|
| Knowledge | Only knows neighbors + costs | Full topology map |
| Updates | Periodic (broadcast full table) | Event-driven (link changes only) |
| Convergence | Slow (count-to-infinity problem) | Fast |
| Memory/CPU | Low | Higher (maintains full topology DB) |

**Embedded relevance:** You won't implement OSPF in a sensor, but you must understand routing tables, default gateway, and how your device's packets traverse the network to debug "can ping gateway but not internet" scenarios.

### 3.7 NAT (Network Address Translation)

**Definition:** Router replaces private source IP:port with its public IP:port. Maintains a translation table to map replies back.

```
Internal: 192.168.1.50:12345 → google.com:443
Router NAT: 203.0.113.1:54321 → google.com:443 (public IP)
Reply from Google → 203.0.113.1:54321 → Router → 192.168.1.50:12345
```

**Types:**
- **SNAT (Source NAT):** Outgoing traffic. Most common.
- **DNAT (Destination NAT) / Port Forwarding:** Allows incoming connections to reach internal devices.
- **PAT (Port Address Translation):** Many internal IPs share one public IP (different ports). This is what home routers do.

**WLAN relevance:** Captive portals on Wi-Fi hotspots use DNAT to redirect HTTP requests to a login page. Understanding NAT traversal is critical when your embedded device needs to accept incoming connections (e.g., P2P, remote management).

### 3.8 IPv6 Essentials

| Feature | IPv4 | IPv6 |
|---------|------|------|
| Address size | 32-bit | 128-bit |
| Header | Variable (options) | Fixed 40 bytes (extensions chained) |
| ARP | Yes | No → uses NDP (Neighbor Discovery Protocol over ICMPv6) |
| Auto-config | DHCP | SLAAC (Stateless Address Autoconfiguration) + optional DHCPv6 |
| Broadcast | Yes | No → uses multicast |
| Link-local | 169.254.x.x (only as fallback) | `fe80::/10` (always present, mandatory) |

**SLAAC:** Device generates its own global address using the router-advertised prefix + its own interface ID (derived from MAC via EUI-64, or randomized for privacy).

---

## 4. Layer 4: Transport Layer (TCP & UDP) 🔴

### 4.1 TCP vs UDP Summary

| Feature | TCP | UDP |
|---------|-----|-----|
| Connection | Connection-oriented (3-way handshake) | Connectionless |
| Reliability | Guaranteed delivery (ACKs, retransmissions) | Best-effort |
| Ordering | Preserves order (sequence numbers) | No ordering |
| Flow control | Sliding window | None |
| Congestion control | Slow start, AIMD, fast retransmit | None |
| Header size | 20 bytes (min) | 8 bytes |
| Use cases | HTTP, MQTT, OTA updates, SSH | DNS, DHCP, VoIP, mDNS, CoAP, video streaming |

### 4.2 TCP 3-Way Handshake

```
Client                    Server
  |                         |
  |─── SYN (seq=x) ───────→|  Client picks initial sequence number x
  |                         |  Server state: SYN-RECEIVED
  |←── SYN-ACK (seq=y, ──  |  Server picks ISN y, acknowledges x+1
  |    ack=x+1)             |
  |                         |
  |─── ACK (ack=y+1) ─────→|  Connection ESTABLISHED
  |                         |
```

**ISN (Initial Sequence Number):** Randomized for security (prevents TCP hijacking). Not starting from 0.

**Interview question:** "What is a SYN flood attack?" → Attacker sends thousands of SYN packets with spoofed source IPs. Server creates half-open connections (SYN-RECEIVED state), exhausting its connection table. Mitigation: **SYN cookies** — server encodes connection state in the ISN of the SYN-ACK, avoiding table allocation until the ACK is received.

### 4.3 TCP Connection Teardown & TIME_WAIT

```
Client                    Server
  |─── FIN ────────────────→|  Client initiates close (FIN_WAIT_1)
  |←── ACK ────────────────  |  Server acknowledges (CLOSE_WAIT)
  |                         |  Server may still send data...
  |←── FIN ────────────────  |  Server closes (LAST_ACK)
  |─── ACK ────────────────→|  Client enters TIME_WAIT
  |                         |
  |    [2 × MSL wait]       |  (MSL = Maximum Segment Lifetime, ~60s)
  |    Client: CLOSED       |
```

**Why TIME_WAIT exists (2×MSL = ~120 seconds):**
1. Ensure the final ACK reaches the server (if lost, server retransmits FIN, and the client can re-ACK)
2. Let old duplicate packets from this connection die before the port is reused

**Embedded problem:** On busy embedded servers handling many short connections, thousands of sockets in TIME_WAIT exhaust resources. Solutions:
- `SO_REUSEADDR`: Allow binding to a port in TIME_WAIT
- `tcp_tw_reuse` / `tcp_tw_recycle` (Linux sysctl): Reuse TIME_WAIT sockets for outgoing connections
- Use persistent connections to avoid frequent open/close

### 4.4 TCP Flow Control (Sliding Window)

**Definition:** The receiver advertises a **window size** in every ACK, telling the sender "I have X bytes of buffer space left; don't send more than that."

```
Receiver: "My window size is 4000 bytes"
Sender sends: [1000B][1000B][1000B][1000B] → window full, must wait
Receiver processes 2000B, ACKs with window=2000
Sender can now send 2000 more bytes
```

**Window scaling (RFC 1323):** The original window field is 16 bits (max 65535 bytes). For high-bandwidth links, this is too small. Window scaling allows windows up to 1 GB using a scale factor negotiated during the handshake.

**Zero window:** If the receiver's buffer is full, it advertises window=0. The sender stops and periodically sends **window probes** (1-byte segments) to check if the window has opened.

### 4.5 TCP Congestion Control

**Key variables:**
- **cwnd (Congestion Window):** Sender-side limit on unACKed bytes (network capacity estimate)
- **rwnd (Receiver Window):** Receiver-advertised buffer space
- **Sending rate:** min(cwnd, rwnd)
- **ssthresh:** Slow Start threshold — below this, use slow start; above, use congestion avoidance

**Phases:**

```
Phase 1: Slow Start (exponential growth)
  cwnd starts at 1 MSS
  Each ACK: cwnd += 1 MSS → doubles every RTT
  Continue until cwnd >= ssthresh OR packet loss

  cwnd: 1 → 2 → 4 → 8 → 16 (doubles each RTT)

Phase 2: Congestion Avoidance (linear growth / AIMD)
  Each RTT: cwnd += 1 MSS (Additive Increase)
  On packet loss: cwnd = cwnd / 2 (Multiplicative Decrease)

  cwnd: 16 → 17 → 18 → 19 → [LOSS] → 10 → 11 → 12 ...

Phase 3: Fast Retransmit + Fast Recovery
  On 3 duplicate ACKs (not timeout):
    - Retransmit the lost segment immediately (don't wait for RTO)
    - ssthresh = cwnd / 2
    - cwnd = ssthresh + 3 MSS (inflate for the 3 dupACKs)
    - Continue with congestion avoidance (don't restart from slow start)

  On RTO timeout (worse case):
    - ssthresh = cwnd / 2
    - cwnd = 1 MSS (back to slow start!)
```

> [!WARNING]
> **TCP over wireless gotcha:** Wireless packet loss is mostly due to RF interference, not congestion. But TCP's congestion control *interprets all loss as congestion*, cutting cwnd in half. This causes TCP to perform poorly over Wi-Fi if the link layer doesn't handle retransmissions aggressively.
>
> **This is the #1 "TCP behavior over wireless" interview question.** Know this cold.
>
> **Mitigations:**
> - MAC-layer retransmissions (802.11 ACKs + retry) hide most RF losses from TCP
> - Block ACK + A-MPDU aggregation amortize per-frame overhead
> - Some wireless stacks implement TCP ACK filtering to reduce uplink congestion

### 4.6 TCP Retransmission & RTO

**RTO (Retransmission Timeout):** Dynamically calculated from RTT measurements.

```
SRTT (Smoothed RTT) = (1 - α) × SRTT + α × measured_RTT    (α = 1/8)
RTTVAR = (1 - β) × RTTVAR + β × |SRTT - measured_RTT|       (β = 1/4)
RTO = SRTT + 4 × RTTVAR
```

**Exponential backoff:** If retransmission also times out: RTO doubles (1s → 2s → 4s → 8s ...). After ~15 retries, TCP gives up and resets the connection.

### 4.7 TCP State Machine — Complete

```
                    ┌──────────┐
             ┌─────│  CLOSED  │←──────────────────────────────┐
             │     └──────────┘                               │
       (passive open)  │  (active open, send SYN)             │
             │         ↓                                  (timeout)
        ┌────────┐  ┌──────────┐                              │
        │ LISTEN │  │ SYN_SENT │                              │
        └────────┘  └──────────┘                              │
       (rcv SYN,    (rcv SYN-ACK,                             │
       send SYN-ACK) send ACK)                                │
             ↓         ↓                                      │
        ┌──────────────────┐                                  │
        │   ESTABLISHED    │ ← data flows here                │
        └──────────────────┘                                  │
       (send FIN)    (rcv FIN, send ACK)                      │
             ↓              ↓                                 │
      ┌────────────┐  ┌────────────┐                          │
      │ FIN_WAIT_1 │  │ CLOSE_WAIT │                          │
      └────────────┘  └────────────┘                          │
      (rcv ACK)       (send FIN)                              │
             ↓              ↓                                 │
      ┌────────────┐  ┌────────────┐                          │
      │ FIN_WAIT_2 │  │  LAST_ACK  │──(rcv ACK)──────────────┘
      └────────────┘  └────────────┘
      (rcv FIN, send ACK)
             ↓
      ┌────────────┐
      │ TIME_WAIT  │──(2×MSL timeout)─→ CLOSED
      └────────────┘
```

### 4.8 Nagle's Algorithm — **[MISSING from theory, only mentioned as socket option]**

**Definition:** Buffers small outgoing TCP segments and sends them as one larger segment, reducing the number of tiny packets ("tinygrams") on the network.

**Rule:** "If there is unACKed data in flight AND the pending data is less than MSS, buffer it until the previous data is ACKed."

```
Without Nagle (TCP_NODELAY):
  send("H") → [H]  send("e") → [e]  send("l") → [l]  send("l") → [l]  send("o") → [o]
  5 packets for "Hello" — wasteful

With Nagle:
  send("H") → [H]  (first byte, sent immediately)
  send("e"), send("l"), send("l"), send("o") → buffered until ACK for "H" arrives
  ACK arrives → [ello]  — 2 packets total
```

**When to disable (TCP_NODELAY):**
- Real-time interactive applications (gaming, remote control)
- When the application already manages buffering
- Low-latency embedded control commands

> [!TIP]
> **Interview gotcha: Nagle + Delayed ACK interaction.** If the sender uses Nagle (waiting for ACK) and the receiver uses Delayed ACK (waiting ~200ms before sending ACK), you get a **200ms artificial delay** on every small write. This is a classic embedded networking performance bug. Fix: set `TCP_NODELAY` on the sender.

### 4.9 TCP Keepalive

**Problem:** A TCP connection can be idle forever. If one side crashes without sending FIN/RST, the other side has no way to know (half-open connection).

**Solution:** `SO_KEEPALIVE` sends periodic probes after idle timeout (default: 2 hours on Linux!).

```
After 7200s idle → send keepalive probe
  If ACK received → connection alive
  If RST received → connection broken → close
  If no response → retry (default: 9 probes, 75s apart)
  If all fail → connection dead → error to application
```

**Embedded tuning:** For IoT devices, 2 hours is too long. Tune via socket options:
```c
int idle = 60;      // Start probes after 60s idle
int interval = 10;  // 10s between probes
int count = 5;      // 5 failed probes → dead
setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));
```

---

## 5. Sockets & Network Programming 🟡

### 5.1 Socket Lifecycle

```c
// TCP Server:
int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // Create TCP socket
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  // Reuse port
bind(sockfd, &addr, sizeof(addr));              // Bind to IP:port
listen(sockfd, backlog);                        // Mark as passive, set queue size
int clientfd = accept(sockfd, &client_addr, &len);  // Block until client connects
recv(clientfd, buf, sizeof(buf), 0);            // Read data
send(clientfd, response, len, 0);               // Write data
close(clientfd);                                // Close client connection
close(sockfd);                                  // Close server socket

// TCP Client:
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
connect(sockfd, &server_addr, sizeof(server_addr));  // 3-way handshake happens here
send(sockfd, data, len, 0);
recv(sockfd, buf, sizeof(buf), 0);
close(sockfd);
```

### 5.2 Blocking vs Non-Blocking

**Blocking (default):** `recv()` sleeps until data arrives. `accept()` sleeps until a client connects. Simple but can't handle multiple connections in one thread.

**Non-blocking:**
```c
int flags = fcntl(sockfd, F_GETFL, 0);
fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

ssize_t n = recv(sockfd, buf, sizeof(buf), 0);
if (n == -1 && errno == EWOULDBLOCK) {
    // No data available right now — try again later
}
```

### 5.3 I/O Multiplexing: select / poll / epoll

| Mechanism | Max FDs | Performance | Kernel support | Embedded use |
|-----------|---------|-------------|----------------|-------------|
| `select()` | 1024 (FD_SETSIZE) | O(n) — scans all FDs | Everywhere (POSIX) | LwIP, FreeRTOS |
| `poll()` | Unlimited | O(n) — similar to select but no FD limit | POSIX | Linux embedded |
| `epoll()` | Unlimited | O(1) — event-driven (kernel notifies only active FDs) | Linux only | High-perf gateways |

```c
// epoll example (Linux):
int epfd = epoll_create1(0);
struct epoll_event ev = { .events = EPOLLIN, .data.fd = sockfd };
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

struct epoll_event events[MAX_EVENTS];
int n = epoll_wait(epfd, events, MAX_EVENTS, timeout_ms);
for (int i = 0; i < n; i++) {
    if (events[i].data.fd == sockfd) {
        // sockfd is ready for reading
    }
}
```

**Edge-triggered vs Level-triggered (epoll):**
- **Level-triggered (default):** `epoll_wait` returns as long as the FD is ready (like `select`)
- **Edge-triggered (`EPOLLET`):** `epoll_wait` returns only when the FD *transitions* to ready. You must read ALL available data, or you won't be notified again.

### 5.4 Netlink Sockets — **[MISSING]**

**Definition:** A Linux-specific socket family (`AF_NETLINK`) for communication between user-space and kernel. Used extensively in WLAN driver work.

**Why it matters for WLAN:**
- **nl80211/cfg80211:** The standard Linux kernel interface for configuring Wi-Fi devices. `iw`, `wpa_supplicant`, `hostapd` all use Netlink to communicate with the wireless driver.
- **Generic Netlink (GENL):** The modern framework used by nl80211. Supports multicast groups for event notifications.

```
User space: wpa_supplicant
    |
    | ← Netlink socket (AF_NETLINK, NETLINK_GENERIC)
    |
    ↓
Kernel: nl80211 → cfg80211 → mac80211 → your WLAN driver
```

**Common nl80211 commands:**
| Command | Purpose |
|---------|---------|
| `NL80211_CMD_TRIGGER_SCAN` | Initiate a scan |
| `NL80211_CMD_CONNECT` | Connect to a BSS |
| `NL80211_CMD_AUTHENTICATE` | 802.11 authentication |
| `NL80211_CMD_ASSOCIATE` | 802.11 association |
| `NL80211_CMD_NEW_SCAN_RESULTS` | Scan results available (event) |
| `NL80211_CMD_SET_CHANNEL` | Change channel |

**Interview question:** "How does `wpa_supplicant` talk to the kernel WLAN driver?" → Via Netlink sockets using the nl80211 generic netlink family. `wpa_supplicant` sends nl80211 commands (scan, connect, set keys) and receives events (scan results, disconnects, EAPOL frames).

### 5.5 Key Socket Options — **[MISSING details expanded]**

| Option | Level | Purpose | Embedded use |
|--------|-------|---------|-------------|
| `SO_REUSEADDR` | SOL_SOCKET | Bind to port in TIME_WAIT | Essential for embedded servers that restart |
| `SO_REUSEPORT` | SOL_SOCKET | Multiple sockets bind to same port (load balance) | Multi-threaded servers |
| `TCP_NODELAY` | IPPROTO_TCP | Disable Nagle's algorithm | Low-latency control |
| `SO_KEEPALIVE` | SOL_SOCKET | Detect dead peers | IoT device heartbeat |
| `SO_RCVBUF/SO_SNDBUF` | SOL_SOCKET | Set receive/send buffer sizes | Tune for embedded RAM constraints |
| `SO_BINDTODEVICE` | SOL_SOCKET | Bind socket to a specific interface (e.g., `wlan0`) | Multi-interface devices |
| `IP_MULTICAST_IF` | IPPROTO_IP | Set outgoing multicast interface | mDNS, SSDP |

---

## 6. Application Layer Protocols 🟢

> [!NOTE]
> Interviewers expect surface familiarity here. Know the message flows, when/why to use each, and embedded tradeoffs. Deep protocol internals are rarely asked.

### 6.1 DHCP (DORA Process)

**Definition:** Dynamically assigns IP addresses, subnet mask, default gateway, and DNS servers to hosts.

```
Client                                DHCP Server
  | ──DISCOVER (broadcast)────────────→ |  "Help, I need an IP!"
  | ←──OFFER (broadcast/unicast)─────── |  "How about 192.168.1.10?"
  | ──REQUEST (broadcast)─────────────→ |  "I'll take 192.168.1.10"
  | ←──ACK (unicast)──────────────────  |  "Confirmed. Lease=3600s, GW=.1, DNS=8.8.8.8"
```

**Why broadcast?** Client doesn't have an IP yet → can't do unicast. Uses UDP port 67 (server) / 68 (client).

**Lease renewal:** At 50% of lease time, client sends unicast REQUEST to renew. At 87.5%, sends broadcast REQUEST if server didn't respond.

**Embedded relevance:** DHCP is often the first thing that must work when bringing up a network on an embedded device. Debug tip: if DHCP fails, check that your device's MAC address is valid and that the DHCP server isn't filtering unknown MACs.

### 6.2 DNS

**Definition:** Translates human-readable domain names to IP addresses.

**Record types:**

| Type | Purpose | Example |
|------|---------|---------|
| A | IPv4 address | `api.example.com → 1.2.3.4` |
| AAAA | IPv6 address | `api.example.com → 2001:db8::1` |
| CNAME | Alias | `www.example.com → example.com` |
| MX | Mail server | `example.com → mail.example.com` |
| TXT | Arbitrary text | SPF records, verification tokens |
| SRV | Service location | Used by mDNS for service discovery |

**Resolution flow:**
```
Application → Stub Resolver → Local Cache
    → Recursive Resolver (ISP/8.8.8.8)
    → Root Server (.)  → TLD Server (.com)  → Authoritative Server (example.com)
```

**mDNS (Multicast DNS):** Used for zero-configuration networking on local networks. Queries go to multicast address `224.0.0.251` (port 5353). Devices respond with their own `.local` names. Critical for device discovery in IoT (e.g., `mydevice.local`).

### 6.3 HTTP Basics

**Request structure:**
```
GET /api/sensor HTTP/1.1\r\n
Host: 192.168.1.100\r\n
Content-Type: application/json\r\n
\r\n
```

**Status codes to know:**

| Code | Meaning | Common embedded scenario |
|------|---------|------------------------|
| 200 | OK | Successful API call |
| 301/302 | Redirect | Captive portal redirect on Wi-Fi |
| 400 | Bad Request | Malformed JSON from device |
| 401/403 | Unauthorized / Forbidden | Missing or invalid auth token |
| 404 | Not Found | Wrong API endpoint |
| 500 | Internal Server Error | Server-side crash |

**HTTP/1.1 vs HTTP/2:**

| Feature | HTTP/1.1 | HTTP/2 |
|---------|----------|--------|
| Format | Text-based | Binary |
| Multiplexing | No (one request/response per connection at a time) | Yes (multiple streams over one TCP connection) |
| Head-of-line blocking | Yes (at TCP level) | Reduced (at application level, still at TCP level) |
| Header compression | No | HPACK |
| Embedded suitability | Easy to parse, common | Complex parser, higher RAM |

### 6.4 MQTT

**Architecture:** Publish/Subscribe via a central broker. Clients don't communicate directly.

```
Sensor → PUBLISH("sensors/temp", "25.3", QoS 1) → Broker
                                                     ↓
Dashboard ← receives message ← SUBSCRIBE("sensors/#") ← Broker
```

**QoS levels:**

| QoS | Name | Flow | Duplicates? | Embedded use |
|-----|------|------|-------------|-------------|
| 0 | At most once | PUBLISH → (no ACK) | Possible loss | Non-critical telemetry |
| 1 | At least once | PUBLISH → PUBACK | Possible duplicates | Sensor data (idempotent) |
| 2 | Exactly once | PUBLISH → PUBREC → PUBREL → PUBCOMP | No | Financial, critical commands |

**Keep-Alive:** Client sends PINGREQ every N seconds. If broker doesn't hear anything in 1.5× keep-alive interval, it drops the client and publishes the **Last Will and Testament (LWT)** message.

### 6.5 CoAP — **[Expanded]**

**Like HTTP but for constrained devices:** RESTful (GET/POST/PUT/DELETE), runs over **UDP** (port 5683), binary header (4 bytes).

```
CoAP vs HTTP:
  HTTP:  TCP handshake (1.5 RTT) + TLS (1-2 RTT) + Request + Response
  CoAP:  UDP Request (CON) → Response (ACK) → Done in 1 RTT
```

**Observe:** Client registers for updates. Server pushes new values automatically when they change (no polling). Perfect for battery-powered sensors.

---

## 7. Network Security 🟡

### 7.1 WPA2 vs WPA3

| Feature | WPA2 | WPA3 |
|---------|------|------|
| Key exchange | PSK (pre-shared key via PBKDF2) | SAE (Simultaneous Authentication of Equals) |
| Offline dictionary attack | **Vulnerable** (captured handshake can be brute-forced offline) | **Resistant** (SAE is a zero-knowledge proof) |
| Forward secrecy | No (compromised PSK decrypts all past traffic) | **Yes** (each session has unique keys) |
| Encryption | AES-CCMP (128-bit) | AES-CCMP (128-bit) or AES-GCMP (256-bit) |
| Protected Management Frames | Optional (802.11w) | **Mandatory** |
| Open network encryption | No (open = plaintext) | **OWE** (Opportunistic Wireless Encryption) |

**SAE (Simultaneous Authentication of Equals) — WPA3-Personal:**
Based on the Dragonfly key exchange (a variant of PAKE — Password-Authenticated Key Exchange). Both sides prove knowledge of the password without revealing it, even to an eavesdropper.

```
STA ──SAE Commit (scalar + element)──→ AP
STA ←──SAE Commit (scalar + element)── AP
STA ──SAE Confirm (confirm value)───→ AP
STA ←──SAE Confirm (confirm value)── AP
// PMK is now established. Proceed with 4-way handshake.
```

### 7.2 802.1X / EAP (Enterprise Authentication) — **[MISSING]**

**Definition:** Port-based network access control. Used in WPA2/WPA3-Enterprise. Instead of a shared password (PSK), each user/device has individual credentials authenticated by a RADIUS server.

```
STA (Supplicant) ←─── EAP over 802.1X ───→ AP (Authenticator) ←── RADIUS ──→ Auth Server

1. STA associates with AP
2. AP blocks all traffic except EAP (802.1X controlled port is blocked)
3. STA and RADIUS server exchange EAP messages (relayed by AP):
   - EAP-TLS: Mutual certificate authentication (strongest)
   - PEAP: Server cert + username/password inside TLS tunnel
   - EAP-TTLS: Similar to PEAP
4. RADIUS sends Access-Accept + PMK to AP
5. AP and STA perform 4-way handshake using PMK
6. 802.1X controlled port opens — STA can send data
```

**Embedded relevance:** Enterprise Wi-Fi devices (printers, industrial sensors) must implement EAP-TLS with client certificates. This requires certificate management, which is complex on constrained devices.

### 7.3 TLS Handshake

**TLS 1.2 (simplified):**
```
Client                              Server
  |── ClientHello ─────────────────→ |  (supported ciphers, client random)
  |                                  |
  |←── ServerHello ──────────────── |  (chosen cipher, server random)
  |←── Certificate ─────────────── |  (server's X.509 cert)
  |←── ServerKeyExchange ────────  |  (DH params, signed)
  |←── ServerHelloDone ──────────  |
  |                                  |
  |── ClientKeyExchange ──────────→ |  (DH public key)
  |── ChangeCipherSpec ───────────→ |  (switching to encrypted)
  |── Finished (encrypted) ───────→ |
  |                                  |
  |←── ChangeCipherSpec ─────────  |
  |←── Finished (encrypted) ────  |
  |                                  |
  // All subsequent data encrypted with session key
```

**TLS 1.3 improvements:**
- 1-RTT handshake (vs 2-RTT for TLS 1.2)
- 0-RTT resumption (for repeat connections)
- Removed insecure ciphers (RSA key exchange, CBC, RC4, SHA-1)
- Only supports AEAD ciphers (AES-GCM, ChaCha20-Poly1305)
- Handshake itself is partially encrypted (after ServerHello)

**Embedded considerations:**
- Full TLS handshake takes 1–3 seconds on slow MCUs (RSA-2048 is expensive)
- Use **ECC** (ECDHE-ECDSA) instead of RSA — 10x faster for equivalent security
- Enable **session resumption/tickets** to skip handshake on reconnection
- mbedTLS: modular library; disable unused ciphersuites to save code size (~50KB Flash with minimal config)

### 7.4 Symmetric vs Asymmetric Encryption

| Aspect | Symmetric (AES) | Asymmetric (RSA/ECC) |
|--------|-----------------|---------------------|
| Keys | Same key for encrypt/decrypt | Public key + private key |
| Speed | Fast (hardware accelerated on most embedded SoCs) | Slow (100–1000x slower) |
| Key exchange | Need secure channel to share key | Public key can be shared openly |
| Use in TLS | Bulk data encryption (after handshake) | Key exchange and digital signatures (during handshake) |
| Typical sizes | AES-128, AES-256 | RSA-2048/4096, ECC P-256/P-384 |

### 7.5 802.11w (Protected Management Frames) — **[MISSING]**

**Problem:** Management frames (Deauth, Disassoc, Action) are sent unencrypted in WPA2. An attacker can forge deauthentication frames to kick users off the network.

**Solution:** 802.11w (MFP — Management Frame Protection) adds a MIC (using the IGTK — Integrity Group Temporal Key) to unicast and broadcast management frames. Forged frames are detected and dropped.

**WPA3 mandates 802.11w.** WPA2 makes it optional.

---

## 8. Performance & Troubleshooting 🟡

### 8.1 Latency vs Throughput

**Latency:** Time for a single packet to travel from source to destination (measured in ms). Critical for real-time applications (VoIP, control commands).

**Throughput:** Total data transferred per unit time (measured in Mbps). Critical for bulk transfers (OTA updates, video).

**They're independent:** A satellite link can have high throughput (100 Mbps) but high latency (600 ms). A serial link can have low latency (1 ms) but low throughput (9600 bps).

**Bandwidth-Delay Product (BDP):** The amount of data "in flight" at any instant = Bandwidth × RTT. For TCP, the window must be at least this large to fully utilize the link.
```
Example: 100 Mbps link, 50 ms RTT
BDP = 100 Mbps × 50 ms = 625 KB
TCP window must be ≥ 625 KB to fill the pipe
```

### 8.2 MTU and MSS

**MTU (Maximum Transmission Unit):** The largest L3 packet (IP header + payload) that can traverse a link. Standard Ethernet MTU = 1500 bytes.

**MSS (Maximum Segment Size):** The largest TCP payload per segment. MSS = MTU − IP header (20B) − TCP header (20B) = **1460 bytes**.

```
Ethernet frame:
[L2 hdr (14B)][IP hdr (20B)][TCP hdr (20B)][TCP payload (≤1460B)][FCS (4B)]
               |<─────────────── MTU = 1500 ──────────────────>|
```

**MSS is negotiated during the TCP handshake** (in the SYN packet's options). Each side advertises its MSS; the lower value is used.

**Embedded gotcha:** If your embedded device is behind a VPN or tunnel that adds overhead (e.g., GRE adds 24 bytes), the effective MTU drops. If you don't account for this, packets are fragmented or silently dropped (if DF bit is set and ICMP is blocked). Solution: reduce MSS via `iptables -j TCPMSS --clamp-mss-to-pmtu` or set MTU lower on the interface.

### 8.3 Nagle's Algorithm (Revisited)

See §4.8. Key performance scenario:

```
Nagle + Delayed ACK = 200ms artificial delay per write

Fix options:
1. TCP_NODELAY on sender (disable Nagle)
2. TCP_QUICKACK on receiver (disable Delayed ACK)
3. Application-level buffering (batch writes, then flush)
```

### 8.4 Packet Capture Mental Model (Wireshark/tcpdump)

**tcpdump (CLI, embedded-friendly):**
```bash
# Capture all traffic on wlan0:
tcpdump -i wlan0 -w capture.pcap

# Filter EAPOL frames (4-way handshake):
tcpdump -i wlan0 ether proto 0x888e

# Filter TCP traffic to/from port 1883 (MQTT):
tcpdump -i wlan0 tcp port 1883

# Show ARP traffic:
tcpdump -i wlan0 arp
```

**Wireshark filters (for analysis):**

| Filter | What it shows |
|--------|-------------|
| `wlan.fc.type_subtype == 0x08` | Beacon frames |
| `eapol` | 4-way handshake frames |
| `wlan.fc.type == 0` | All management frames |
| `tcp.analysis.retransmission` | TCP retransmissions |
| `tcp.analysis.zero_window` | Zero-window events |
| `dns` | DNS queries/responses |
| `ip.addr == 192.168.1.50` | All traffic to/from a device |

**How to debug a "Wi-Fi connection failing" with captures:**
```
1. Check: Are Probe Requests going out? → If not, scan isn't working
2. Check: Is Auth Response successful (status 0)? → If not, auth failure
3. Check: Is Assoc Response successful (status 0)? → If not, capability mismatch
4. Check: Do all 4 EAPOL messages complete? → If not, wrong password or KRACK
5. Check: Does DHCP DORA complete? → If not, no IP address assigned
6. Check: Are ARPs resolving? → If not, network isolation issue
```

### 8.5 Interrupt Coalescing & NAPI — **[Expanded]**

**Interrupt Coalescing:** Instead of one interrupt per received packet, the NIC waits for N packets or T microseconds, then fires one interrupt. Reduces CPU overhead at the cost of slight latency.

**NAPI (New API — Linux kernel):**
```
Normal mode: Interrupt per packet → ISR → process packet
  Problem at high rates: "Receive livelock" — CPU spends 100% in ISR, no time for apps

NAPI mode:
  1. First packet arrives → interrupt fires
  2. ISR disables further interrupts, schedules NAPI poll
  3. Kernel polls the device (reads up to budget packets per poll cycle)
  4. When no more packets, re-enables interrupts
  
  Switches between interrupt-driven (low load) and polling (high load) automatically
```

**Embedded relevance:** If you're writing an Ethernet or Wi-Fi driver for Linux, you implement the NAPI interface (`napi_poll`, `napi_schedule`, `napi_complete`). It's a standard interview topic for Linux network driver roles.

### 8.6 Checksum Offloading

**Definition:** Instead of the CPU computing TCP/IP/UDP checksums in software, the NIC hardware does it.

| Direction | What happens |
|-----------|-------------|
| **TX offload** | Driver fills in a dummy checksum; NIC computes and inserts the real one before transmitting |
| **RX offload** | NIC verifies the checksum and reports pass/fail to the driver; no CPU computation needed |

**Embedded relevance:** Essential for high-throughput on limited-CPU devices. Check your NIC/MAC datasheet for supported offloads. Wireshark may show "bad checksums" on captured TX packets because the checksum hasn't been filled in yet (it's offloaded to hardware).

### 8.7 Zero-Copy Networking — **[MISSING as a concept]**

**Definition:** Eliminating unnecessary memory copies in the packet path.

**Traditional path (lots of copies):**
```
NIC DMA → kernel buffer → copy to socket buffer → copy to user-space
(3 copies, 2 context switches)
```

**Zero-copy techniques:**
| Technique | Where | How |
|-----------|-------|-----|
| `sendfile()` | Linux kernel | Transfer data from file directly to socket, no user-space copy |
| `mmap()` | User-space | Map the socket buffer directly into user memory |
| LwIP PBUF_REF/ROM | RTOS | Pass pointers to data instead of copying |
| `AF_XDP` | Linux (advanced) | Kernel bypass for ultra-high performance |
| DMA scatter-gather | Hardware | NIC DMA writes directly to final buffer location |

### 8.8 Network Time: NTP vs PTP

| Feature | NTP | PTP (IEEE 1588) |
|---------|-----|-----------------|
| Accuracy | ~1–50 ms | ~1 µs (with HW timestamping) |
| Protocol | UDP port 123 | UDP ports 319/320 |
| HW support needed | No | Yes (PHY/MAC hardware timestamping) |
| Use cases | General logging, certificates | Industrial automation, 5G, automotive TSN, audio/video sync |

**PTP requires hardware timestamping:** The PHY/MAC records the exact time a PTP frame is transmitted/received at the wire, eliminating software jitter. Your driver must expose hardware timestamp capabilities to the PTP stack (`SO_TIMESTAMPING`, `SIOCSHWTSTAMP`).

### 8.9 Multicast & IGMP

**Multicast:** One-to-many transmission. Uses IP range 224.0.0.0 – 239.255.255.255.

**IGMP (Internet Group Management Protocol):** Host tells its local router: "I want to receive traffic for group 224.0.1.1."

| IGMP Version | Key Feature |
|--------------|------------|
| IGMPv1 | Join only (no leave — relies on timeout) |
| IGMPv2 | Adds explicit Leave + Group-Specific Query |
| IGMPv3 | Source-specific multicast (include/exclude lists) |

**Multicast MAC mapping:** The lower 23 bits of the multicast IP map to the Ethernet MAC `01:00:5E:xx:xx:xx`. This means multiple IPs can map to the same MAC (32 IPs per MAC — potential for unwanted traffic).

---

## Appendix: Gap Analysis — What Was Missing From Your Notes

The following topics were **absent** from your original notes and have been added:

| Topic | Section | Importance |
|-------|---------|------------|
| **802.11 frame format** | 2.3 | 🔴 Deep |
| **Scanning (active/passive)** | 2.4 | 🔴 Deep |
| **Association/Auth flow** | 2.5 | 🔴 Deep |
| **4-Way Handshake (EAPOL)** | 2.6 | 🔴 Deep |
| **Beacons, TIM/DTIM** | 2.7 | 🔴 Deep |
| **Power Save (PS-Poll, U-APSD, TWT)** | 2.8 | 🔴 Deep |
| **QoS / WMM (EDCA)** | 2.9 | 🔴 Deep |
| **Rate Adaptation / MCS** | 2.10 | 🔴 Deep |
| **MIMO & Beamforming** | 2.11 | 🟡 Solid |
| **OFDMA (Wi-Fi 6)** | 2.12 | 🟡 Solid |
| **BSS Coloring** | 2.13 | 🟡 Solid |
| **MLO (Wi-Fi 7)** | 2.14 | 🟡 Solid |
| **Roaming / 802.11r (FT)** | 2.15 | 🔴 Deep |
| **Hidden Node / RTS-CTS** | 2.16 | 🔴 Deep |
| **Block ACK / A-MPDU** | 2.17 | 🔴 Deep |
| **DFS (Dynamic Frequency Selection)** | 2.18 | 🟡 Solid |
| **CSMA/CD vs CSMA/CA** | 1.8 | 🟡 Solid |
| **Collision domain vs Broadcast domain** | 1.6 | 🟡 Solid |
| **IP Fragmentation** | 3.5 | 🟡 Solid |
| **Routing (static/dynamic, RIP/OSPF/BGP)** | 3.6 | 🟢 Surface |
| **TCP Keepalive** | 4.9 | 🟡 Solid |
| **Nagle's Algorithm (theory)** | 4.8 | 🟡 Solid |
| **Netlink sockets / nl80211** | 5.4 | 🔴 Deep |
| **802.1X / EAP** | 7.2 | 🟡 Solid |
| **802.11w (PMF)** | 7.5 | 🟡 Solid |
| **Zero-copy networking** | 8.7 | 🟡 Solid |
| **Bandwidth-Delay Product** | 8.1 | 🟡 Solid |

> [!CAUTION]
> **Critical gap:** Your original notes had almost **zero 802.11/WLAN L2 content** beyond basic terms (SSID, RSSI, STA/AP). For a WLAN-focused embedded role, Section 2 of this guide should be your primary study area. Interviewers will expect you to whiteboard the 4-way handshake, explain power save modes, describe A-MPDU aggregation, and discuss roaming mechanisms.

---

> [!TIP]
> **Study priority for limited time:**
> 1. 🔴 **WLAN deep dive (§2)** — This is your specialty claim. Must be flawless.
> 2. 🔴 **TCP behavior over wireless (§4.5)** — The intersection of layers 2 and 4. Classic interview topic.
> 3. 🟡 **Sockets + Netlink (§5)** — You'll write this code daily.
> 4. 🟡 **Security: 4-way handshake + WPA2/3 differences (§7)** — Always asked.
> 5. 🟢 **Application layer (§6)** — Know flows, don't memorize header fields.
