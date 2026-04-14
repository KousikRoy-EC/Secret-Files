
------>     Module 1: Fundamentals & Physical Layer     <------

1. OSI vs. TCP/IP Model (Embedded Context)
-------------------------------------------
While the OSI model (7 layers) is theoretical, the TCP/IP model (4/5 layers) is what actually runs on devices.

OSI Layer       TCP/IP Layer    Embedded Examples
7. Application  Application     MQTT, HTTP, CoAP, FTP
6. Presentation Application     TLS/SSL (mbedTLS), JSON, Protobuf
5. Session      Application     Socket creation, session management
4. Transport    Transport       TCP (Reliable), UDP (Fast/Low-overhead)
3. Network      Internet        IP (IPv4/v6), ICMP (Ping), ARP
2. Data Link    Network Access  Ethernet MAC, Wi-Fi (802.11), PPP
1. Physical     Network Access  Ethernet PHY, RS-485, Copper/Fiber

2. Ethernet Internals (MAC and PHY)
------------------------------------
In an embedded system (e.g., STM32, ESP32, i.MX), networking is split into two components:

- MAC (Media Access Control): Usually inside the Microcontroller (MCU). Handles data framing, CRC, and addressing.
- PHY (Physical Layer Transceiver): Usually an external chip (e.g., LAN8720, DP83848). Converts digital signals from the MAC into electrical signals for the cable.

Interfaces between MAC and PHY:
- MII (Media Independent Interface): Uses 16 pins for data/control.
- RMII (Reduced MII): Uses 9 pins, higher clock frequency (50MHz).
- RGMII (Reduced Gigabit MII): Used for 1000 Mbps speeds.

3. ARP (Address Resolution Protocol)
-------------------------------------
- Purpose: Maps a Known IP Address to an Unknown MAC Address.
- Flow: My device sends a "Broadcast" (FF:FF:FF:FF:FF:FF) asking "Who has 192.168.1.5?". The owner replies with its MAC.
- ARP Cache: Embedded devices save these mappings to avoid repeated broadcasts.


------>     Module 2: Transport & Network Layer     <------

1. TCP (Transmission Control Protocol)
---------------------------------------
- Characteristic: Connection-oriented, Reliable, Ordered, Flow-controlled.
- Three-Way Handshake:
  1. Client -> Server: SYN (Synchronize)
  2. Server -> Client: SYN-ACK
  3. Client -> Server: ACK (Acknowledge)
- Critical for: OTA updates, File transfers, Web dashboards.

2. UDP (User Datagram Protocol)
--------------------------------
- Characteristic: Connectionless, Unreliable (best-effort), No overhead.
- Why Embedded likes UDP: 
  - Real-time sensor data where losing one packet is okay.
  - Low memory footprint (no need for retransmission buffers).
  - Used in discovery protocols (mDNS, SSDP).

3. IPv4 vs IPv6
----------------
- IPv4: 32-bit (e.g., 192.168.1.1). Running out of addresses.
- IPv6: 128-bit (e.g., 2001:0db8:85a3...). Embedded devices use "Link-Local" addresses (fe80::) for local communication without a router.


------>     Module 3: Berkeley Sockets Programming     <------

Standard API used in Linux and RTOS (FreeRTOS + LwIP) for networking.

1. Server Lifecycle:
   socket() -> bind() -> listen() -> accept() -> read/write -> close()

2. Client Lifecycle:
   socket() -> connect() -> write/read -> close()

