Here are 50 scenario-based interview questions across the full WLAN stack, organized by domain. For each one I'll give you the question exactly as an interviewer might ask it, plus the **key things your answer must hit** to pass.

---

## Category 1: Association & Connection Failures (Most Common)

**Q1.** A STA sends a Probe Request and gets a Probe Response, but never sends an Auth Request. What do you check?
> Must hit: SSID mismatch, security mismatch (RSNE), band/channel capability, driver scan result filtering in cfg80211, whether the supplicant got the BSS entry.

**Q2.** STA sends Auth Request, AP sends Auth Response with status 0 (success), but STA never sends Association Request. What's happening?
> Must hit: Supplicant state machine, RSNE parsing failure, PMF capability negotiation mismatch, AKM suite mismatch.

**Q3.** Association Request is sent, Association Response comes back with status code 17. What does that mean and what do you do?
> Must hit: Status 17 = AP has full association table. Check `max_num_sta` in hostapd, check driver/firmware STA table limits.

**Q4.** STA associates successfully but traffic doesn't flow. 4-way handshake completes but pings fail. Where do you look?
> Must hit: Key installation — did `drv_set_key` succeed? Check if GTK was installed. Check if the driver's crypto offload is correctly programmed. Check VLAN/bridge forwarding if in AP mode.

**Q5.** STA gets associated but is immediately deauthenticated with reason code 2. What happened?
> Must hit: Reason 2 = previous auth no longer valid. AP BSS restarted or STA entry got evicted. Check hostapd restart, check `max_num_sta`, check if AP went through a CSA mid-association.

**Q6.** WPA3-SAE association is failing at the Auth frame stage. Commit frame goes out, no response. What do you debug?
> Must hit: SAE group negotiation (group 19 vs 20), anti-clogging token required by AP, rate at which commit is being sent (SAE confirms are sensitive to timing), check if AP has SAE enabled in hostapd config.

**Q7.** STA re-associates successfully but has to redo the full 4-way handshake every time. 802.11r is configured. What's wrong?
> Must hit: MDIE mismatch, PMKSA cache not being used, FT AKM not selected in RSNE, check if FT over DS vs FT over air mismatch.

**Q8.** Dual-band STA only connects on 2.4 GHz even though 5 GHz AP is visible. What do you investigate?
> Must hit: Band steering logic in hostapd (reject assoc on 2.4, BSS TM request), check if STA's VHT/HT caps are being read correctly, check if 5 GHz requires PMF and STA doesn't support it.

---

## Category 2: Kernel Stack (cfg80211 / mac80211) Scenarios

**Q9.** You call `iw dev wlan0 scan` and it hangs. No results returned. Where in the kernel do you look?
> Must hit: cfg80211 scan state machine, is `scan_req` pending? Did the driver's `hw_scan` callback return? Did the driver call `ieee80211_scan_completed`? Check if a previous scan wasn't completed properly.

**Q10.** `iw dev wlan0 set channel 100` fails with EINVAL even though the channel is valid for your country. Why?
> Must hit: Regulatory domain — check if your country code is set, if the channel is marked with `IEEE80211_CHAN_DISABLED` or `IEEE80211_CHAN_RADAR` requiring CAC first, check DFS state.

**Q11.** You're seeing `cfg80211: Calling CRDA to update world regulatory domain` in dmesg repeatedly. What does that indicate and what's the fix?
> Must hit: CRDA is being invoked because the regulatory domain isn't settled. Check if `regdb` is installed, if udev rules are correct, if country code is being set by AP (country element) or manually.

**Q12.** mac80211 calls `drv_tx` but the frame never reaches the air. Driver returns success. What do you check in the driver?
> Must hit: DMA mapping — did `dma_map_single` succeed? Is the ring full (TCL ring full condition)? Did the HAL write the descriptor correctly? Check WBM completion — is there a TX error being reported silently?

