# Embedded Systems & C/C++ Technical Interview Preparation

This workspace contains a highly structured, comprehensive library of embedded systems, Linux kernel, RTOS, and C/C++ programming interview questions and answers.

---

## 📚 Master Index of Preparation Modules

| Module / File | Description | Key Topics Covered |
| :--- | :--- | :--- |
| **[Part 1: C Code Output Questions](file:///D:/Interview%20Material/Part1_C_Code_Output_Questions.md)** | Step-by-step trace of 19 common C code snippets. | Pointer arithmetic (`*p++`), post/pre-increment undefined behavior, dangling pointers, double pointer dereferencing, negative array indices, `sizeof` calculations, casting arrays, operator precedence. |
| **[Part 2: C Language Concepts](file:///D:/Interview%20Material/Part2_C_Language_Concepts.md)** | Core language qualifiers, keywords, and linkages. | `volatile` optimization details, `static` keyword scopes, `extern` declarations, storage classes, compiling phases, weak functions, macros vs inline functions, struct/union memory differences, `void*` pointer rules, callbacks. |
| **[Part 3: Memory & Bit Manipulation](file:///D:/Interview%20Material/Part3_Memory_And_Bit_Manipulation.md)** | Memory segments, allocators, endianness, and bitwise tricks. | Process memory segments, stack vs heap, malloc internal mechanics (brk/mmap/bins), struct padding and packing, endianness detection & conversion, setting/clearing/toggling bits, counting set bits, bit/nibble reversal, binary gap. |
| **[Part 4: Data Structures & Algorithms](file:///D:/Interview%20Material/Part4_Data_Structures_And_Algorithms.md)** | Pure C implementations of core data structures and operations. | String reverse, custom string operations from scratch (`strstr`/`strcmp`/etc.), singly/doubly linked list CRUD, list loop detection, middle element find, sorting algorithms comparison, matrix math, linear vs non-linear structures. |
| **[Part 5: Embedded Systems & Protocols](file:///D:/Interview%20Material/Part5_Embedded_Systems_Protocols.md)** | Communication protocols, hardware peripherals, and MCU startup. | I2C clock phases and start/stop timing, SPI CPOL/CPHA modes, UART frame format, CAN bit arbitration & errors, GPIO register configuration, software debouncing, ADC voltage divider calculator, DMA, MCU startup sequence. |
| **[Part 6: Interrupts & RTOS](file:///D:/Interview%20Material/Part6_Interrupts_And_RTOS.md)** | Interrupt service routines, priority resolution, and real-time kernels. | Interrupt execution lifecycle, IVT layouts, ISR rules and best practices, nested priority preemption, interrupt latency, RTOS vs OS, preemptive preemption flow, priority inversion & inheritance, spinlocks, deadlocks, bare-metal loops. |
| **[Part 7: OS, Linux & Drivers](file:///D:/Interview%20Material/Part7_OS_Linux_SystemProgramming.md)** | System programming, virtual memory protection, and kernel drivers. | Process vs thread, `fork()` vs `vfork()`, zombie processes, IPC capacity matrix, virtual memory MMU address translation, cache flushing & DMA coherency, module loading (`insmod`/`modprobe`), user-to-kernel copies, device trees (DTS). |
| **[Part 8: Networking, Sockets & C++](file:///D:/Interview%20Material/Part8_Networking_And_Sockets.md)** | Network stack layers, socket API, and C++ OOP concepts. | OSI 7-layers, IP vs MAC, TCP vs UDP, TCP 3-way handshake flow, TCP socket client-server implementation in C, symmetric vs asymmetric encryption, C++ virtual destructors, constructor call order, operator overloading, templated maps. |

---

## 🛠️ Compilation & Development Files
In addition to the markdown guides, this repository contains active code files for practice and compilation:
- **`ds_implementations.c`**: Reference implementation of Stack, Queue, Deque, Singly Linked List, and Binary Search Tree in C.
- **`_questions.cpp`**: C++ templates and basic coding algorithms.
- **`os_theory.c` / `cn_theory.c`**: Low-level implementation notes and scratch code files for OS/networking logic.
- **`qsched.txt`**: Case study breakdown of the custom task scheduler daemon (`qsched`) utilizing the fork-exec pattern, SIGCHLD signals, and POSIX mutexes.