C Code Example (Simple UDP Sender):
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void send_sensor_data(int value) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create UDP socket
    struct sockaddr_in servaddr;
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.100");

    char buffer[50];
    sprintf(buffer, "Sensor: %d", value);
    
    sendto(sockfd, buffer, strlen(buffer), 0, 
           (struct sockaddr*)&servaddr, sizeof(servaddr));
    
    close(sockfd);
}
```

3. select() and non-blocking I/O:
Crucial for embedded servers. `select()` allows a single thread to monitor multiple sockets simultaneously without blocking.

------>     Module 4: Embedded Stacks & IoT Protocols     <------

1. LwIP (Lightweight IP)
-------------------------
The industry standard for networking in resource-constrained MCUs (like STM32).
- Key Feature: Zero-copy support. The stack can process data directly from the DMA buffer without copying it to another memory region.
- Operating Modes:
  - RAW API: Low-level callback-based, fastest, but hardest to program.
  - Netconn API: Sequential API, requires an RTOS.
  - Socket API: Standard BSD-style, easiest but slowest due to overhead.

2. DHCP & DNS
--------------
- DHCP (Dynamic Host Configuration Protocol): How a device automatically gets an IP, Subnet mask, and Gateway from a router.
- DNS (Domain Name System): Converts "google.com" to "142.250.190.46". Essential for OTA and cloud connections.

3. MQTT (Message Queuing Telemetry Transport)
----------------------------------------------
- Architecture: Publish/Subscribe (via a Central Broker).
- Why IoT LOVES MQTT:
  - Tiny overhead (2-byte header).
  - Keeps connections alive with "Keep-Alive" packets.
  - "Last Will and Testament": Broker notifies others if the device dies unexpectedly.
  - QoS Levels: 
    - 0: At most once (Fire and forget).
    - 1: At least once (Duplicate possible).
    - 2: Exactly once (Slowest, most reliable).

4. CoAP (Constrained Application Protocol)
-------------------------------------------
- Architecture: Request/Response (RESTful), similar to HTTP but over UDP.
- Use Case: Extremely low-power sensors that wake up, send data, and sleep immediately.

5. HTTP/REST
-------------
- Architecture: Client/Server.
- Usage: When your embedded device acts as a Web Server for configuration or as a Client to post data to a REST API.


------>     Module 5: Wireless & Security     <------

1. Wi-Fi (802.11)
------------------
- Modes: 
  - Station (STA): Connects to an Access Point (Router).
  - Access Point (AP): Acts as a router (e.g., for initial setup).

2. Bluetooth Low Energy (BLE)
------------------------------
- GAP (Generic Access Profile): Handles advertising and connection.
- GATT (Generic Attribute Profile): Handles data exchange via Characteristics and Services.
- Analogy: A Service is a "Folder" (e.g., Heart Rate Service) and a Characteristic is a "File" (e.g., Heart Rate Value).

3. LoRaWAN
-----------
- Purpose: Long range (kilometers), Ultra-low power, Very low data rate.
- Topology: Star-of-Stars (Devices -> Gateways -> Network Server).

4. Network Security (mbedTLS)
------------------------------
- TLS/SSL: Encrypts data at the Transport layer.
- Certificates: Embedded devices must store "CA Certificates" to verify the server's identity.
- Symmetric (AES): Fast, used for actual data.
- Asymmetric (RSA/ECC): Slow, used for initial key exchange.

------>     Network Debugging Tools     <------

1. Wireshark
-------------
The "Scanner" for networking. It captures packets from the wire and decodes them.
- Critical Skill: Filtering (e.g.,  to see MQTT traffic).

2. Ping & Traceroute
---------------------
- Ping: Checks if the target is alive (uses ICMP Echo).
- Traceroute: Shows every "Hop" (router) a packet takes to reach its destination.

3. Netstat & Iperf
-------------------
- Netstat: Shows active connections on your local machine/device.
- Iperf: Measures the maximum throughput (bandwidth) of the network link.


------>     Network Interview Questions for 2 YOE     <------

1. Explain the difference between a Hub, Switch, and Router.

Ans : The difference between a Hub, Switch, and Router is as follows : 

1. Hub (The "Dumb" Device)
Layer: Physical Layer (Layer 1)
How it works: A Hub is the simplest device. It connects multiple Ethernet devices together, but it has no intelligence. When it receives data on one port, it blindly broadcasts (repeats) that data to ALL other connected ports, regardless of the destination.
Collision Domain: A Hub is a single collision domain. If two devices try to send data at the same time, the signals collide, and both must wait and retry.
Security & Efficiency: Very poor. Since everyone receives everyone else's traffic, it creates security risks (sniffing) and network congestion.
Addressing: It does not understand MAC or IP addresses. It only deals with electrical signals (bits).
Example: Imagine a small office with 4 computers (A, B, C, D) connected to a Hub.

If Computer A wants to send a private file to Computer B, it sends the data to the Hub.
The Hub immediately copies that data and sends it to B, C, and D.
Computers C and D receive the packet but ignore it because it's not meant for them — however, the bandwidth was still wasted, and a hacker on C could easily intercept the message.

2. Switch (The "Intelligent" Device)
Layer: Data Link Layer (Layer 2)
How it works: A Switch is smarter than a hub. It keeps a MAC Address Table in its memory, which maps MAC addresses to specific physical ports. When a packet arrives, the switch looks at the Destination MAC Address and forwards the data ONLY to the port where that device is connected.
Collision Domain: Each port on a switch is its own separate collision domain. This allows full-duplex communication (sending and receiving at the same time) without collisions.
Broadcast Domain: A Switch is still keeping everyone on the same LAN, so it is a single broadcast domain. (If a device sends a broadcast frame like ARP, the switch will forward it to everyone).
Example: In the same office, replace the Hub with a Switch.

Computer A sends a file to Computer B.
The Switch looks at its table: "Computer B's MAC address is on Port 2."
The Switch sends the data only to Port 2.
Computers C and D hear nothing. The network is faster and more secure.

3. Router (The "Gateway")
Layer: Network Layer (Layer 3)
How it works: A Router is designed to connect different networks together (e.g., your Home LAN connects to the Internet WAN). It routes data based on IP Addresses. It uses a Routing Table to decide the best path for a packet to reach its destination.
Broadcast Domain: A Router breaks broadcast domains. Broadcasts sent on your LAN do not cross the router to the internet.
Crucial Function: It usually performs NAT (Network Address Translation), allowing multiple devices on your private LAN (192.168.x.x) to share a single Public IP address provided by your ISP.
Example: You are at home on your Laptop (connected to Wi-Fi) and you want to visit google.com.

Your Switch (often built into the router) handles the local traffic between your Laptop and the Router.
The Router sees the destination IP is 142.250.x.x (Google), which is not on your local network.
The Router sends the request out through the WAN port to your ISP, forwarding it across the internet to reach Google.
Without the router, your computer would have no way to leave your local home network.

2. What is a "Default Gateway" and why does an embedded device need it?

Ans : The default gateway is the IP address of the router that the device uses to communicate with devices on other networks. It is the first hop that the device takes when sending a packet to a destination that is not on the same network as the device.

3. How does TCP handle a packet that was sent but never acknowledged?

Ans : If a TCP packet is sent but never acknowledged, the sender will retransmit the packet after a certain timeout period. This timeout period is calculated based on the round-trip time (RTT) of the network connection. If the packet is still not acknowledged after several retransmissions, the connection will be terminated.

4. If your device can ping its local router but not google.com, what could be the issue? (Common answer: DNS or Gateway misconfiguration).

Ans : The issue could be either DNS or Gateway misconfiguration. 

5. Compare MQTT and HTTP for a battery-powered sensor device.

Ans : 

| Feature | MQTT | HTTP |
|---------|------|------|
| Model | Publish/Subscribe (Async) | Request/Response (Sync) |
| Overhead | Low (2-byte header) | High (Text-based headers) |
| Connection | Persistent | Usually per-request |
| Power | Low (Optimized for sleep) | High (Handshake overhead) |

MQTT is better for battery-powered devices because it minimizes the radio "on-time" by using smaller packets and avoiding the repeated overhead of establishing new TCP/TLS connections for every data point.

6. What is the MTU (Maximum Transmission Unit)? (Standard is 1500 bytes for Ethernet; fragments occur if exceeded).

Ans : The MTU is the largest size of a network layer packet (in bytes) that can be communicated in a single transaction over a network interface. For standard Ethernet, the MTU is 1500 bytes. If a packet exceeds the MTU of a link, it must be fragmented into smaller packets, which increases overhead and processing at both the router and the destination device.






================================================================================================
------>     Module 1: Physical & Link Layers (The Hardware Boundary)     <------
================================================================================================

In embedded systems, the boundary between hardware and software is most evident at the Link Layer.

1. Ethernet PHY, MAC, and Controller
-------------------------------------
- MAC (Media Access Control): A sublayer of the Data Link Layer. 
  - Roles: Framing, Addressing (MAC Address), Error Detection (CRC), Collision Handling (CSMA/CD).
  - Usually integrated into the Application Processor or MCU (e.g., i.MX6, STM32H7).
- PHY (Physical Layer): 
  - Roles: Line coding (Manchester, 4B5B), Signal conversion (Digital to Analog/Electrical), Link pulses (Auto-negotiation).
  - Usually an external chip connected via a systematic interface.
- Controller: The combination of MAC logic and the interface to the system bus (PCIe, AHB).

2. Interfaces: MII, RMII, RGMII
-------------------------------
These are the physical "pipes" connecting the MAC inside the MCU to the external PHY chip.

- MII (Media Independent Interface): 
  - Default for 10/100 Mbps.
  - Requires 16 signals (wires).
  - Clock frequency: 25 MHz (for 100 Mbps).
- RMII (Reduced Media Independent Interface):
  - Uses only 9 signals to save pins.
  - Clock frequency: 50 MHz (synchronized for both TX and RX).
- RGMII (Reduced Gigabit Media Independent Interface):
  - Used for 1000 Mbps (Gigabit).
  - Samples data on both edges of the clock (DDR - Double Data Rate).
- MDIO (Management Data Input/Output): 
  - A 2-wire serial interface (Clock + Data) used to configure the PHY registers (Speed, Duplex, Auto-neg status).

3. Auto-Negotiation & Flow Control
----------------------------------
- Auto-Negotiation: The process where two connected PHYs exchange "Base Page" abilities (10 vs 100 Mbps, Full vs Half Duplex) and agree on the highest common denominator.
- Duplex Mismatch: Occurs if one side is hardcoded to Full-Duplex and the other is Auto. Results in high packet loss and late collisions.
- Flow Control (IEEE 802.3x): Uses "PAUSE" frames. If a device's RX buffer is full, it sends a PAUSE frame to the sender to stop transmission for a specified time.

4. ARP (Address Resolution Protocol) Deep Dive
----------------------------------------------
- Problem: IP addresses are logical; hardware only understands MAC addresses.
- ARP Request: A Broadcast packet ("FF:FF:FF:FF:FF:FF") sent to all devices on the local subnet.
- ARP Reply: A Unicast packet sent back by the owner of the IP.
- ARP Cache: A table in RAM mapping IPs to MACs. 
  - Timeout: Entries are deleted after a few minutes to handle hardware changes.
- Gratuitous ARP: A device sends an ARP request for its own IP. 
  - Purpose: Detecting IP conflicts or notifying switches that a MAC has moved ports.
- Proxy ARP: A router answers ARP requests on behalf of a device on another network segment.

5. ICMP (Internet Control Message Protocol) Details
---------------------------------------------------
ICMP is used for diagnostics and error reporting. It sits "on top" of IP but is considered part of the Network Layer.

- Type 8 / Code 0: Echo Request (Ping).
- Type 0 / Code 0: Echo Reply.
- Type 3: Destination Unreachable. 
  - Code 4: Fragmentation Needed (Critical for Path MTU Discovery).
- Type 11: Time Exceeded (Used by Traceroute when TTL hits 0).

================================================================================================
------>     Module 2: Network Layer (IP & Routing)     <------
================================================================================================

1. IPv4 Header Structure
-------------------------
A standard header is 20 bytes. Key fields for embedded devs:
- Version: 4.
- IHL: Header length (usually 5, meaning 20 bytes).
- Total Length: Header + Data.
- Identification/Flags/Offset: Used for IP Fragmentation (avoided in high-perf systems).
- TTL (Time to Live): Decremented by every router. If it hits 0, the packet is dropped (prevents infinite loops).
- Protocol: 6 for TCP, 17 for UDP, 1 for ICMP.
- Header Checksum: Verified at every hop.

2. Subnetting & CIDR (Classless Inter-Domain Routing)
-----------------------------------------------------
- Subnet Mask: Defines which part of the IP is the Network ID and which is the Host ID.
- Example: 192.168.1.50 /24
  - Netmask: 255.255.255.0
  - Network Address: 192.168.1.0
  - Broadcast Address: 192.168.1.255
- Default Gateway: The IP of the router used to reach IPs OUTSIDE the local subnet.

3. DHCP DORA Process
---------------------
How an embedded device gets its IP address:
1. Discover: Client broadcasts "Help, I need an IP!".
2. Offer: Server unicasts/broadcasts "I have 192.168.1.10 for you."
3. Request: Client broadcasts "I'll take 192.168.1.10, thanks!".
4. Acknowledge: Server unicasts "Confirmed. Here are your DNS and Gateway details."

4. NAT (Network Address Translation)
------------------------------------
- Problem: Private IPs (10.x, 192.168.x) cannot route on the public internet.
- Solution: The router replaces the Private source IP/Port with its Public IP/Port and maintains a "Translation Table" to route the reply back.
- Why it matters: NAT can block incoming connections (e.g., trying to SSH into a device behind a home router).

5. IPv6 for Embedded
---------------------
- Format: 128-bit address, hexadecimal (e.g., fe80::1).
- No ARP: IPv6 uses Neighbor Discovery Protocol (NDP) over ICMPv6.
- SLAAC (Stateless Address Autoconfiguration): A device can generate its own IP without a DHCP server by using its MAC address (EUI-64) and the network prefix from the router.
- Link-Local Address (fe80::/10): Every IPv6 interface has one. It is only valid on the local segment (no routing).

6. DNS Internals
-----------------
- Record Types: 
  - A: IPv4 mapping.
  - AAAA: IPv6 mapping.
  - CNAME: Alias (pointing one domain to another).
- Resolution Flow: Device -> Local Cache -> Router (Resolver) -> Root Server -> TLD Server -> Authoritative Server.


================================================================================================
------>     Module 3: Transport Layer (TCP & UDP)     <------
================================================================================================

1. TCP State Machine (Crucial for Interviews)
----------------------------------------------
A single TCP connection transitions through several states:

- LISTEN: Waiting for a connection (Server side).
- SYN-SENT: Sent a SYN, waiting for SYN-ACK (Client side).
- SYN-RECEIVED: Received a SYN, sent SYN-ACK.
- ESTABLISHED: Handshake complete, data can flow.
- FIN-WAIT-1: Sent a FIN (initiating close).
- CLOSE-WAIT: Received a FIN, waiting for local app to close.
- TIME-WAIT: Wait for 2*MSL (Maximum Segment Lifetime) to ensure the remote side received the final ACK and to let wandering packets die. (Prevents reused ports from getting old data).

2. Reliability Mechanisms
-------------------------
- Sequence Numbers: Bytes sent.
- Acknowledgment Numbers: Next byte expected.
- RTO (Retransmission Timeout): If an ACK isn't received within RTO, the packet is resent. RTO is calculated dynamically based on RTT (Round Trip Time).
- Cumulative ACK: "I have received everything up to byte X."

3. TCP Flow & Congestion Control
--------------------------------
- Flow Control (Sliding Window): The Receiver sends a "Window Size" in the header to tell the Sender: "Don't send more than X bytes because my buffer is full."
- Congestion Control (Network Health):
  - Slow Start: Start with a small window, double it every RTT until a threshold (ssthresh).
  - Congestion Avoidance: Once past threshold, increase window linearly (add 1 per RTT).
  - Fast Retransmit: If 3 duplicate ACKs are received, assume a packet was lost and resend it immediately without waiting for RTO.
  - Fast Recovery: After fast retransmit, don't drop back to Slow Start; just reduce window by half.

4. UDP: Why and Where?
----------------------
- Header: Only 8 bytes (Source Port, Dest Port, Length, Checksum).
- Best-Effort: No ACKs, no retransmissions, no flow control.
- Application Layer Intelligence: If reliability is needed over UDP (e.g., CoAP or QUIC), it must be handled by the application code.

================================================================================================
------>     Module 4: Berkeley Sockets & Network Programming     <------
================================================================================================

1. The API Breakdown
---------------------
- socket(): Creates a file descriptor for communication.
- bind(): Assigns a local IP and Port to the socket.
- listen(): Tells the kernel to start queuing incoming connections.
- accept(): Pulls a connection off the queue (blocks by default).
- connect(): Connects to a remote server.
- recv() / send(): Data transfer (similar to read/write).

2. Socket Options (setsockopt)
-------------------------------
- SO_REUSEADDR: Allows the app to restart and bind to a port that is still in TIME_WAIT. Essential for quick reboots.
- TCP_NODELAY: Disables Nagle's Algorithm.
  - Nagle's Algorithm: Buffers small packets to send them as one big segment.
  - Disable it when you need low latency (e.g., sending real-time sensor updates).
- SO_KEEPALIVE: Sends periodic probes to check if the remote side is still alive.

3. Blocking vs. Non-blocking vs. Multiplexing
----------------------------------------------
- Blocking: One thread per connection. Inefficient for embedded if handling multiple clients.
- Non-blocking: Using fcntl(fd, F_SETFL, O_NONBLOCK). Calls return immediately with EWOULDBLOCK if no data.
- Multiplexing:
  - select(): Monitors up to 1024 descriptors. Slow because it must scan the whole bitmask every time.
  - poll(): Similar to select, but handles more descriptors and uses a list instead of a bitmask.
  - epoll() (Linux Specific): Highly efficient. The kernel notifies only when an event happens. Best for high-performance gateways/controllers.

================================================================================================
------>     Module 5: IoT & Application Protocols     <------
================================================================================================

1. MQTT (Message Queuing Telemetry Transport) Deep Dive
--------------------------------------------------------
- Topic Structure: sensors/livingroom/temperature.
- Wildcards: 
  - +: Single level (e.g., sensors/+/temperature).
  - #: Multi-level (e.g., sensors/#).
- MQTT Keep-Alive: Client sends a PINGREQ; Broker replies PINGRESP. If skipped twice, the broker drops the client.
- Retained Messages: The broker stores the last message on a topic and sends it immediately to any new subscriber.
- QoS 0: Fire and forget. No storage.
- QoS 1: Guaranteed delivery. Needs PUBACK. Can cause duplicates.
- QoS 2: Exactly once. Uses 4-step handshake (PUBLISH -> PUBREC -> PUBREL -> PUBCOMP).

2. CoAP (Constrained Application Protocol) Mechanics
-----------------------------------------------------
- Built over UDP.
- Binary Format: Much smaller than HTTP text headers.
- Observe: A client "subscribes" to a resource. The server pushes updates whenever the value changes (replaces polling).
- Confirmable (CON): Requires ACK.
- Non-confirmable (NON): Fire and forget.

3. HTTP/1.1 vs HTTP/2 in Embedded
----------------------------------
- HTTP/1.1: Text-based, persistent connections but suffers from "Head-of-Line Blocking" (one slow request slows down all).
- HTTP/2: Binary-based, Multiplexed (multiple requests over one TCP connection). Harder for tiny MCUs to parse due to HPACK compression.

================================================================================================
------>     Module 6: Embedded Stacks & Wireless     <------
================================================================================================

1. LwIP (Lightweight IP) Architecture
--------------------------------------
Most embedded network drivers (Ethernet, Wi-Fi) use LwIP.
- The PBUF (Packet Buffer):
  - PBUF_RAM: Allocated from memory pool for TX data.
  - PBUF_POOL: Consists of many fixed-size buffers for incoming RX packets (from DMA).
  - PBUF_ROM/REF: Used to point to constant data in Flash without copying.
- Zero-Copy RX: The driver gives LwIP a pointer to the DMA buffer directly. LwIP processes it and only frees it when finished.
- Zero-Copy TX: The application gives LwIP a pointer to the data. LwIP adds TCP/IP headers in a separate pbuf and chains it via a link-list to the payload.

2. BLE (Bluetooth Low Energy) Deep Dive
----------------------------------------
- Generic Access Profile (GAP): Handles Broadcasters (Beacons), Observers (Scanners), Centrals (Master), and Peripherals (Slave).
- Generic Attribute Profile (GATT):
  - Service: A group of characteristics (e.g., Battery Service).
  - Characteristic: The smallest unit of data (e.g., Battery Level 85%).
- Operations:
  - Read: Client asks for data.
  - Write: Client sends data.
  - Notify: Server pushes data to Client (no ACK needed).
  - Indicate: Server pushes data and waits for Client ACK (reliable).

3. Wi-Fi (802.11) Internals
----------------------------
- SSID: Service Set Identifier (Network Name).
- RSSI: Received Signal Strength Indication (measured in dBm, e.g., -50 is great, -90 is poor).
- PSK (Pre-Shared Key): The Wi-Fi password.
- WPA2/WPA3: Security protocols. WPA3 adds "Simultaneous Authentication of Equals" (SAE) to stop dictionary attacks.

================================================================================================
------>     Module 7: Network Security & TLS     <------
================================================================================================

1. TLS 1.2/1.3 Handshake
-------------------------
- Client Hello: Sends supported ciphers and a random number (Client Random).
- Server Hello: Picks a cipher, sends Server Random + Digital Certificate.
- Certificate Verification: Client checks the certificate against its Root CAs.
- Key Exchange (Diffie-Hellman): Securely agree on a "Pre-Master Secret" without sending it over the wire.
- Session Keys Derived: Both sides calculate the same Symmetric key (AES) for data encryption.

2. mbedTLS in Embedded
-----------------------
- Why mbedTLS?: Tiny footprint, modular (you can disable what you don't use), and portable.
- Alt Hardware Acceleration: Many chips (like ESP32 or STM32) have hardware blocks for AES and SHA. mbedTLS can be "hooked" to use these instead of software routines to save CPU.

3. SSL/TLS Contexts
--------------------
- Handshake overhead: Can take 1-2 seconds on slow MCUs.
- Session Resumption: Reuses a previous session's key to skip the heavy handshake.

================================================================================================
------>     Final Review: Advanced Networking Interview Q&A     <------
================================================================================================

Q1: What happens if two devices on a network have the same MAC address?
A: ARP tables will flap. Traffic will reach one device or the other unpredictably (MAC Flooding). Switches will keep updating their MAC-to-Port maps.

Q2: Explain the significance of the TTL field in IP.
A: It prevents packets from looping infinitely. Every router decrements it. If it hits 0, an ICMP "Time Exceeded" is sent back.

Q3: Difference between "Link-local" and "Global" IPv6 address?
A: Link-local (fe80::) is only for one segment. Global is routable on the internet.

Q4: How does TCP handle Congestion?
A: By adjusting its Congestion Window (cwnd) using Slow Start, Congestion Avoidance, and Fast Recovery.

Q5: Why is UDP used for DHCP?
A: Because a client doesn't have an IP address yet. It needs to broadcast, and TCP cannot handle broadcasts as it requires a point-to-point handshake.

Q6: What is the "Maximum Segment Size" (MSS) in TCP?
A: It's the maximum amount of DATA a TCP segment can carry. MSS = MTU - (IP Header + TCP Header).

Q7: What is a "Half-Open" TCP connection?
A: When one side has crashed or disconnected without a proper FIN/RST, but the other side still thinks the connection is active. Handled by TCP Keep-Alives.

================================================================================================
------>     Module 8: Performance & Elite Networking Topics     <------
================================================================================================

These topics separate a senior embedded engineer from a junior one in networking roles.

1. Multicast & IGMP
--------------------
- Multicast (One-to-Many): Sending a single packet that is received by multiple interested hosts. Uses a specific IP range (224.0.0.0 to 239.255.255.255).
- IGMP (Internet Group Management Protocol): The protocol used by a host to tell its local router: "I want to join/leave this multicast group."
- Specialized MAC: Multicast IP 224.0.0.1 maps to a specialized Ethernet MAC (01:00:5E:00:00:01).
- Use Case: discovery (mDNS), IPTV, and industrial automation.

2. VLANs (802.1Q)
------------------
- Virtual LAN: Logically separating a physical switch into multiple broadcast domains.
- Tagging: A 4-byte header added to the Ethernet frame containing the "VLAN ID".
- Trunk Port: A port that carries traffic for multiple VLANs (tagged).
- Access Port: A port that carries traffic for only one VLAN (untagged).

3. Network Time Synchronization (NTP & PTP)
-------------------------------------------
- NTP (Network Time Protocol): Accuracy in milliseconds. Good for general logging.
- PTP (Precision Time Protocol - IEEE 1588): Accuracy in microseconds/nanoseconds. 
  - Why it's embedded-specific: Requires hardware support in the PHY/MAC to "timestamp" packets at the exact moment they hit the wire to eliminate jitter.
  - Critical for: Factory automation, 5G base stations, and Automotive (TSN).

4. Network Performance Tuning
------------------------------
- Interrupt Coalescing: Instead of the PHY triggering an interrupt for every single packet, it waits for a few packets or a small timeout to reduce CPU context switching.
- NAPI (New API - Linux): Switches from interrupt-driven to polling-driven mode under high load to prevent "Receive Livelock" (a state where the CPU only processes interrupts and has no time for the actual data).
- Zero-Copy: (As mentioned in LwIP) Avoiding expensive `memcpy()` between layers.
- Checksum Offloading: Telling the hardware (MAC) to calculate the TCP/IP checksums instead of doing it in software.

5. Layer 2 vs Layer 3 Switching
-------------------------------
- L2 Switch: Forwards based on MAC address (Hardware speed, same subnet).
- L3 Switch/Router: Forwards based on IP address (Routes between different subnets).