**Q13.** You see `ieee80211_tx_status` being called with `WLAN_STATUS_TX_FILTERED`. What does that mean and when does it happen?
> Must hit: Frame was filtered — STA is in PS and the driver filtered it rather than buffering it, or multicast was filtered. mac80211 will retry or set TIM bit depending on what type of frame it was.

**Q14.** A kernel WARN fires: `WARN_ON(rcu_read_lock_held())` inside an `ieee80211_sta` access path. Explain what went wrong.
> Must hit: `ieee80211_find_sta` must be called under RCU read lock. If someone freed the STA or dereferenced outside the RCU critical section, this fires. Explain RCU rules for sta_info access.

**Q15.** `drv_ampdu_action` is being called with `IEEE80211_AMPDU_TX_START` but aggregation never actually starts. What's the flow and where can it break?
> Must hit: mac80211 initiates via `ieee80211_start_tx_ba_session`, driver returns 0, then driver must call `ieee80211_start_tx_ba_cb_irqsafe` when ready. If driver never calls the callback, mac80211 session stays in WAIT_ACT state forever.

**Q16.** You're running in AP mode. A STA goes to sleep (PS=1 in frame control). The AP keeps sending frames and the STA misses them. What should have happened and what's broken?
> Must hit: mac80211 should buffer frames when STA PS flag is set, set TIM bit in beacon, wait for PS-Poll or U-APSD trigger. If frames are going out anyway, check if `ieee80211_sta_ps_transition` is being called by the driver on PS state change, and if mac80211's PS filtering is enabled for that STA.

**Q17.** Channel switch (CSA) is announced in beacon but STAs are not switching. What does the full kernel-to-driver flow look like and where can it fail?
> Must hit: cfg80211 gets radar event → triggers CSA → mac80211 schedules channel switch → `drv_channel_switch` called → driver programs hardware → `ieee80211_chswitch_done` called → mac80211 completes switch. STAs need to receive the CSA element before count reaches zero.

---

## Category 3: Data Path & Performance

**Q18.** Throughput suddenly drops to near zero. `iw dev wlan0 station dump` shows the STA is still associated. What do you check?
> Must hit: TX queue stopped? Check `ieee80211_wake_queue` / `ieee80211_stop_queue`. Rate dropped to lowest MCS? Check rate control stats. A-MPDU sessions still active? WMM queue backlogged? Check ring status in debugfs.

**Q19.** You're seeing high TX latency spikes but average throughput is fine. What causes this in the mac80211 data path?
> Must hit: A-MPDU aggregation timeout/flush, rate control oscillation, PS buffering delay, WMM queue starvation on VO/VI if BK is flooding, TXOP limit being hit.

**Q20.** Multicast traffic is not being received by some STAs even though they're associated. What's the protocol-level reason and where in the kernel is it handled?
> Must hit: Multicast delivered after DTIM beacon. STAs must wake at DTIM. If STA is in PS and misses DTIM beacon, it misses the multicast. Also check if AP is converting multicast to unicast (`multicast_to_unicast` in hostapd). In the kernel, check how mac80211 handles `IEEE80211_TX_CTL_NO_PS_BUFFER` for multicast.

**Q21.** UDP throughput is high but TCP throughput is bad on the same link. Link quality looks fine. What's happening?
> Must hit: TCP ACK starvation, A-MPDU reorder buffer causing out-of-order delivery and triggering spurious TCP retransmits, RX reorder timeout, check `BA_SESSION_TIMEOUT`, also check if GRO is working correctly on the RX path.

**Q22.** NSS offload is enabled. You see a flow going through the exception path back to the kernel NAPI path instead of staying offloaded. Why would that happen?
> Must hit: Flow table miss (new flow, 5-tuple not yet established), fragmented packets, encrypted frames that NSS can't handle, QoS-sensitive frames marked for software handling, PPE exception conditions.

**Q23.** `skb_drop_reason` is being incremented for a particular reason in the RX path. How do you find which layer is dropping and why?
> Must hit: `drop_monitor`, `perf trace`, `kprobe` on `kfree_skb`, check `mac80211` RX handlers in order — decrypt failure, MIC failure, replay counter, reorder drop, duplicate detection.

