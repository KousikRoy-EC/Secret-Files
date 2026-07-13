# Part 7: OS, Linux, System Programming, and Device Drivers

## Q1: Process vs Thread Comparison

A **Process** is an independent execution unit with its own virtual address space, file descriptors, and resources allocated by the OS.
A **Thread** is the smallest unit of execution within a process. Multiple threads within the same process share the process's memory space, global variables, and open file descriptors, but each thread has its own stack and Program Counter (PC).

| Feature                | Process                                         | Thread                                     |
|------------------------|-------------------------------------------------|--------------------------------------------|
| **Address Space**      | Private/Isolated (Virtual memory space)         | Shared (Shares parent process's space)     |
| **Creation Cost**      | High (Requires copying descriptor tables)       | Low (Shares resources)                     |
| **Switching Overhead** | High (Requires swapping MMU Page Tables)        | Low (No Page Table swap, only SP/PC)       |
| **IPC**                | Requires OS mechanisms (Shared Memory, Sockets) | Direct access to global variables          |
| **Crash Safety**       | Crash in one process doesn't affect others      | Crash in one thread crashes entire process |

---

## Q2: Process Creation via `fork()`

`fork()` clones the calling process, creating an exact child duplicate. It returns:
- `0` inside the child process.
- The **Process ID (PID)** of the child inside the parent.
- `-1` if creation failed.

### Code Example
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        printf("Fork Failed\n");
    } else if (pid == 0) {
        // Child Process
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    } else {
        // Parent Process
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
    }
    return 0;
}
```

---

## Q3: Multithreading with `pthread`

### Code Example
This example creates two threads: one checks odd numbers, the other checks even numbers.

```c
#include <stdio.h>
#include <pthread.h>

void* print_even(void *arg) {
    printf("Even Thread Running:\n");
    for (int i = 0; i <= 10; i += 2) {
        printf("%d ", i);
    }
    printf("\n");
    return NULL;
}

