# Part 8: Networking, Sockets, and C++ Concepts

---

## Q1: The OSI Model and Its 7 Layers

The OSI (Open Systems Interconnection) model is a conceptual framework that standardizes the network communication functions into seven distinct logical layers.

```
       OSI Reference Model
  +───────────────────────────+
  |  7. Application Layer     |  <-- HTTP, FTP, DNS (User interfaces)
  +───────────────────────────+
  |  6. Presentation Layer    |  <-- SSL/TLS, ASCII, JPEG (Data formatting/encryption)
  +───────────────────────────+
  |  5. Session Layer         |  <-- NetBIOS, RPC (Establish/maintain connections)
  +───────────────────────────+
  |  4. Transport Layer       |  <-- TCP, UDP (End-to-end transport, port numbers)
  +───────────────────────────+
  |  3. Network Layer         |  <-- IP, ICMP (Routing, logical addresses)
  +───────────────────────────+
  |  2. Data Link Layer       |  <-- Ethernet, PPP (MAC addresses, framing)
  +───────────────────────────+
  |  1. Physical Layer        |  <-- RS232, RJ45 (Raw bits transmission)
  +───────────────────────────+
```

---

## Q2: IP Address vs MAC Address

| Feature    | IP Address (Internet Protocol)            | MAC Address (Media Access Control)          |
|------------|-------------------------------------------|---------------------------------------------|
| **Layer**  | Network Layer (Layer 3)                   | Data Link Layer (Layer 2)                   |
| **Type**   | Logical Address (Assigned by DHCP/Router) | Physical Address (Burned into NIC hardware) |
| **Format** | 32-bit (IPv4) or 128-bit (IPv6)           | 48-bit hex (e.g., `00:0a:95:9d:68:16`)      |
| **Scope**  | Global (Used for routing across networks) | Local (Used within local subnet)            |

---

## Q3: TCP vs UDP

| Feature          | TCP (Transmission Control Protocol)            | UDP (User Datagram Protocol)                      |
|------------------|------------------------------------------------|---------------------------------------------------|
| **Connection**   | Connection-oriented (Handshake required)       | Connectionless (Just fire-and-forget)             |
| **Reliability**  | Guaranteed delivery (Retransmits lost packets) | Not guaranteed (Packets can be lost/out-of-order) |
| **Speed**        | Slower (overhead of acknowledgments)           | Faster (minimal header size)                      |
| **Flow Control** | Yes (sliding window)                           | No                                                |
| **Header Size**  | Minimum 20 bytes                               | Fixed 8 bytes                                     |

---

## Q4: TCP Handshake and Connection Flow

### 3-Way Handshake (Establish Connection)
```
  Client                                 Server
    │                                      │
    │─────── SYN (Seq=X) ─────────────────►│ (Listen State)
    │                                      │
    │◄────── SYN-ACK (Seq=Y, Ack=X+1) ─────│ (SYN Received)
    │                                      │
    │─────── ACK (Ack=Y+1) ───────────────►│ (Established)
```

### 4-Way Termination (Close Connection)
```
  Client                                 Server
    │                                      │
    │─────── FIN ─────────────────────────►│
    │◄────── ACK ──────────────────────────│ (Close Wait)
    │                                      │ [Data flush]
    │◄────── FIN ──────────────────────────│
    │─────── ACK ─────────────────────────►│
```

---

## Q5: Socket Programming (TCP Client-Server C Code)

### TCP Server Code
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // 1. Create Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Bind Socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 4. Accept
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 5. Read and Send
    read(new_socket, buffer, 1024);
    printf("Received: %s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    
    close(new_socket);
    close(server_fd);
    return 0;
}
```

### TCP Client Code
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    // 1. Create Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address\n");
        return -1;
    }

    // 2. Connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // 3. Send and Read
    send(sock, hello, strlen(hello), 0);
    read(sock, buffer, 1024);
    printf("Server responded: %s\n", buffer);

    close(sock);
    return 0;
}
```

---

## Q6: Encryption and Security Libraries

- **Symmetric Encryption:** Same key is used for encryption and decryption. Very fast. (e.g., **AES**, DES).
- **Asymmetric Encryption:** Uses a public key for encryption and a private key for decryption. Computationally heavy. (e.g., **RSA**, ECC).
- **Hashing:** One-way cryptographic hash functions (e.g., **SHA-256**). Used for integrity verification.
- **Embedded Security Libraries:**
  - **mbedTLS:** Light footprint library optimized for resource-constrained microcontrollers.
  - **wolfSSL:** Extremely fast, target-oriented SSL/TLS library.

---

## Q7: TLS (Transport Layer Security) Handshake

TLS encrypts TCP connections. The handshake occurs after the TCP 3-way handshake:

```
  Client                                 Server
    │                                      │
    │─────── ClientHello (Supported Cipher)──►│
    │◄────── ServerHello & Certificate ─────│
    │◄────── ServerKeyExchange ────────────│
    │                                      │
    │─────── ClientKeyExchange ───────────►│ (Pre-master secret encrypted using RSA/DH)
    │─────── ChangeCipherSpec ────────────►│ (Switch to symmetric encryption keys)
    │◄────── Finished ─────────────────────│
```

---

## Q8: C++ Concepts in Embedded Interviews

### 1. Virtual Destructor
If a derived class object is deleted using a base class pointer, and the base class destructor is **not virtual**, the base class destructor executes but the derived class destructor does not. This causes resource leaks.

### 2. Constructor/Destructor Call Order
- **Constructors:** Called top-down: `Base` -> `Derived` -> `Grandchild`.
- **Destructors:** Called bottom-up: `Grandchild` -> `Derived` -> `Base`.

### 3. Why 1 Byte = 8 bits?
Historically, computers used character sizes ranging from 4 to 12 bits. In 1964, IBM standardized on an 8-bit byte for System/360, accommodating the EBCDIC character set. 8 bits is also a power of 2 ($2^3$), making binary memory addressing hardware architectures simpler.