**Q24.** You're doing iperf3 and see periodic throughput dips every ~100ms. What are the likely causes in the WLAN stack?
> Must hit: A-MPDU BA session flush timer, rate control probing interval, beacon TBTT causing TX pause, DTIM multicast delivery pause, background scan triggering off-channel time.

---

## Category 4: DFS and Regulatory

**Q25.** AP is on DFS channel 100. It suddenly stops beaconing and clients disconnect. No radar was detected by your driver. What do you investigate?
> Must hit: Check if another radio on the same platform triggered radar and the NOL was shared. Check if the regulatory database marked the channel differently. Check if `cfg80211_radar_event` was called spuriously. Check if CAC completed properly before AP started beaconing.

**Q26.** CAC completes successfully but AP still won't start beaconing on the DFS channel. What's the flow and what's broken?
> Must hit: After `cfg80211_cac_event(NL80211_RADAR_CAC_FINISHED)`, cfg80211 marks channel as available and notifies hostapd. Hostapd then starts the BSS. If hostapd doesn't receive the event, or if the channel is in NOL, it won't start. Check the event delivery path from driver → cfg80211 → nl80211 → hostapd.

**Q27.** You're testing on a US regulatory domain. Your driver supports channel 144 but the channel never shows up in scan results. Why?
> Must hit: Channel 144 is DFS in the US and requires CAC. It won't appear in active scan. Check regulatory rules — `IEEE80211_CHAN_RADAR` flag set. It may appear in passive scan results only if a beacon is heard.

**Q28.** Radar is detected mid-traffic. Walk me through exactly what happens from driver interrupt to client reconnecting on a new channel.
> Must hit: HW interrupt → `ath11k_radar_detected` → `ieee80211_radar_detected` → mac80211 calls `drv_abort_channel_switch` if ongoing, then triggers CSA → cfg80211 picks new channel → AP announces CSA → clients switch → AP goes through CAC on old channel and adds it back eventually.

---

## Category 5: Security and Authentication

**Q29.** EAPOL 4-way handshake fails at Message 3. STA sends EAPOL Message 2 but never gets Message 3. What do you check?
> Must hit: AP-side issue — GTK generation, RSNE in M3 must match M1, PMF bit negotiation, check if hostapd's EAPOL state machine is stuck, check if the STA entry was cleaned up between M1 and M3 (race condition).

**Q30.** PMF is required (`ieee80211w=2` in hostapd) but some STAs are being rejected. What frames are you looking at and what's the failure mode?
> Must hit: Association Response with status 31 (Robust management frame policy violation) — STA didn't set MFP required/capable bits in RSNE. Check STA's RSN capabilities field bit 6/7.

**Q31.** STA is sending deauth frames and kicking clients off the AP. Nobody sent those frames from the AP side. What's the attack and what's the defense?
> Must hit: Deauth attack (802.11 management frame injection). Defense is PMF — protected management frames make deauth/disassoc require MIC verification. Explain BIP (CMAC) for multicast management frames.

**Q32.** After a roam (802.11r FT), the STA can associate but can't pass traffic for ~2 seconds. What's happening?
> Must hit: PTK is derived during FT, but GTK installation may be delayed. Bridge/ARP table needs to be updated — AP should send EAPOL with GTK in FT reassociation response. Check if `ft_psk` vs `ft_eap` affects timing.

---

## Category 6: MLO and 802.11be

**Q33.** In an MLO setup, a STA associates on Link 0 but you only see traffic on Link 1. What determines which link traffic flows on?
> Must hit: T2LM (TID-to-Link Mapping) — the negotiated mapping determines which TIDs are allowed on which links. Default mapping allows all TIDs on all links, but T2LM negotiation can restrict this. Also check if one link is in power-save.