void* print_odd(void *arg) {
    printf("Odd Thread Running:\n");
    for (int i = 1; i <= 10; i += 2) {
        printf("%d ", i);
    }
    printf("\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    
    // Create threads
    pthread_create(&thread1, NULL, print_even, NULL);
    pthread_create(&thread2, NULL, print_odd, NULL);
    
    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Threads Finished Execution.\n");
    return 0;
}
```

### Thread Cancellation and Detaching
- **`pthread_cancel`:** Sends a cancellation request to a thread. The thread terminated at execution cancellation points (like I/O system calls) unless disabled.
- **`pthread_detach`:** Marks a thread as detached. When a detached thread terminates, its resources are automatically released back to the system without requiring `pthread_join`.
  - **Trap:** If you call `pthread_join` on a detached thread, it will return an error (`EINVAL`).

---



## Q5: `fork()` vs `vfork()`

- **`fork()`:** Creates a child process with a separate address space. Modern OS uses Copy-on-Write (COW) optimization, copying page tables first and only copying memory pages when write occurs.
- **`vfork()`:** Creates a child without copying the address space. The child borrows the parent's address space directly. The parent process is suspended until the child calls `exec()` or `_exit()`.
  - **Trap:** Modifying variables in the child before `exec` directly modifies them in the parent, risking stack corruption.
- **`clone()`:** `fork()` and `vfork()` are system calls that are implemented on top of the `clone()` system call. `clone()` is a more general-purpose system call that allows the caller to specify which resources to share with the child process. `fork()` and `vfork()` are special cases of `clone()` where the caller specifies different sharing options.   
---

## Q6: Orphan vs Zombie Processes

- **Orphan Process:** A parent process terminates before its child process. The orphan child is adopted by the system initialization process (PID 1, `init` or `systemd`), which automatically reaps it on exit.
- **Zombie Process:** A child process terminates, but its parent has not yet read its exit status using `wait()` or `waitpid()`. The zombie consumes a slot in the system process table.
  - **Reaping Zombies:** Parent reaps zombies by trapping the `SIGCHLD` signal and calling `waitpid(-1, NULL, WNOHANG)`.

---

## Q7: Inter-Process Communication (IPC) Types

| IPC Type              | Max Data Capacity                    | Description                                                              |
|-----------------------|--------------------------------------|--------------------------------------------------------------------------|
| **Pipe (Anonymous)**  | 65,536 bytes (Linux default)         | Half-duplex communication between related parent/child processes.        |
| **FIFO (Named Pipe)** | 65,536 bytes (Linux default)         | Half-duplex, appears as a file, communicates unrelated processes.        |
| **Message Queue**     | Regulated by sysctl limit            | Structured messaging with priority control.                              |
| **Shared Memory**     | System RAM limit (GBs)               | Fastest IPC. Maps memory block directly to virtual space. Needs mutexes. |
| **Socket (Unix/TCP)** | Network buffer limited               | Communicates processes across network or locally.                        |
| **Signals**           | 1 bit (Just interrupt signal number) | Asynchronous notifications. Cannot send raw data payload safely.         |

---

## Q8: System Calls vs API

- **System Call:** An entry path into the kernel space to request hardware resources (e.g., `sys_write`, `sys_fork`). Invokes software interrupt (`svc` or `syscall` instruction).
- **API (Application Programming Interface):** A wrapper library function (e.g., standard library `printf` or `malloc`).
  - **Relationship:** `printf()` (API) internally processes formatting and calls `write()` (System Call) to output to console.

---

## Q9: Virtual Memory, MMU, Page Table, Page Fault

### Virtual Memory
Maps non-contiguous physical RAM to a contiguous virtual space, isolating processes.



```
  Process A Virtual Memory             Physical RAM (Page Frames)
  +----------------------+            +-----------------------+
  | Page 0 (0x0000)      |───────────►| Frame 4               |
  | Page 1 (0x1000)      |──┐         +-----------------------+
  +----------------------+  │         | Frame 1               |
                            │         +-----------------------+
  Process B Virtual Memory  └────────►| Frame 2               |
  +----------------------+            +-----------------------+
  | Page 0 (0x0000)      |───────────►| Frame 8               |
  +----------------------+            +-----------------------+
```

### Memory Management Unit (MMU)
A hardware unit in the CPU that translates virtual addresses to physical addresses at runtime using Page Tables.
- **Stack/Heap Protection:** MMU marks pages with permission flags (Read, Write, Execute). If a program attempts stack overflow or heap write outside allocated boundaries, the MMU triggers an access violation (**Segmentation Fault**).

### Page Table and Page Fault
- **Page Table:** A table mapping virtual page numbers to physical frames.
- **Page Fault:** Triggered when the CPU attempts to access a virtual page that has its valid bit set to `0` in the Page Table (not currently loaded in physical RAM). The CPU traps to the OS kernel, which loads the page from swap space into RAM, updates the Page Table, and restarts the instruction.

### Translation Lookaside Buffer (TLB)
A hardware cache inside the MMU that stores recent virtual-to-physical address translations, skipping Page Table lookup overhead.

---

## Q10: Cache Memory and Coherency

- **Cache Hit:** The CPU requests data that is present in the cache, avoiding slow RAM read cycles.
- **Cache Miss:** The requested data is missing in the cache, forcing the CPU to fetch from RAM and load it into cache.

### Cache Coherency Issues with DMA
Since DMA transfers data directly between peripherals and physical RAM without notifying the CPU cache:
- **Write-back problem:** CPU writes data to cache, but DMA reads old data from RAM.
- **Read-invalidate problem:** DMA writes new data to RAM, but CPU reads stale data from cache.

### Solution: Cache Invalidation & Cleaning
- **Cache Invalidation:** Clears the valid bit of cache lines, forcing the CPU to fetch new data from physical RAM (used before reading DMA data).
- **Cache Clean (Flush):** Forces cache lines to write their contents back to RAM (used before DMA reads RAM).
- **Flush Specific Line vs All:** Flush only specific lines containing the transfer buffer using memory barriers (`dmb`, `dsb`). Flushing the whole cache kills system performance.

---

## Q11: Linux Device Drivers

A device driver is a kernel module that abstracts physical hardware, presenting standard file interfaces (`open`, `read`, `write`, `close`) to user-space.

### Module Insertion Commands
- `insmod <module.ko>`: Directly inserts specified module.
- `modprobe <module>`: Inserts module while resolving and loading dependencies automatically.

### User to Kernel Data Copies
- **`copy_to_user(dest, src, size)`:** Copies kernel buffer to user space memory safely checking pointer validity.
- **`copy_from_user(dest, src, size)`:** Copies user space buffer to kernel memory safely preventing buffer overflow vulnerabilities.

---

## Q12: Device Tree Source (DTS)

A Device Tree is a tree-like data structure that describes the physical hardware layout (addresses, interrupts, GPIO lines, clocks) of a system.
- **DTS and Driver Relation:** During boot, the Linux kernel parses the Device Tree blob (`.dtb`). The kernel matches device drivers with hardware nodes described in the device tree by evaluating the `compatible` property string (e.g., `compatible = "vendor,device"`).
- **Probing Steps:** The matching driver's `.probe()` function is executed, passing hardware resources (register addresses, IRQ numbers) directly to the driver code.
```
  [ Device Tree Node ]              [ Device Driver ]
  compatible = "st,stm32-uart"      of_match_table = "st,stm32-uart"
           │                                ▲
           └──────────── Match ─────────────┘
                        │
                        ▼
                 Calls .probe()
```