**Q34.** MLO association succeeds but the second link never comes up. Both links are visible in scan. What do you debug?
> Must hit: Per-link capability negotiation in Multi-Link Element — check if both links' capabilities are compatible. Check if the STA's `valid_links` bitmap in `ieee80211_vif` was updated by the driver. Check if firmware acknowledged the second link setup.

**Q35.** In EMLSR mode, you see the STA switching between links but with very high transition latency. What controls that latency?
> Must hit: EMLSR transition delay (in the EMLSR Parameter Update frame), transition delay and padding delay in the EMLSR Capabilities element. The STA has to silence one link before becoming active on another — this is bounded by the negotiated transition delay.

**Q36.** Preamble puncturing is configured for an 80 MHz channel with the second 20 MHz subchannel punctured. How is this communicated to the peer and what happens at the driver level?
> Must hit: EHT Operation element carries the puncturing bitmap. At the driver level, the hardware must be told which subchannels are excluded — this is typically a bitmask written to the PHY register via a WMI command (in ath12k context). The peer must also support puncturing (check EHT capabilities).

---

## Category 7: Debugging and Tooling

**Q37.** You suspect a frame is being dropped somewhere between mac80211 RX and the socket receive buffer. How do you trace it without modifying kernel code?
> Must hit: `kprobe`/`ftrace` on `kfree_skb`, `perf trace -e skb:kfree_skb`, `drop_monitor` netlink, `tc filter` with `skbedit` action to mark frames, check `/sys/kernel/debug/ieee80211/phy0/netdev:wlan0/stations/<mac>/rx_stats`.

**Q38.** A customer reports intermittent disconnects every few hours. Logs show reason code 4 (Disassociated due to inactivity). What do you check and how do you tune it?
> Must hit: AP's inactivity timer — in hostapd `ap_max_inactivity`, in mac80211 `IEEE80211_CONF_IDLE`, driver's keep-alive null data frame mechanism. Check if the STA is responding to null data probes. Could be a PS interaction where STA isn't waking to respond.

**Q39.** You see `tx_dropped` incrementing in `ifconfig` but no corresponding errors in driver debugfs. Where is the drop happening?
> Must hit: `tx_dropped` in net_device stats comes from `netif_tx_drop`. Could be queue stop — check `ieee80211_stop_queue` / `IEEE80211_TX_FILTERED`. Check if `ieee80211_wake_queue` is being called. Also check if `skb_headroom` is insufficient and driver is dropping for that reason.

**Q40.** `iw dev wlan0 station dump` shows `tx retries` very high for a particular STA. What does this mean at the driver/firmware level?
> Must hit: BA session — frames are being retransmitted because BA (Block Ack) is not being received, or individual MPDUs within the A-MPDU are being NACKed. Could be interference, link quality, or a stale BA session. Check if `DELBA` was received and session restarted.

**Q41.** You want to verify that A-MPDU aggregation is actually happening for a specific STA. What tools and what kernel paths do you check?
> Must hit: `/sys/kernel/debug/ieee80211/phy0/netdev:wlan0/stations/<mac>/agg_status` for session state. `iw dev wlan0 station dump` for `tx packets` vs `tx retries`. Wireshark — look for A-MPDU delimiter fields. In mac80211 code: `agg-tx.c`, session state machine.

**Q42.** A new driver feature causes random kernel panics under heavy traffic. You only have a crash dump. Walk me through your analysis approach.
> Must hit: `kdump`/`crash` tool, decode the call stack, identify the faulting instruction, check if it's a NULL pointer dereference (likely a race — RCU not held, or `ieee80211_sta` freed under us), check locks — did something run in softirq context while holding a spinlock taken in process context?

---

## Category 8: Power Save and Battery

**Q43.** A mobile STA reports poor battery life even when idle. It's connected to your AP. What do you check on the AP/driver side?
> Must hit: Is `IEEE80211_HW_SUPPORTS_PS` set? Is dynamic PS enabled? Check DTIM period (longer = fewer wakeups for multicast but higher latency). Check if AP is sending spurious null data frames keeping the STA awake. TWT — is it being negotiated for 802.11ax STAs?

**Q44.** STA is configured for U-APSD on VO and VI ACs. But it keeps polling unnecessarily even when there's no traffic. What does the protocol say should happen and what's likely misconfigured?
> Must hit: U-APSD service period should only be triggered by trigger frames from the STA. If the STA is polling repeatedly, check if `delivery_enabled` and `trigger_enabled` bits are set correctly per AC in the QoS Info element. AP should only deliver during service period.

---

## Category 9: Bring-up and Platform

**Q45.** A new ath11k-based board doesn't enumerate on AHB. The driver probes but firmware download fails at QMI. What's your debug sequence?
> Must hit: Check `dmesg` for QMI error codes. Is the firmware binary present at the expected path (`/lib/firmware/ath11k/<hw>/`)? Check memory region configuration — AHB firmware requires correct MSA (memory shared area) reservation in DTS. Check if remoteproc is bringing up the firmware correctly before ath11k takes over.

**Q46.** After firmware download, the ath11k driver gets stuck in `WMI_READY` wait. Timeout fires after 5 seconds. What do you check?
> Must hit: WMI READY event is sent by firmware after full initialization. If it's not arriving: check Copy Engine (CE) interrupt — is CE5 (WMI event path) receiving interrupts? Check MMIO mapping — is the CE register base correct? Check if firmware crashed silently (look for Q6 crash dump in remoteproc).

**Q47.** You have a dual-radio IPQ platform. Radio 0 works fine but Radio 1 never comes up. What's the likely issue at the platform level?
> Must hit: Separate firmware instances per radio on IPQ (especially IPQ5018 with 2.4G + 5G split). Check if the second firmware binary is present. Check DTS — separate memory regions per radio. Check if the second NSS instance is properly initialized. Check if `ath11k_pci` vs `ath11k_ahb` probes differently per radio.

---

## Category 10: Architecture and Design (Senior-Level)

**Q48.** Explain the exact difference between softmac and fullmac from a driver author's perspective. If you were porting a new chipset, how would you decide which model to use?
> Must hit: Softmac — mac80211 owns MLME, driver only does TX/RX and low-level HW programming. Fullmac — firmware owns MLME, driver is a thin shim (cfg80211 directly, no mac80211). Decision factors: does the firmware handle association/scanning internally? (fullmac) Or does the host need to do MLME? (softmac). ath11k is softmac-leaning but with WMI offload for some MLME.

**Q49.** Why does ath11k use shadow registers for SRNG HP/TP updates instead of direct MMIO writes? What problem does it solve and what's the tradeoff?
> Must hit: PCIe MMIO writes are expensive — they cross the PCIe bus and cannot be reordered or batched. Shadow registers are in DDR — the host writes HP/TP to DDR, firmware reads from DDR. This avoids PCIe bus latency on the critical TX submission path. Tradeoff: cache coherency — must ensure DDR writes are visible to the device (DMA coherency, memory barriers).

**Q50.** If you had to add support for a new mac80211 feature — say, per-STA TX power control — what are every touchpoint in the stack you'd need to modify, from nl80211 down to the driver?
> Must hit: Add nl80211 attribute → cfg80211 policy table → `cfg80211_ops` new callback or extend existing one → mac80211 plumbs it through to `ieee80211_ops` → driver implements `drv_` function → WMI command to firmware → firmware programs PHY TX power per-STA. Also need to handle it in `ieee80211_sta` struct if it's per-STA state, and ensure it survives station teardown/re-add.

---

## How to Use These

Don't just read these. For each one, **cover the "must hit" section** and try to answer out loud from scratch. If you can't hit 80% of the points without looking, that's a gap. For gaps, trace the actual kernel code for that scenario — reading the source makes it stick in a way that notes don't.

The questions in Categories 8–10 are the ones that separate strong mid-level candidates from senior-level ones. Get those solid and you'll stand out.