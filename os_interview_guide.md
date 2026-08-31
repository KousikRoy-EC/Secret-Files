# Embedded OS Interview Master Reference

> [!NOTE]
> **Depth Legend** used throughout this document:
> - 🔴 **Deep Dive** — Expect multi-level follow-ups in kernel/driver interviews. You must know internals.
> - 🟡 **Solid Understanding** — Know the concepts, tradeoffs, and one code example.
> - 🟢 **Surface Familiarity** — Know definitions and when/why to use. Rarely grilled beyond basics.
>
> **[MISSING]** tags mark topics that were **not present** in your original notes and have been added.

---

## Table of Contents

1. [Process & Thread Fundamentals](#1-process--thread-fundamentals-)
2. [CPU Scheduling](#2-cpu-scheduling-)
3. [Process Synchronization & Concurrency](#3-process-synchronization--concurrency-)
4. [Memory Management](#4-memory-management-)
5. [Interrupts & Exceptions](#5-interrupts--exceptions-)
6. [IPC Mechanisms](#6-ipc-mechanisms-)
7. [Signals](#7-signals-)
8. [RTOS-Specific Concepts](#8-rtos-specific-concepts-)
9. [Boot Process](#9-boot-process-)
10. [Linux Kernel Concurrency Primitives](#10-linux-kernel-concurrency-primitives-)
11. [Linux Device Driver Architecture](#11-linux-device-driver-architecture-)
12. [File Systems](#12-file-systems-)
13. [Embedded-Specific Topics](#13-embedded-specific-topics-)

---

## 1. Process & Thread Fundamentals 🔴

### 1.1 What is a Process?

**Definition:** A process is an instance of a program in execution. It has its own virtual address space (code, data, heap, stack), file descriptor table, signal handlers, and a unique PID. The kernel tracks each process via a `task_struct`.

**Why it matters for embedded:** In embedded Linux, understanding process isolation is critical for designing multi-process architectures (e.g., separate watchdog process, separate sensor-reading process) that are fault-tolerant — one crashing process doesn't bring down the system.

### 1.2 Process States

| State | Kernel Representation | Meaning |
|-------|----------------------|---------|
| Running | `TASK_RUNNING` | Currently executing on CPU or in run queue |
| Interruptible Sleep | `TASK_INTERRUPTIBLE` | Waiting for event; can be woken by signal |
| Uninterruptible Sleep | `TASK_UNINTERRUPTIBLE` | Waiting for I/O; **cannot** be killed (the dreaded `D` state) |
| Stopped | `TASK_STOPPED` | Halted by `SIGSTOP` / debugger |
| Zombie | `EXIT_ZOMBIE` | Terminated but parent hasn't called `wait()` |

**Embedded relevance:** Processes stuck in `D` state (uninterruptible sleep) are a common embedded debugging problem — usually caused by a hung device driver waiting for hardware that never responds.

> [!WARNING]
> **Interview gotcha:** "Can you kill a process in `D` state?" — No. Not even `SIGKILL` works. The driver must complete or time out. This is a real embedded bug pattern.

### 1.3 Process Control Block (PCB) — `task_struct`

**Definition:** The kernel data structure (`task_struct` in Linux, ~6KB) that stores everything the kernel needs to manage a process: PID, state, register context, memory maps (`mm_struct`), file descriptors, signal masks, scheduling info, CPU affinity.

**Why it matters for embedded:** When writing kernel modules or debugging with `/proc/<pid>/`, you're directly interacting with fields of `task_struct`. Understanding its layout helps you debug scheduling anomalies and memory issues.

```c
// Key fields you should know:
struct task_struct {
    volatile long state;          // TASK_RUNNING, etc.
    struct mm_struct *mm;         // Virtual memory maps (NULL for kernel threads)
    pid_t pid;                    // Process ID
    int prio;                     // Dynamic priority
    struct files_struct *files;   // Open file descriptors
    cpumask_t cpus_allowed;       // CPU affinity mask
    // ...hundreds more fields
};
```

**Common interview follow-ups:**
- "What's the difference between `pid` and `tgid`?" → `tgid` (Thread Group ID) is the PID of the thread group leader. For single-threaded processes, `pid == tgid`. For threads, each has a unique `pid` but shares the same `tgid`. `getpid()` actually returns `tgid`.

### 1.4 Context Switch

**Definition:** The process of saving the CPU state (registers, program counter, stack pointer) of the currently running process/thread and restoring the state of the next scheduled one. This is the core mechanism enabling multitasking.

**What happens during a context switch:**
1. Save current process registers to its kernel stack / `task_struct`
2. Switch the page table base register (e.g., `TTBR0` on ARM) — this changes the virtual address space
3. Flush/invalidate TLB entries (or use ASIDs to avoid full flush)
4. Restore registers of the next process from its kernel stack
5. Jump to the restored PC

**Cost:** ~1–10 µs on modern hardware. The real cost is **cache pollution** — the new process's working set replaces the old one in cache, causing cache misses.

**Embedded relevance:** On resource-constrained systems, excessive context switching kills real-time performance. This is why RTOS designers minimize unnecessary preemptions and why spinlocks (which avoid context switches) are preferred for very short critical sections.

```
Interview question: "What's more expensive — a context switch between
two threads in the same process, or between two different processes?"

Answer: Process switch is more expensive because:
  - Thread switch: Only saves/restores registers (shared address space)
  - Process switch: Saves/restores registers + switches page tables + TLB flush
```

### 1.5 fork(), vfork(), exec(), clone()

#### `fork()`
Creates a child process by duplicating the parent. Uses **Copy-on-Write (COW)**: pages are shared read-only until either process writes, at which point only the modified page is physically copied.

```c
pid_t pid = fork();
if (pid == 0) {
    // Child: fork() returned 0
    printf("I am child, PID=%d\n", getpid());
} else if (pid > 0) {
    // Parent: fork() returned child's PID
    wait(NULL);  // Reap child — MUST do this to avoid zombies
} else {
    perror("fork failed");  // Returns -1 on error
}
```

**Key detail:** The child inherits copies of all file descriptors (sharing the same open file descriptions, including file offset).

#### `vfork()`
Shares the parent's address space with the child (no COW). The parent is **suspended** until the child calls `exec()` or `_exit()`. Dangerous because the child can corrupt the parent's stack.

**Embedded relevance:** `vfork()` was historically used on MMU-less systems where COW isn't possible. On modern systems with MMU, prefer `fork()` + `exec()` or `posix_spawn()`.

#### `exec()` family
Replaces the current process image with a new program. Does **not** create a new process (same PID).

```c
// Typical fork+exec pattern:
pid_t pid = fork();
if (pid == 0) {
    execl("/bin/ls", "ls", "-l", NULL);  // Child becomes 'ls'
    _exit(1);  // Only reached if exec fails
}
```

#### `clone()` — **[MISSING from your notes]**
The Linux-specific system call underlying both `fork()` and `pthread_create()`. It allows fine-grained control over what is shared between parent and child via flags:

| Flag | What's Shared |
|------|--------------|
| `CLONE_VM` | Virtual memory (address space) |
| `CLONE_FILES` | File descriptor table |
| `CLONE_SIGHAND` | Signal handlers |
| `CLONE_THREAD` | Same thread group (share PID externally) |

- `fork()` = `clone()` with no sharing flags
- `pthread_create()` = `clone(CLONE_VM | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | ...)`

### 1.6 Thread vs Process

| Aspect | Process | Thread |
|--------|---------|--------|
| Address space | Own (isolated) | Shared with all threads in process |
| Creation cost | High (~fork + page table copy) | Low (~10x faster, just new stack + TCB) |
| Communication | IPC required (pipes, shmem) | Direct memory access (but needs sync) |
| Crash isolation | Crash doesn't affect other processes | Crash kills the entire process |
| Context switch cost | Higher (TLB flush, page table switch) | Lower (register save/restore only) |
| File descriptors | Inherited copies (separate table after fork) | Shared table |

**Embedded design decision:** Use multiple processes for fault isolation (e.g., a crash in the networking daemon doesn't kill sensor sampling). Use multiple threads within a process for performance-critical parallelism where shared data is essential.

### 1.7 Zombies and Orphans

**Zombie:** Child has terminated, but parent hasn't called `wait()`. The kernel keeps a minimal entry (PID, exit status) in the process table. Zombies consume PID table entries — in embedded systems with limited PIDs, this is a resource leak.

**Orphan:** Parent dies before child. The child is re-parented to `init` (PID 1), which automatically reaps it.

```c
// Classic zombie leak — NEVER do this in production embedded code:
while (1) {
    if (fork() == 0) {
        _exit(0);  // Child exits immediately
    }
    // Parent never calls wait() → zombies accumulate
}

// Fix: install SIGCHLD handler
signal(SIGCHLD, SIG_IGN);  // Auto-reap (Linux-specific)
// Or properly:
struct sigaction sa;
sa.sa_handler = sigchld_handler;
sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
sigaction(SIGCHLD, &sa, NULL);

void sigchld_handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);  // Reap all
}
```

### 1.8 POSIX Threads (pthreads) Lifecycle

| API | Purpose |
|-----|---------|
| `pthread_create()` | Create a new thread |
| `pthread_join()` | Wait for thread termination (like `waitpid` for threads) |
| `pthread_detach()` | Mark as detached — resources auto-freed on termination |
| `pthread_exit()` | Terminate calling thread |
| `pthread_cancel()` | Request thread cancellation |

```c
#include <pthread.h>
#include <stdio.h>

void *worker(void *arg) {
    int id = *(int *)arg;
    printf("Thread %d running\n", id);
    return (void *)(long)id;
}

int main() {
    pthread_t t;
    int id = 42;
    void *retval;
    
    pthread_create(&t, NULL, worker, &id);
    pthread_join(t, &retval);  // MUST join or detach — else resource leak
    printf("Thread returned: %ld\n", (long)retval);
}
```

> [!IMPORTANT]
> **Embedded gotcha — stack size:** Default pthread stack is ~2MB on x86 Linux. In embedded, set it explicitly:
> ```c
> pthread_attr_t attr;
> pthread_attr_init(&attr);
> pthread_attr_setstacksize(&attr, 16384);  // 16KB — common in embedded
> pthread_create(&t, &attr, worker, NULL);
> ```

**Common interview follow-ups:**
- "What happens if you neither join nor detach a thread?" → Resource leak (zombie thread). The thread's stack and TCB are never freed.
- "Can you join a detached thread?" → No. Undefined behavior.
- "Thread vs pthread — what's the diff?" → `pthread` is the POSIX API. In Linux, threads are implemented via `clone()` and are scheduled as individual `task_struct` entries (1:1 model via NPTL).

---

## 2. CPU Scheduling 🟡

### 2.1 Preemptive vs Non-Preemptive

**Non-Preemptive (Cooperative):** A running process keeps the CPU until it voluntarily yields (blocks on I/O, exits, or calls yield). Risk: a misbehaving task can starve everything else.

**Preemptive:** The scheduler can forcibly take the CPU away from a running process (via timer interrupt). Essential for real-time and interactive systems.

**Embedded relevance:** RTOS always uses preemptive priority-based scheduling. A higher-priority task **must** preempt a lower-priority one immediately for real-time guarantees.

### 2.2 Scheduling Algorithms

#### FCFS (First Come First Served)
- **How:** Processes run in arrival order, non-preemptive.
- **Problem:** Convoy effect — short jobs wait behind long ones.
- **Embedded use:** Almost never used in real-time. Too unpredictable.

#### SJF (Shortest Job First) / SRTF (Shortest Remaining Time First)
- **SJF:** Non-preemptive. Picks shortest burst. Optimal average wait time but requires knowing burst times (impractical in real systems).
- **SRTF:** Preemptive SJF. Preempts if a shorter job arrives. Better average wait but high overhead.
- **Problem:** Starvation of long jobs.

#### Priority Scheduling
- **How:** Each process has a priority. Highest priority runs first.
- **Problem:** Starvation → solved by **aging** (gradually increasing priority of waiting processes).
- **Embedded relevance:** The foundation of RTOS scheduling. But introduces priority inversion risk.

#### Round Robin (RR)
- **How:** Each process gets a fixed time quantum. After quantum expires, it goes to the back of the queue.
- **Tradeoff:** Small quantum → good response time but high context-switch overhead. Large quantum → degrades to FCFS.
- **Typical quantum:** 10–100 ms in GPOS. In RTOS, time slicing is usually within the same priority level only.

#### Multilevel Queue
- **How:** Multiple queues with different priorities and algorithms (e.g., Queue 0: RT with FIFO, Queue 1: Interactive with RR, Queue 2: Batch with FCFS).
- **Problem:** Lower queues can starve.

#### Multilevel Feedback Queue — **[MISSING]**
- **How:** Like multilevel queue, but processes can **move between queues** based on behavior. CPU-bound tasks get demoted; I/O-bound tasks get promoted.
- **Embedded relevance:** Linux's CFS is conceptually related. Not used directly in RTOS.

### 2.3 Linux CFS (Completely Fair Scheduler) — **[MISSING]**

**Definition:** CFS is the default scheduler for `SCHED_OTHER` tasks in Linux (since 2.6.23). It models an "ideal multitasking CPU" where each task gets exactly 1/N of CPU time.

**How it works:**
- Each task tracks its `vruntime` (virtual runtime) — weighted by nice value
- Tasks are stored in a **red-black tree** ordered by `vruntime`
- The scheduler always picks the task with the **smallest `vruntime`** (leftmost node)
- Nice value maps to a weight: nice -20 gets ~88x more CPU than nice +19

```
vruntime += (actual_runtime × NICE_0_WEIGHT) / task_weight
```

**Embedded relevance:** CFS is a **fairness** scheduler, not a real-time scheduler. For deterministic latency, you must use `SCHED_FIFO` or `SCHED_RR`. CFS has unbounded latency by design.

### 2.4 Real-Time Scheduling in Linux

Linux provides two POSIX real-time policies that **always preempt** CFS tasks:

| Policy | Behavior | When to use |
|--------|----------|-------------|
| `SCHED_FIFO` | Runs until it blocks/yields. No time slicing. | Deterministic, time-critical tasks (sensor polling, motor control) |
| `SCHED_RR` | Like FIFO but with time slicing among same-priority tasks | Multiple RT tasks at same priority need fairness |

**Priority range:** 1–99 (higher = more urgent). RT tasks at priority 99 will preempt everything.

```c
// Setting a thread to SCHED_FIFO priority 80:
struct sched_param param;
param.sched_priority = 80;
pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
```

> [!WARNING]
> **Interview gotcha:** "What happens if a SCHED_FIFO task has a bug and runs forever?" → It starves all lower-priority tasks and CFS tasks. The system appears frozen. This is why the Linux kernel provides `/proc/sys/kernel/sched_rt_runtime_us` (default: 950000 µs per 1000000 µs period) — RT tasks get at most 95% of CPU, reserving 5% for system health.

### 2.5 CPU Affinity — **[MISSING]**

**Definition:** Binding a thread to run only on specific CPU core(s).

- **Soft affinity:** The scheduler tries to keep a thread on the same core (for cache warmth) but may migrate it.
- **Hard affinity:** `sched_setaffinity()` or `pthread_attr_setaffinity_np()` forces a thread to specific cores.

```c
cpu_set_t cpuset;
CPU_ZERO(&cpuset);
CPU_SET(2, &cpuset);  // Pin to core 2
sched_setaffinity(0, sizeof(cpuset), &cpuset);
```

**Embedded relevance:** Critical for RT tasks — pin time-critical threads to a dedicated core, isolate that core from other tasks using `isolcpus` kernel parameter.

**Common interview follow-ups:**
- "What's the `isolcpus` boot parameter?" → Removes CPUs from the general scheduler pool. Only tasks explicitly assigned to those CPUs will run there. Used for dedicated real-time processing.
- "What scheduling algorithm does FreeRTOS use?" → Fixed-priority preemptive with optional round-robin time-slicing for equal-priority tasks. No CFS, no nice values.

---

## 3. Process Synchronization & Concurrency 🔴

> [!IMPORTANT]
> This is the **#1 most grilled topic** in embedded interviews. Expect questions about when to use each primitive, what happens in ISR context, and real deadlock scenarios.

### 3.1 Race Conditions

**Definition:** A race condition occurs when the output of a program depends on the timing/interleaving of concurrent threads or processes accessing shared data, and at least one access is a write.

**Classic example — Read-Modify-Write:**
```c
int counter = 0;  // Shared between threads

// Thread A and B both do:
counter++;  // NOT atomic! Compiles to:
            //   LOAD  counter → reg
            //   ADD   reg, 1
            //   STORE reg → counter

// Interleaving that loses an increment:
// Thread A: LOAD counter (=0)
//   --- context switch ---
// Thread B: LOAD counter (=0), ADD (=1), STORE (counter=1)
//   --- context switch ---
// Thread A: ADD (=1), STORE (counter=1)  // Should be 2!
```

**Embedded relevance:** Race conditions are the root cause of intermittent "once-a-week" bugs in embedded systems. They're non-deterministic and notoriously hard to reproduce.

### 3.2 Critical Section

**Definition:** A code segment that accesses shared resources and must be executed atomically (no concurrent access). Entry and exit to the critical section must be guarded by a synchronization primitive.

**Requirements:**
1. **Mutual exclusion:** Only one thread/process at a time
2. **Progress:** If no one is in the CS, a waiting thread should be able to enter
3. **Bounded waiting:** A thread shouldn't wait forever

### 3.3 Mutex

**Definition:** A binary lock where only the **owner** can unlock it. A thread that tries to lock a held mutex is put to **sleep** (blocked) until the mutex is released.

**Internal implementation (Linux futex):**
```
Fast path (no contention):
  Atomic CAS on user-space integer: 1→0 (locked), 0→1 (unlocked)
  No system call needed!

Slow path (contention):
  futex(FUTEX_WAIT) — thread sleeps in kernel wait queue
  On unlock: futex(FUTEX_WAKE) — wake one waiter
```

**Key properties:**
- Has **ownership** — only the locker can unlock
- Supports **priority inheritance** (if configured)
- Sleeping behavior → **cannot be used in ISR context**

```c
// Correct mutex usage:
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg) {
    pthread_mutex_lock(&lock);    // Sleep if contended
    // --- critical section ---
    shared_data++;
    // --- end critical section ---
    pthread_mutex_unlock(&lock);  // Only the owner can unlock
    return NULL;
}
```

### 3.4 Spinlock

**Definition:** A lock where the waiting thread **busy-waits** (spins) in a tight loop until the lock becomes available. No context switch, no sleeping.

```c
// Pseudocode of a spinlock:
void spin_lock(spinlock_t *lock) {
    while (atomic_test_and_set(&lock->locked) == 1) {
        // Spin — burns CPU cycles
    }
}

void spin_unlock(spinlock_t *lock) {
    atomic_clear(&lock->locked);
}
```

**Key properties:**
- No sleeping → **CAN be used in ISR context** (with `spin_lock_irqsave`)
- Wastes CPU → only efficient for **very short** critical sections (< few µs)
- On uniprocessor: `spin_lock()` simply disables preemption (no point spinning if no other CPU can release the lock)

### 3.5 Mutex vs Spinlock — The Critical Comparison

| Aspect | Mutex | Spinlock |
|--------|-------|----------|
| Wait mechanism | Sleep (blocks, yields CPU) | Busy-wait (burns CPU) |
| Context switch? | Yes (on contention) | No |
| ISR-safe? | **NO** (sleeping is forbidden in ISR) | **YES** (with `spin_lock_irqsave`) |
| Best for | Long critical sections (> ~10µs) | Very short critical sections (< ~10µs) |
| Priority inheritance | Supported | Not applicable |
| Deadlock on self-lock | Detected (with error-checking mutex) | **Hard hang** (busy-waits forever) |

```c
// WRONG — mutex in ISR context:
irqreturn_t my_isr(int irq, void *dev) {
    mutex_lock(&my_mutex);     // BUG: may sleep in ISR!
    shared_data = read_hw();
    mutex_unlock(&my_mutex);
    return IRQ_HANDLED;
}

// CORRECT — spinlock in ISR context:
irqreturn_t my_isr(int irq, void *dev) {
    unsigned long flags;
    spin_lock_irqsave(&my_spinlock, flags);  // Disables IRQs + acquires lock
    shared_data = read_hw();
    spin_unlock_irqrestore(&my_spinlock, flags);  // Restores IRQ state
    return IRQ_HANDLED;
}
```

> [!CAUTION]
> **The #1 embedded interview question on this topic:** "Can you use a mutex in an ISR?" → **No.** Mutexes can sleep, and sleeping in ISR context causes a kernel panic. Use `spin_lock_irqsave()` for ISR-to-thread synchronization.

### 3.6 Semaphore

**Definition:** A counter-based synchronization primitive. Unlike mutexes, semaphores have no ownership — any thread can post (signal) a semaphore, not just the one that waited.

| Type | Counter | Use case |
|------|---------|----------|
| Binary (0/1) | 0 or 1 | Like a mutex but without ownership; used for signaling |
| Counting (0..N) | 0 to N | Limiting concurrent access to N resources (e.g., connection pool) |

**Semaphore vs Mutex:**

| Aspect | Mutex | Semaphore |
|--------|-------|-----------|
| Ownership | Yes (only owner unlocks) | No (any thread can post) |
| Primary use | Mutual exclusion | Signaling / resource counting |
| Priority inheritance | Yes | **No** (major problem for RT) |
| Can be > 1 | No (binary only) | Yes (counting semaphore) |

```c
// Producer-Consumer with semaphore:
sem_t empty_slots, full_slots;
pthread_mutex_t buffer_lock;

sem_init(&empty_slots, 0, BUFFER_SIZE);  // Initially all slots empty
sem_init(&full_slots, 0, 0);            // Initially no items

void *producer(void *arg) {
    while (1) {
        sem_wait(&empty_slots);          // Decrement: wait for empty slot
        pthread_mutex_lock(&buffer_lock);
        // --- add item to buffer ---
        pthread_mutex_unlock(&buffer_lock);
        sem_post(&full_slots);           // Increment: signal item available
    }
}
```

### 3.7 Condition Variables

**Definition:** A mechanism for threads to wait until a particular condition on shared data is true. Always used **with a mutex**.

**The wait loop pattern (critical to get right in interviews):**
```c
pthread_mutex_lock(&mtx);
while (!condition) {                    // MUST be a while loop, not if
    pthread_cond_wait(&cond, &mtx);    // Atomically: unlock mtx + sleep
    // On wakeup: mtx is re-locked automatically
}
// condition is now true, mtx is held
do_work();
pthread_mutex_unlock(&mtx);
```

**Why `while` not `if`?**
1. **Spurious wakeups:** POSIX allows `pthread_cond_wait` to return even without a signal
2. **Stolen wakeups:** Another thread may have consumed the resource between signal and wakeup

### 3.8 Reader-Writer Locks — **[MISSING]**

**Definition:** A lock allowing multiple concurrent readers OR one exclusive writer. Optimizes read-heavy workloads.

```c
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

// Reader (many can hold simultaneously):
pthread_rwlock_rdlock(&rwlock);
int val = shared_data;    // Read-only access
pthread_rwlock_unlock(&rwlock);

// Writer (exclusive access):
pthread_rwlock_wrlock(&rwlock);
shared_data = new_value;  // Read-write access
pthread_rwlock_unlock(&rwlock);
```

**Embedded relevance:** Used in kernel for protecting data structures read frequently but modified rarely (e.g., routing tables, device lists).

**Gotcha:** Writer starvation — if readers keep arriving, the writer may never get the lock. Solution: writer-preference rwlocks.

### 3.9 Deadlock

**Definition:** A state where two or more threads are blocked forever, each waiting for a resource held by the other.

**Four necessary conditions (ALL must hold):**
1. **Mutual Exclusion** — resource is non-shareable
2. **Hold and Wait** — thread holds one resource while waiting for another
3. **No Preemption** — resources can't be forcibly taken
4. **Circular Wait** — A waits for B's resource, B waits for A's

**Classic two-mutex deadlock:**
```c
// Thread A:                      // Thread B:
pthread_mutex_lock(&lock_1);      pthread_mutex_lock(&lock_2);
  // ... some work ...              // ... some work ...
pthread_mutex_lock(&lock_2);      pthread_mutex_lock(&lock_1);
  // DEADLOCK!                      // DEADLOCK!

// FIX: Always acquire locks in the same global order:
// Thread A:                      // Thread B:
pthread_mutex_lock(&lock_1);      pthread_mutex_lock(&lock_1);  // Same order!
pthread_mutex_lock(&lock_2);      pthread_mutex_lock(&lock_2);
```

**Prevention strategies:**

| Strategy | Attacks Condition | Method |
|----------|------------------|--------|
| Lock ordering | Circular wait | Always acquire locks in a fixed global order |
| All-at-once | Hold and wait | Request all resources before starting |
| Trylock + backoff | Hold and wait | Use `pthread_mutex_trylock()`; on failure, release all held locks and retry |
| Timeout | All | `pthread_mutex_timedlock()` — give up after deadline |

**Banker's Algorithm (Avoidance):** Pre-check if granting a request leaves the system in a "safe state" (a sequence exists where all processes can complete). Theoretical — rarely used in embedded practice. Lock ordering is the practical solution.

**Detection:** Build a wait-for graph. If there's a cycle → deadlock. Linux kernel has `lockdep` — a runtime lock dependency validator that detects potential deadlocks *before* they actually occur.

### 3.10 Priority Inversion

**Definition:** A high-priority task (H) is blocked because a low-priority task (L) holds a resource H needs. A medium-priority task (M) then preempts L, effectively making H wait for M — inverting the priority hierarchy.

```
Time →
H: [run] [BLOCKED on mutex held by L..........]  ← waits indefinitely
M:              [preempts L] [running..........]  ← runs before H!
L: [holds mutex] [preempted by M] [can't run...]
```

**The Mars Pathfinder Bug (1997):** A priority inversion caused the spacecraft's computer to repeatedly reset via watchdog. The fix was enabling Priority Inheritance Protocol in VxWorks RTOS, applied via a remote patch to Mars.

**Solutions:**

| Protocol | How it works | Pros | Cons |
|----------|-------------|------|------|
| Priority Inheritance (PIP) | L temporarily inherits H's priority while holding the mutex | Simple, reactive | Transitive chains can cascade; doesn't prevent deadlock |
| Priority Ceiling (PCP) | Each mutex has a ceiling = max priority of any task that may lock it. A task can only lock the mutex if its priority > ceilings of all currently locked mutexes | Prevents deadlock + inversion | Requires static analysis; overly conservative |

```c
// Enabling Priority Inheritance in Linux:
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);

pthread_mutex_t pi_mutex;
pthread_mutex_init(&pi_mutex, &attr);
// Now if H blocks on pi_mutex held by L, L's priority is boosted to H's level
```

**Interview follow-up:** "Does the Linux kernel itself use priority inheritance?" → Yes. The `rt_mutex` in the kernel implements PI. Regular `mutex` in kernel does not. Futex-based mutexes in user-space support PI via `PTHREAD_PRIO_INHERIT`.

### 3.11 Starvation

**Definition:** A thread is perpetually denied access to a needed resource because other threads keep getting it first. Unlike deadlock, the system is making progress — just not for the starved thread.

**Common causes:**
- Priority scheduling without aging
- Writer starvation in reader-writer locks
- Unfair spinlock implementations

**Solution:** Aging — gradually increase the priority of waiting tasks. Or use fair locks (e.g., ticket spinlocks, queued spinlocks).

---

## 4. Memory Management 🔴

### 4.1 Memory Layout of a Process

```
High Address (e.g., 0xBFFFFFFF on 32-bit)
┌──────────────────────┐
│   Kernel Space       │  ← User can't access (above 0xC0000000 on 32-bit)
├──────────────────────┤  ← 0xC0000000 (3GB/1GB split typical)
│       Stack          │  ↓ Grows downward
│   (local vars,       │
│    return addresses)  │
├──────────────────────┤
│         ↕            │  ← Unmapped gap (guard page)
├──────────────────────┤
│       Heap           │  ↑ Grows upward
│   (malloc/free)      │
├──────────────────────┤
│   BSS (uninitialized)│  ← Zero-initialized global/static vars
├──────────────────────┤
│   Data (initialized) │  ← Global/static vars with initial values
├──────────────────────┤
│   Text (Code)        │  ← Read-only, executable
└──────────────────────┘
Low Address (e.g., 0x08048000)
```

### 4.2 Stack vs Heap

| Aspect | Stack | Heap |
|--------|-------|------|
| Allocation | Automatic (compiler-managed) | Manual (`malloc`/`free`) |
| Speed | Very fast (just adjust SP) | Slower (allocator overhead) |
| Size | Small (1–8MB typical, configurable) | Limited by system RAM |
| Fragmentation | None (LIFO) | Both internal and external |
| Lifetime | Function scope | Until `free()` is called |
| Thread safety | Each thread has own stack | Shared — needs synchronization |
| Direction | Grows downward (typically) | Grows upward (typically) |

**Embedded relevance:** Stack overflow is a critical embedded bug. No MMU → no guard page → silent corruption of adjacent data. FreeRTOS provides stack overflow detection hooks (`vApplicationStackOverflowHook`).

### 4.3 Paging

**Definition:** A memory management scheme dividing virtual memory into fixed-size **pages** (typically 4KB) and physical memory into **frames** of the same size. A **page table** maps virtual page numbers to physical frame numbers.

**Address translation:**
```
Virtual Address = [Page Number | Offset]
                       ↓
              Page Table lookup
                       ↓
Physical Address = [Frame Number | Offset]
```

**Multi-level page tables (reduces memory overhead):**
- 32-bit x86: 2-level (Page Directory → Page Table)
- 64-bit x86: 4-level (PGD → PUD → PMD → PTE)
- ARM (AArch64): Up to 4-level

**Why paging matters for embedded:**
- Eliminates external fragmentation
- Enables virtual memory (processes can use more memory than physical RAM)
- Enables memory protection (page-level permissions: R/W/X)
- Enables COW fork

### 4.4 TLB (Translation Lookaside Buffer)

**Definition:** A hardware cache inside the CPU that stores recent virtual-to-physical page translations. Without TLB, every memory access would require walking the page table (multiple memory accesses for multi-level tables).

**Performance impact:**
- TLB hit: ~1 cycle overhead
- TLB miss: ~10–100 cycles (page table walk)
- TLB is usually small: 64–1024 entries

**ASID (Address Space ID):** Tags TLB entries with a process identifier, avoiding full TLB flush on context switch. ARM supports ASIDs natively.

**Embedded relevance:** TLB misses are a significant source of latency jitter in real-time systems. Using **huge pages** (2MB instead of 4KB) reduces TLB pressure for large buffers.

> [!TIP]
> **Interview question:** "What happens to the TLB on a context switch?" → The TLB must be flushed (entries from old process are invalid for new process), unless the hardware supports ASIDs. TLB flush → cold start → cache misses → performance hit.

### 4.5 Page Faults

**Definition:** A CPU exception triggered when a process accesses a virtual page that is not currently mapped to a physical frame.

| Type | Cause | Cost |
|------|-------|------|
| Minor (soft) | Page is in memory but PTE not set up (e.g., first access to mmap'd region, COW) | ~1–10 µs |
| Major (hard) | Page is on disk (swapped out). Must do disk I/O. | ~1–10 **ms** (1000x slower!) |
| Invalid | Access to unmapped address or permission violation | SIGSEGV → process killed |

**Embedded relevance:** Major page faults are **unacceptable** in real-time systems. Solutions:
- `mlockall(MCL_CURRENT | MCL_FUTURE)` — lock all pages in RAM, prevent swapping
- Pre-fault stack memory before entering RT section
- Disable swap entirely

```c
// Real-time embedded pattern:
#include <sys/mman.h>

int main() {
    mlockall(MCL_CURRENT | MCL_FUTURE);  // Lock everything in RAM
    
    // Pre-fault the stack:
    volatile char stack_prefault[MAX_STACK_SIZE];
    memset((void *)stack_prefault, 0, sizeof(stack_prefault));
    
    // Now enter real-time loop — no page faults possible
    while (1) {
        // time-critical work
    }
}
```

### 4.6 MMU (Memory Management Unit) — **[MISSING]**

**Definition:** Hardware component that translates virtual addresses to physical addresses using page tables. It enforces memory protection (prevents user-space from accessing kernel memory, prevents processes from accessing each other's memory).

**Key functions:**
1. Address translation (virtual → physical via page table walk)
2. Memory protection (permission bits: read/write/execute, user/kernel)
3. Cache control attributes per page

**Embedded relevance:** Many microcontrollers (Cortex-M) have an **MPU** (Memory Protection Unit) instead of a full MMU:

| Feature | MMU | MPU |
|---------|-----|-----|
| Virtual memory | Yes | No (physical addresses only) |
| Paging/swap | Yes | No |
| Address translation | Yes | No |
| Memory protection | Yes (per-page) | Yes (per-region, typically 8–16 regions) |
| OS support | Linux | FreeRTOS, Zephyr, bare-metal |
| Used in | Cortex-A, x86 | Cortex-M, Cortex-R |

### 4.7 Virtual Memory — **[MISSING]**

**Definition:** An abstraction that gives each process the illusion of having its own large, contiguous address space, regardless of physical RAM size. Implemented via paging + swap space on disk.

**Key benefits:**
1. **Isolation:** Each process has its own address space; can't corrupt others
2. **More memory than RAM:** Inactive pages can be swapped to disk
3. **Simplified linking:** Every process loads at the same virtual address
4. **Shared libraries:** Multiple processes map the same physical pages of libc

**Embedded relevance:** Embedded Linux uses virtual memory. Bare-metal RTOS (FreeRTOS on Cortex-M) does **not** — all tasks share a flat physical address space. This is why MPU-based region protection is important on Cortex-M.

### 4.8 Segmentation

**Definition:** Dividing a program's memory into logical **segments** (code, data, stack, heap) with base address + limit for each. Segments can be of different sizes.

**Paging vs Segmentation:**

| Aspect | Paging | Segmentation |
|--------|--------|-------------|
| Unit | Fixed-size pages | Variable-size segments |
| Fragmentation | Internal (last page waste) | External (variable-size holes) |
| View | Physical organization | Logical organization |
| Modern use | Primary mechanism | Largely deprecated (x86-64 has flat segments) |

**Embedded relevance:** ARM processors don't use segmentation. x86-64 uses flat segmentation (base=0, limit=max). The concept matters mainly for understanding linker scripts in bare-metal embedded (`.text`, `.data`, `.bss`, `.rodata` sections).

### 4.9 Fragmentation

**Internal:** Wasted space *within* allocated blocks (e.g., allocator gives 64-byte block for a 10-byte request → 54 bytes wasted).

**External:** Free memory exists but is scattered in non-contiguous chunks, so a large allocation fails even though total free memory is sufficient.

**Solutions:**
| Solution | How | Where used |
|----------|-----|------------|
| Compaction | Move allocations to consolidate free space | Rarely practical in C (pointers would break) |
| Buddy system | Allocate power-of-2 blocks, merge buddies on free | Linux kernel (`alloc_pages`) |
| Slab allocator | Pre-allocate caches of fixed-size objects | Linux kernel (for `task_struct`, `inode`, etc.) |
| Memory pools | Fixed-size block pools | FreeRTOS, embedded allocators |

### 4.10 DMA (Direct Memory Access)

**Definition:** A hardware mechanism that allows peripherals (SPI, UART, Ethernet, etc.) to transfer data directly to/from RAM without CPU involvement. The CPU sets up the transfer (source, destination, length) and gets an interrupt on completion.

**Types of DMA mappings:**

| Type | When to use | Lifetime |
|------|-------------|----------|
| Coherent (`dma_alloc_coherent`) | Data accessed by both CPU and device frequently | Long-lived (usually driver lifetime) |
| Streaming (`dma_map_single`) | One-shot or per-transfer buffers | Per-transfer (map before, unmap after) |
| Scatter-Gather (`dma_map_sg`) | Non-contiguous buffers | Per-transfer |

**Cache coherency problem:**
```
CPU writes data → data is in CPU cache but NOT in RAM
DMA reads from RAM → reads stale data!

Solution:
  Before DMA read from RAM:  CPU flushes cache → data reaches RAM
  After DMA writes to RAM:   CPU invalidates cache → next CPU read goes to RAM
```

```c
// Streaming DMA example (Linux driver):
dma_addr_t dma_handle;
void *buf = kmalloc(BUF_SIZE, GFP_KERNEL);

// Fill buffer with data to send to device
memcpy(buf, data, BUF_SIZE);

// Map for DMA — flushes cache automatically
dma_handle = dma_map_single(dev, buf, BUF_SIZE, DMA_TO_DEVICE);

// Program the DMA controller with dma_handle (physical address)
start_dma_transfer(dma_handle, BUF_SIZE);

// After transfer complete (interrupt)...
dma_unmap_single(dev, dma_handle, BUF_SIZE, DMA_TO_DEVICE);
```

### 4.11 Memory-Mapped I/O vs Port I/O — **[MISSING]**

**Memory-Mapped I/O (MMIO):**
- Device registers appear at physical memory addresses
- Accessed via normal load/store instructions
- Used by ARM, RISC-V, and modern x86 devices
- Requires `ioremap()` in Linux kernel to map physical device address into kernel virtual space

```c
// Accessing a device register via MMIO:
void __iomem *base = ioremap(0x4000C000, 0x1000);
u32 status = readl(base + STATUS_REG_OFFSET);   // Use readl/writel!
writel(0x01, base + CONTROL_REG_OFFSET);
iounmap(base);
```

**Port I/O (PIO):**
- Separate I/O address space accessed via special instructions (`IN`/`OUT` on x86)
- Legacy x86 only (e.g., COM ports, keyboard controller)
- Not used on ARM at all

> [!TIP]
> **Interview question:** "Why use `readl()`/`writel()` instead of direct pointer dereference?" → They include memory barriers and compiler ordering guarantees. A raw `*(volatile u32 *)addr` may be reordered by the compiler or CPU, causing writes to reach the device in wrong order.

### 4.12 Slab Allocator — **[MISSING]**

**Definition:** A kernel memory allocator that pre-allocates caches of frequently used objects (e.g., `task_struct`, `inode`, `sk_buff`). Objects are allocated/freed without the overhead of the general allocator.

**Why it matters:** `kmalloc()` uses the slab allocator internally. Understanding this helps debug kernel memory leaks and optimize driver memory usage.

```c
// Creating a slab cache in a driver:
struct kmem_cache *my_cache;

// Module init:
my_cache = kmem_cache_create("my_objects", sizeof(struct my_obj), 0,
                              SLAB_HWCACHE_ALIGN, NULL);

// Allocate an object (very fast — pre-allocated):
struct my_obj *obj = kmem_cache_alloc(my_cache, GFP_KERNEL);

// Free:
kmem_cache_free(my_cache, obj);

// Module exit:
kmem_cache_destroy(my_cache);
```

---

## 5. Interrupts & Exceptions 🔴

### 5.1 What is an Interrupt?

**Definition:** An asynchronous signal from hardware (or software) that causes the CPU to suspend its current execution, save context, and jump to a predefined handler (ISR). After the ISR completes, the CPU resumes the interrupted code.

**Types:**
| Type | Source | Examples |
|------|--------|----------|
| Hardware interrupt (IRQ) | External device | Timer tick, UART RX, GPIO edge, DMA complete |
| Software interrupt (trap) | CPU instruction | `SVC` (ARM syscall), `INT 0x80` (x86 syscall) |
| Exception | CPU fault | Page fault, divide-by-zero, undefined instruction |

### 5.2 Interrupt Handling Flow (ARM)

```
1. Hardware asserts IRQ line → Interrupt controller (GIC) prioritizes
2. CPU finishes current instruction
3. CPU saves CPSR to SPSR, saves return address to LR
4. CPU switches to IRQ mode, disables further IRQs
5. CPU jumps to IRQ vector (fixed address, e.g., 0x18)
6. Kernel ISR prologue: save registers to stack
7. Identify interrupt source (read GIC register)
8. Call registered handler
9. Acknowledge interrupt (write EOI to GIC)
10. Restore registers, return from exception (CPSR restored from SPSR)
```

### 5.3 ISR Constraints

> [!CAUTION]
> **These are interview-critical rules. Violating any of them causes kernel panics or data corruption.**

| Rule | Why |
|------|-----|
| **Keep it short** | Long ISRs increase interrupt latency for other IRQs (jitter) |
| **No sleeping** | ISRs run in interrupt context — no `task_struct`, no scheduling |
| **No blocking calls** | No `mutex_lock()`, no `kmalloc(GFP_KERNEL)`, no `copy_from_user()` |
| **No `printf()`/`printk()` (carefully)** | `printk()` is technically okay but slow; avoid in fast paths |
| **Use `GFP_ATOMIC`** | If you must allocate memory: `kmalloc(size, GFP_ATOMIC)` |
| **No floating point** | FPU state isn't saved/restored in ISR context (on many architectures) |
| **Use `spin_lock_irqsave()`** | Not just `spin_lock()` — must disable IRQs to prevent deadlock with same ISR |

### 5.4 Top-Half / Bottom-Half

**Problem:** You want to respond to hardware immediately (acknowledge, grab data) but also do heavy processing (protocol decoding, waking processes). You can't do both in the ISR because it's too long.

**Solution:** Split the work:

| Part | Runs in | Can sleep? | What it does |
|------|---------|------------|-------------|
| **Top half** (hardirq) | Interrupt context | No | Minimum: acknowledge HW, read urgent data, schedule bottom half |
| **Bottom half** (deferred) | Varies | Depends on type | Heavy processing |

### 5.5 Bottom-Half Mechanisms

| Mechanism | Context | Can sleep? | Concurrency | When to use |
|-----------|---------|------------|-------------|-------------|
| **Softirq** | Interrupt (soft) | No | Runs on multiple CPUs simultaneously | High-performance: networking (`NET_TX_SOFTIRQ`), block I/O |
| **Tasklet** | Interrupt (soft, built on softirq) | No | Same tasklet serialized (won't run on 2 CPUs at once) | Simpler than softirq; most driver bottom halves |
| **Workqueue** | Process (kernel thread) | **Yes** | Full process context | When you need to sleep (e.g., I2C transfer, memory allocation) |
| **Threaded IRQ** | Process (dedicated kthread) | **Yes** | Full process context | Modern preferred approach; integrates with RT scheduling |

```c
// Threaded IRQ — the modern approach:
// Top half: quick acknowledge
static irqreturn_t my_isr_top(int irq, void *dev_id) {
    // Read status register to check if our device interrupted
    if (!device_has_interrupt(dev_id))
        return IRQ_NONE;
    // Acknowledge the interrupt at hardware level
    ack_interrupt(dev_id);
    return IRQ_WAKE_THREAD;  // Schedule bottom half thread
}

// Bottom half: runs in process context, CAN sleep
static irqreturn_t my_isr_bottom(int irq, void *dev_id) {
    // Can do I2C reads, mutex_lock, kmalloc(GFP_KERNEL), etc.
    data = read_sensor_data(dev_id);
    process_data(data);
    return IRQ_HANDLED;
}

// Registration:
request_threaded_irq(irq, my_isr_top, my_isr_bottom,
                     IRQF_ONESHOT, "my_device", dev);
```

### 5.6 Nested Interrupts — **[MISSING]**

**Definition:** When a higher-priority interrupt preempts a currently executing ISR.

**How it works:**
- ARM GIC (Generic Interrupt Controller) supports interrupt priorities
- After acknowledging an IRQ and re-enabling interrupts in the ISR, a higher-priority IRQ can preempt
- Each nested interrupt pushes a new frame on the IRQ stack

**Risks:**
- Stack overflow (each nesting level uses stack space)
- Increased worst-case latency complexity
- Deadlocks if nested ISR tries to acquire a spinlock already held by the preempted ISR

**Embedded best practice:** Most RTOS and Linux drivers avoid arbitrary nesting. Linux uses IRQ priorities via the GIC but processes all at the same kernel level; threaded IRQs provide a safer model.

### 5.7 Interrupt Latency — **[MISSING]**

**Definition:** The time from when an interrupt is asserted to when the first instruction of the ISR executes.

**Components:**
```
Total latency = Recognition time (finish current instruction)
              + Pipeline flush
              + Context save (registers to stack)
              + Vector fetch + branch to handler
              + ISR prologue (compiler-generated register saves)
```

**Typical values:**
- Cortex-M (bare metal): 12–15 CPU cycles (deterministic)
- Cortex-A (Linux): 1–10 µs (variable due to IRQ-disabled sections)
- PREEMPT_RT Linux: ~50 µs worst case (with careful tuning)

**How to minimize:**
- Keep IRQ-disabled critical sections short
- Use threaded IRQs (reduces time spent in hardirq)
- Use `PREEMPT_RT` patch for Linux
- Pin ISR-servicing CPU core with `irq_set_affinity`

---

## 6. IPC Mechanisms 🟡

### 6.1 Summary Table

| Mechanism | Direction | Related processes? | Message boundaries? | Fastest? | Synchronization? | Persistence |
|-----------|-----------|-------------------|---------------------|----------|-------------------|-------------|
| Pipe | Unidirectional | Yes (parent-child) | No (byte stream) | No | Implicit (blocking) | Process |
| FIFO (Named Pipe) | Unidirectional | No | No (byte stream) | No | Implicit | File-system name persists |
| Message Queue | Bidirectional | No | **Yes** | No | Priority-ordered | Kernel-persistent |
| Shared Memory | N/A (direct access) | No | N/A | **Yes** (zero-copy) | **None** (you provide it) | Kernel-persistent |
| Signal | Asynchronous notification | No | N/A | N/A | N/A | N/A |
| Socket (Unix domain) | Bidirectional | No | Yes (SOCK_DGRAM) or No (SOCK_STREAM) | Near-shmem | Implicit | Process |

### 6.2 Pipes

**Definition:** Unidirectional byte-stream channel between related processes. Data written to one end is read from the other.

**Key details:**
- Capacity: 65,536 bytes (Linux ≥ 2.6.11)
- Atomic write guarantee: writes ≤ `PIPE_BUF` (4096 bytes) are atomic
- EOF: reader gets EOF (0 from `read()`) only when **all** write FDs are closed
- `SIGPIPE`: Writing to a pipe with no readers → `SIGPIPE` signal (default: kill writer)

**Embedded tradeoff:** Two kernel copies (user→kernel→user). Acceptable for control messages, not for high-bandwidth data streams.

### 6.3 FIFOs (Named Pipes)

Like pipes but with a filesystem name (`mkfifo`). Allows unrelated processes to communicate. Opening blocks until both reader and writer are present (rendezvous semantics).

### 6.4 Message Queues

**Advantages over pipes:**
- Message boundaries preserved (no framing needed)
- Priority-based retrieval
- Asynchronous notification (`mq_notify`)
- Unrelated processes can use them

```c
// Sender:
mqd_t mq = mq_open("/my_queue", O_WRONLY | O_CREAT, 0644, &attr);
mq_send(mq, "sensor:42", 9, priority);

// Receiver:
char buf[MAX_MSG_SIZE];
unsigned int prio;
mq_receive(mq, buf, MAX_MSG_SIZE, &prio);  // Gets highest priority msg first
```

**Embedded relevance:** Excellent for inter-task communication in RTOS (FreeRTOS `xQueueSend`/`xQueueReceive`). Fixed-size, priority-aware, ISR-safe (with `FromISR` variants in FreeRTOS).

### 6.5 Shared Memory

**Fastest IPC** — no data copying. Kernel sets up page table entries so multiple processes map to the same physical pages.

```c
// Setup:
int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
ftruncate(fd, SHM_SIZE);
void *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

// Use directly (but MUST synchronize with semaphore/mutex):
struct shared_data *data = (struct shared_data *)ptr;
sem_wait(&data->lock);
data->value = 42;
sem_post(&data->lock);
```

**Embedded tradeoff:** Maximum performance but maximum complexity. You're responsible for all synchronization.

### 6.6 Unix Domain Sockets — **[MISSING]**

**Definition:** Socket-based IPC for communication between processes on the same machine. Supports both stream (`SOCK_STREAM`) and datagram (`SOCK_DGRAM`) modes.

**Why embedded devs care:**
- Many embedded Linux daemons use Unix sockets (e.g., udev, D-Bus, systemd)
- Can pass file descriptors between processes via `SCM_RIGHTS` (powerful for device sharing)
- More feature-rich than pipes (bidirectional, multiplexable with `select`/`poll`/`epoll`)

**Embedded IPC decision flowchart:**
```
Need high bandwidth, zero copy?  → Shared Memory + semaphore
Need message boundaries + priority? → Message Queue
Simple parent→child data flow?   → Pipe
Need bidirectional + unrelated?   → Unix Domain Socket
Just a notification (no data)?   → Signal (or eventfd)
```

---

## 7. Signals 🟡

### 7.1 Signal Basics

**Definition:** A software interrupt delivered to a process to notify it of an event. Limited form of IPC — carries minimal information (just the signal number, optionally a value with real-time signals).

**Key signals for embedded:**

| Signal | Default | Meaning | Embedded relevance |
|--------|---------|---------|-------------------|
| `SIGINT` (2) | Term | Ctrl+C | Graceful shutdown trigger |
| `SIGKILL` (9) | Term | **Uncatchable** kill | Last resort; can't clean up |
| `SIGSEGV` (11) | Core | Invalid memory access | Dereferencing NULL, buffer overflow |
| `SIGPIPE` (13) | Term | Write to pipe with no reader | Common bug in socket programming |
| `SIGCHLD` (17) | Ign | Child process died | Reap zombies |
| `SIGSTOP` (19) | Stop | **Uncatchable** stop | Debugger control |
| `SIGALRM` (14) | Term | Alarm timer expired | Watchdog timers in user-space |
| `SIGUSR1/2` | Term | User-defined | Custom IPC in daemons |

### 7.2 Signal Handler Safety

> [!CAUTION]
> **Signal handlers are asynchronous — they can fire between ANY two instructions.** Only call **async-signal-safe** functions inside handlers.

**Safe:** `write()`, `read()`, `_exit()`, `signal()`, `waitpid()`, atomic operations
**Unsafe:** `printf()`, `malloc()`, `free()`, `pthread_mutex_lock()`, any function using global state

```c
// SAFE pattern:
volatile sig_atomic_t got_signal = 0;  // Atomic flag

void handler(int sig) {
    got_signal = 1;  // Just set a flag — minimal work
}

int main() {
    struct sigaction sa = { .sa_handler = handler, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &sa, NULL);
    
    while (!got_signal) {
        // Main work loop
        do_work();
    }
    // Clean shutdown here (in main context, not handler)
    cleanup();
}
```

### 7.3 Signals and Threads

- **Signal dispositions** (handlers) are **process-wide** — all threads share the same handler table
- **Signal masks** are **per-thread** — each thread can block different signals
- **Thread-directed signals:** Hardware faults (`SIGSEGV`) delivered to the causing thread
- **Process-directed signals:** `kill()` delivers to any unblocked thread

**Best practice for multithreaded embedded daemons:** Block all signals in all threads except one dedicated signal-handling thread that calls `sigwait()`.

### 7.4 Real-Time Signals

| Feature | Standard (1–31) | Real-Time (32–63) |
|---------|-----------------|-------------------|
| Queuing | No (multiple deliveries collapsed to one) | Yes (each instance queued) |
| Data payload | No | Yes (`sigqueue()` with `sigval`) |
| Delivery order | Undefined | Lowest number first |

---

## 8. RTOS-Specific Concepts 🔴

### 8.1 RTOS vs GPOS

| Aspect | RTOS | GPOS (Linux, Windows) |
|--------|------|----------------------|
| **Primary goal** | Meet timing deadlines | Maximize throughput/fairness |
| **Determinism** | Bounded worst-case latency | Best-effort, variable latency |
| **Scheduling** | Strict priority preemptive | CFS (fair), nice values |
| **Interrupt latency** | Deterministic, bounded (µs) | Variable (ms possible) |
| **Kernel preemptibility** | Fully preemptible | Partially (unless PREEMPT_RT) |
| **Memory model** | Flat physical (no MMU typically) | Virtual memory with MMU |
| **Examples** | FreeRTOS, Zephyr, QNX, VxWorks | Linux, Windows, macOS |

### 8.2 Hard vs Soft Real-Time

| Type | Deadline miss consequence | Example |
|------|--------------------------|---------|
| **Hard RT** | System failure, physical danger | Airbag deployment, pacemaker, ABS braking |
| **Firm RT** | Result is useless but no catastrophe | Video frame rendering (dropped frames visible but safe) |
| **Soft RT** | Degraded quality, acceptable | Audio streaming, UI responsiveness |

### 8.3 RTOS Task Scheduling

**FreeRTOS model (representative of most RTOS):**
- Fixed-priority preemptive scheduler
- Highest ready priority task always runs
- Equal-priority tasks use round-robin (configurable)
- No CFS, no nice values, no aging
- Task priorities are static (set at creation, rarely changed at runtime)

```c
// FreeRTOS task creation:
void sensor_task(void *params) {
    while (1) {
        read_sensor();
        vTaskDelay(pdMS_TO_TICKS(100));  // Yield CPU for 100ms
    }
}

xTaskCreate(sensor_task, "Sensor", 256, NULL, 
            3,      // Priority (higher number = higher priority in FreeRTOS)
            NULL);
```

### 8.4 Bounded Latency — **[MISSING]**

**Definition:** The guarantee that any event (interrupt, task switch) will be handled within a known worst-case time bound. This is the defining property of a real-time system.

**What RTOS guarantees (that GPOS doesn't):**
1. Worst-case interrupt latency is bounded and documented
2. Worst-case task switch time is bounded
3. All kernel operations have bounded execution time (no unbounded loops)
4. No priority inversion (PI protocols enforced)

**Why GPOS fails here:**
- Linux kernel can disable interrupts for unbounded durations (spinlock-protected sections)
- Memory allocation can trigger page faults (disk I/O → unbounded)
- CFS doesn't guarantee when a task will run

### 8.5 RTOS Synchronization Primitives

| Primitive | FreeRTOS API | ISR-safe variant |
|-----------|-------------|-----------------|
| Mutex | `xSemaphoreCreateMutex()` | **NO** (ISR can't take mutex) |
| Binary Semaphore | `xSemaphoreCreateBinary()` | `xSemaphoreGiveFromISR()` |
| Counting Semaphore | `xSemaphoreCreateCounting()` | `xSemaphoreGiveFromISR()` |
| Queue | `xQueueCreate()` | `xQueueSendFromISR()` |
| Event Group | `xEventGroupCreate()` | `xEventGroupSetBitsFromISR()` |
| Task Notification | Built into each task | `xTaskNotifyFromISR()` |

> [!WARNING]
> **FreeRTOS gotcha:** `xSemaphoreGive()` (mutex version) can only be called from the task that took it. Using a mutex for ISR-to-task signaling will corrupt the mutex state. Use a **binary semaphore** instead for ISR signaling.

### 8.6 PREEMPT_RT Linux — **[MISSING]**

**Definition:** A patch set (now being merged into mainline Linux) that makes the Linux kernel fully preemptible, converting it from a GPOS into a soft real-time OS.

**Key changes:**
- Converts spinlocks to sleeping RT-mutexes (preemptible)
- Converts softirqs and hardirqs to kernel threads (schedulable, with priorities)
- Implements priority inheritance across the kernel
- Reduces maximum non-preemptible critical sections

**When to use:** When you need real-time guarantees but also need Linux's device driver ecosystem, networking stack, and filesystem support.

**Typical latencies:**
- Standard Linux: ~100µs–10ms worst case
- PREEMPT_RT Linux: ~30–100µs worst case
- Dedicated RTOS: <10µs worst case

---

## 9. Boot Process 🟡

### 9.1 ARM Embedded Linux Boot Flow

```
┌─────────────────────────────────────────────────────────┐
│ 1. POWER ON / RESET                                     │
│    CPU starts at fixed Reset Vector (e.g., 0xFFFF0000)  │
│    Executes code from on-chip ROM (BootROM)             │
└────────────────────┬────────────────────────────────────┘
                     ↓
┌─────────────────────────────────────────────────────────┐
│ 2. ROM BOOTLOADER (Primary/BootROM)                     │
│    - Silicon vendor's immutable code                    │
│    - Initializes minimal hardware (clocks, SRAM)        │
│    - Reads boot medium (eMMC, SD, SPI NOR, USB)         │
│    - Loads SPL/MLO into on-chip SRAM                    │
│    - May verify signature (Secure Boot chain)           │
└────────────────────┬────────────────────────────────────┘
                     ↓
┌─────────────────────────────────────────────────────────┐
│ 3. SPL (Secondary Program Loader) / MLO                 │
│    - First user-modifiable code                         │
│    - Initializes DDR RAM (complex PHY training)         │
│    - Loads full U-Boot into DDR                         │
└────────────────────┬────────────────────────────────────┘
                     ↓
┌─────────────────────────────────────────────────────────┐
│ 4. U-BOOT (Full Bootloader)                             │
│    - Initializes remaining peripherals                  │
│    - Provides shell for debugging (serial console)      │
│    - Loads kernel image (zImage/Image) into RAM         │
│    - Loads Device Tree Blob (.dtb) into RAM             │
│    - Sets kernel command line (bootargs)                │
│    - Jumps to kernel entry point                        │
└────────────────────┬────────────────────────────────────┘
                     ↓
┌─────────────────────────────────────────────────────────┐
│ 5. LINUX KERNEL INITIALIZATION                          │
│    - Decompresses itself (if zImage)                    │
│    - Sets up page tables, enables MMU                   │
│    - Initializes interrupt controller (GIC)             │
│    - Parses Device Tree → probes drivers                │
│    - Mounts root filesystem                             │
│    - Executes first user-space process: /sbin/init      │
└────────────────────┬────────────────────────────────────┘
                     ↓
┌─────────────────────────────────────────────────────────┐
│ 6. USER SPACE (init/systemd)                            │
│    - Mounts filesystems, starts services                │
│    - Launches the application                           │
└─────────────────────────────────────────────────────────┘
```

### 9.2 Device Tree

**Definition:** A data structure (`.dts` source → `.dtb` compiled binary) that describes the hardware topology to the kernel. Eliminates the need for hard-coded hardware descriptions in the kernel.

```dts
// Example: describing a UART
uart0: serial@4000C000 {
    compatible = "arm,pl011";         // Kernel matches this to a driver
    reg = <0x4000C000 0x1000>;        // Base address + size
    interrupts = <0 33 0x4>;          // SPI interrupt 33, level-triggered
    clocks = <&uart_clk>;
    status = "okay";                  // "disabled" to turn off
};
```

**Why it matters:** The `compatible` string is how the kernel finds the right driver. In `platform_driver`, you define an `of_match_table` with compatible strings.

### 9.3 Secure Boot — **[MISSING]**

**Definition:** A cryptographic chain of trust ensuring only authenticated code runs at each boot stage.

```
Root of Trust (ROM) → verifies → SPL (signed)
SPL → verifies → U-Boot (signed)
U-Boot → verifies → Kernel (signed)
Kernel → verifies → rootfs / modules (optional)
```

**Embedded relevance:** Required for medical devices, automotive, and IoT products where firmware tampering is a security threat.

**Common interview question:** "What happens if verification fails at any stage?" → The boot halts. The device does not execute untrusted code. Some implementations fall back to a recovery partition.

---

## 10. Linux Kernel Concurrency Primitives 🔴

### 10.1 Atomic Operations

**Definition:** Operations that complete in a single, indivisible step with respect to other CPUs and interrupts. Implemented via special CPU instructions (`LDREX/STREX` on ARM, `LOCK CMPXCHG` on x86).

```c
// Linux kernel atomic API:
atomic_t counter = ATOMIC_INIT(0);

atomic_inc(&counter);           // counter++, atomically
atomic_dec_and_test(&counter);  // counter--, return true if result is 0
atomic_read(&counter);          // Read current value
atomic_set(&counter, 42);       // Set to 42
atomic_cmpxchg(&counter, old, new);  // CAS: if counter==old, set to new
```

**When to use:** Simple counters, flags, reference counts. Not sufficient for protecting multi-variable data structures (need locks for those).

### 10.2 Memory Barriers

**Problem:** Both the **compiler** and the **CPU** can reorder memory operations for performance. This is invisible on a single CPU but breaks multi-CPU synchronization.

```c
// Without barriers, this pattern is BROKEN on SMP:
// Thread A (producer):              // Thread B (consumer):
data = 42;                           while (!flag) ;
flag = 1;                            use(data);
// CPU might reorder: flag=1 BEFORE data=42!
// Thread B could see flag=1 but read stale data!

// FIX with memory barriers:
data = 42;
smp_wmb();   // Write Memory Barrier: all writes before this complete before writes after
flag = 1;
                                      while (!flag) ;
                                      smp_rmb();   // Read Memory Barrier
                                      use(data);
```

**Types of barriers in Linux kernel:**

| Barrier | Effect |
|---------|--------|
| `barrier()` | Compiler-only barrier (prevents compiler reordering, not CPU) |
| `smp_mb()` | Full memory barrier (both reads and writes ordered) |
| `smp_rmb()` | Read barrier (read ordering only) |
| `smp_wmb()` | Write barrier (write ordering only) |
| `smp_store_release()` | Store with release semantics |
| `smp_load_acquire()` | Load with acquire semantics |

**Embedded relevance:** Essential when implementing lock-free algorithms, device register access, and DMA buffer handoff. `readl()`/`writel()` include barriers; `__raw_readl()`/`__raw_writel()` do not.

### 10.3 RCU (Read-Copy-Update) — **[MISSING]**

**Definition:** A synchronization mechanism optimized for read-mostly data structures. Readers access shared data **without any locks or atomic instructions** — they are essentially free. Writers create a modified copy and atomically swap the pointer; old data is freed only after all current readers have finished (a "grace period").

```c
// Reader (zero overhead!):
rcu_read_lock();                    // Just disables preemption
struct data *p = rcu_dereference(global_ptr);  // Read pointer with barrier
use(p->field);
rcu_read_unlock();

// Writer (does the heavy lifting):
struct data *new = kmalloc(...);    // Allocate new version
struct data *old = global_ptr;
*new = *old;                        // Copy old data
new->field = new_value;             // Modify
rcu_assign_pointer(global_ptr, new);  // Atomically publish new version
synchronize_rcu();                  // Wait for all readers of old version to finish
kfree(old);                         // Safe to free now
```

**When to use:** Kernel routing tables, module lists, anything read far more often than written.

**Interview question:** "Why not just use a rwlock?" → RCU readers have zero overhead (no cache line bouncing, no atomic instructions). Reader-writer locks still require atomic operations that cause cache contention on SMP systems. RCU scales perfectly with number of reader CPUs.

### 10.4 Per-CPU Variables — **[MISSING]**

**Definition:** Variables where each CPU core has its own private copy. No locking needed because only one CPU accesses each copy.

```c
// Declaration:
DEFINE_PER_CPU(unsigned long, my_counter);

// Access (must disable preemption to stay on the same CPU):
preempt_disable();
this_cpu_inc(my_counter);  // Increment this CPU's copy — no locks!
unsigned long val = this_cpu_read(my_counter);
preempt_enable();
```

**When to use:** Statistics counters, per-CPU caches, per-CPU run queues. Perfect when each CPU works independently and results are aggregated periodically.

### 10.5 Seqlocks — **[MISSING]**

**Definition:** A reader-writer mechanism where writers are never blocked by readers. Readers detect if a write occurred during their read and retry.

```c
// Writer:
write_seqlock(&my_seqlock);
// modify data
write_sequnlock(&my_seqlock);

// Reader (may need to retry):
unsigned seq;
do {
    seq = read_seqbegin(&my_seqlock);
    // read data (might be inconsistent if writer is active)
} while (read_seqretry(&my_seqlock, seq));  // Retry if writer was active
```

**When to use:** Data written rarely but read frequently, and reads can be retried cheaply. Classic example: `jiffies` (system tick counter), `xtime` (system time).

### 10.6 Completions — **[MISSING]**

**Definition:** A simple signaling mechanism for one thread to tell another "I'm done." Like a one-shot condition variable optimized for the common "wait for initialization/completion" pattern.

```c
DECLARE_COMPLETION(my_init_done);

// Thread A (waiter):
wait_for_completion(&my_init_done);  // Blocks until complete

// Thread B (signaler):
// ... do initialization work ...
complete(&my_init_done);             // Wake up waiter
```

**Embedded use:** Driver probe functions waiting for firmware load, DMA completion, hardware initialization.

---

## 11. Linux Device Driver Architecture 🔴

### 11.1 "Everything is a File"

In Linux, devices are accessed through file operations. User-space programs use standard `open()`, `read()`, `write()`, `ioctl()`, `close()` system calls.

### 11.2 Character vs Block Devices

| Aspect | Character Device | Block Device |
|--------|-----------------|--------------|
| Data access | Stream of bytes | Fixed-size blocks (sectors) |
| Seeking | Usually not supported | Supported |
| Buffering | Usually unbuffered | Kernel buffer cache |
| Examples | Serial port, GPIO, sensors | SD card, eMMC, hard drive |
| Driver struct | `struct file_operations` | `struct block_device_operations` |

### 11.3 `file_operations` Structure

```c
static struct file_operations my_fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .read    = my_read,       // copy_to_user() here
    .write   = my_write,      // copy_from_user() here
    .unlocked_ioctl = my_ioctl,  // Custom commands
    .release = my_close,
    .mmap    = my_mmap,       // Memory-map device registers to user-space
    .poll    = my_poll,       // select/poll/epoll support
};
```

### 11.4 Major and Minor Numbers

- **Major number:** Identifies the driver (e.g., 4 = TTY driver)
- **Minor number:** Identifies the specific device instance (e.g., ttyS0, ttyS1)

```c
// Dynamic allocation (preferred):
int major;
major = register_chrdev(0, "my_device", &my_fops);  // 0 = auto-assign major
```

### 11.5 `copy_to_user()` / `copy_from_user()`

**Why not use `memcpy()`?** Kernel and user-space have different address spaces. Direct pointer dereference could:
1. Access invalid user-space address → kernel oops
2. Touch a swapped-out page → page fault in kernel context (needs careful handling)

```c
// In read handler:
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
    char kernel_buf[64] = "Hello from kernel\n";
    size_t to_copy = min(len, sizeof(kernel_buf));
    
    if (copy_to_user(buf, kernel_buf, to_copy))
        return -EFAULT;  // Bad user-space pointer
    
    return to_copy;
}
```

### 11.6 Kernel Modules (LKM)

```c
#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void) {
    pr_info("Module loaded\n");
    return 0;  // 0 = success
}

static void __exit my_exit(void) {
    pr_info("Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
```

| Command | Purpose |
|---------|---------|
| `insmod` | Load module (no dependency resolution) |
| `modprobe` | Load module + resolve dependencies |
| `rmmod` | Unload module |
| `lsmod` | List loaded modules |
| `dmesg` | View kernel log (see `printk`/`pr_info` output) |

---

## 12. File Systems 🟢

> [!NOTE]
> File systems are rarely grilled in embedded interviews beyond the basics below. Focus your time on synchronization, interrupts, and memory management instead.

### 12.1 VFS (Virtual File System) — **[MISSING]**

**Definition:** An abstraction layer in the Linux kernel that provides a uniform interface (`open`, `read`, `write`, etc.) for all file systems (ext4, FAT, NFS, procfs, sysfs, etc.).

**Key objects:**
| Object | Represents | Key fields |
|--------|-----------|------------|
| `superblock` | A mounted filesystem | Block size, inode count, FS type |
| `inode` | A file on disk (metadata) | Permissions, size, block pointers, timestamps |
| `dentry` | A directory entry (name→inode mapping) | Name string, parent, inode pointer |
| `file` | An open file (per-process) | Current offset, flags, `file_operations` |

### 12.2 Inode

**Definition:** An index node storing a file's metadata (owner, permissions, timestamps, block pointers). Does NOT store the filename — that's in the directory entry.

**Interview question:** "Can two filenames point to the same inode?" → Yes. That's a **hard link**. The inode has a link count; the file is deleted only when all hard links are removed.

### 12.3 Journaling — **[MISSING]**

**Definition:** A technique where the filesystem writes metadata changes (and optionally data) to a journal (log) before committing them to disk. If a crash occurs mid-write, the journal can be replayed to restore consistency.

**Types:**
| Mode | Journals | Speed | Safety |
|------|----------|-------|--------|
| Journal (full) | Metadata + data | Slowest | Safest |
| Ordered (default ext4) | Metadata only, data written first | Medium | Good |
| Writeback | Metadata only, data order uncontrolled | Fastest | Data may be stale after crash |

### 12.4 Embedded-Relevant File Systems — **[MISSING]**

| Filesystem | Type | Use case |
|------------|------|----------|
| ext4 | Block (journaling) | eMMC/SD root filesystem |
| FAT/FAT32 | Block (no journal) | SD cards, USB, cross-platform compatibility |
| JFFS2 | Flash (log-structured) | NOR flash, wear leveling built-in |
| UBIFS | Flash (on top of UBI) | NAND flash, modern replacement for JFFS2 |
| SquashFS | Read-only compressed | Root filesystem for space-constrained devices |
| tmpfs | RAM-based | `/tmp`, `/run` — fast, no flash wear |
| procfs | Virtual | `/proc` — kernel/process info interface |
| sysfs | Virtual | `/sys` — device/driver model interface |
| devtmpfs | Virtual | `/dev` — device nodes auto-created |

**Interview question:** "Why not use ext4 directly on raw NAND flash?" → NAND flash has erase-before-write semantics, wear leveling requirements, and bad block management needs. ext4 doesn't handle these. You need a flash-aware filesystem (UBIFS) or a Flash Translation Layer (FTL).

---

## 13. Embedded-Specific Topics 🟡

### 13.1 `volatile` Keyword

**Definition:** Tells the compiler that a variable's value may change at any time (by hardware, ISR, or another thread) and must not be optimized away. Every read/write must actually access memory.

```c
// WITHOUT volatile — compiler optimizes away repeated reads:
uint32_t *status = (uint32_t *)0x4000C000;
while (*status & 0x01) {}  // Compiler: "status never changes, skip the loop"

// WITH volatile — compiler generates actual memory reads each iteration:
volatile uint32_t *status = (volatile uint32_t *)0x4000C000;
while (*status & 0x01) {}  // Actually reads the register each time
```

**When to use `volatile`:**
1. Hardware registers (MMIO)
2. Variables shared between ISR and main code
3. Variables shared between threads (though `volatile` alone is NOT sufficient for thread safety — you need atomics or locks too)

> [!WARNING]
> **Common misconception:** `volatile` does NOT provide atomicity or ordering guarantees. `volatile int x; x++;` is still a non-atomic read-modify-write.

### 13.2 Watchdog Timer (WDT)

**Definition:** A hardware countdown timer that resets the system if software fails to "kick" (reset) it within a configured interval.

```c
// Simplified watchdog pattern:
void main_loop(void) {
    watchdog_init(5000);  // 5-second timeout
    
    while (1) {
        process_sensor_data();
        handle_communication();
        watchdog_kick();  // Reset countdown — proves software is alive
    }
    // If main_loop hangs → WDT expires → system resets
}
```

**Windowed Watchdog:** Kick must happen within a window (not too early, not too late). Catches both hangs AND runaway fast loops.

### 13.3 Power Management

| Mechanism | What it does | Latency tradeoff |
|-----------|-------------|-------------------|
| DVFS | Scale CPU frequency/voltage with load | Minimal |
| C-States (Idle) | CPU enters low-power idle modes | Deeper = more savings but longer wakeup |
| S3 (Suspend to RAM) | System sleeps; RAM stays powered | ~seconds to wake |
| S4 (Hibernate) | Context saved to disk; all power off | ~10+ seconds to wake |

### 13.4 OTA Firmware Updates

**A/B Partitioning:**
```
┌─────────────┐  ┌─────────────┐
│  Slot A     │  │  Slot B     │
│ (Active)    │  │ (Update)    │
│  Kernel A   │  │  Kernel B   │  ← New firmware written here
│  RootFS A   │  │  RootFS B   │
└─────────────┘  └─────────────┘

After verification → bootloader marks Slot B as active → reboot
If Slot B fails → bootloader falls back to Slot A (automatic rollback)
```

### 13.5 Secure Boot & TEE

- **Secure Boot:** Cryptographic chain of trust from ROM → bootloader → kernel → rootfs
- **ARM TrustZone:** Hardware isolation dividing CPU into "Secure World" and "Normal World"
- **TEE (Trusted Execution Environment):** Runs security-critical code (key management, DRM) in the Secure World, isolated from Linux

### 13.6 Linker Scripts — **[MISSING]**

**Definition:** In bare-metal embedded, the linker script (`.ld` file) tells the linker where to place each section (`.text`, `.data`, `.bss`, `.rodata`) in memory.

```ld
/* Simplified ARM linker script */
MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS {
    .text : { *(.text*) } > FLASH
    .rodata : { *(.rodata*) } > FLASH
    .data : { *(.data*) } > SRAM AT > FLASH   /* Stored in FLASH, copied to SRAM at startup */
    .bss : { *(.bss*) } > SRAM                /* Zero-initialized at startup */
}
```

**Interview question:** "What's the difference between the load address (LMA) and the virtual/run address (VMA)?" → `.data` is stored in FLASH (LMA) but runs from SRAM (VMA). The startup code (`Reset_Handler`) copies `.data` from FLASH to SRAM before `main()`.

### 13.7 Memory-Mapped Registers and Bit Manipulation — **[MISSING]**

```c
// Common patterns in embedded:

// 1. Define register addresses:
#define GPIO_BASE    0x40020000
#define GPIO_ODR     (*(volatile uint32_t *)(GPIO_BASE + 0x14))

// 2. Set bit (turn on pin 5):
GPIO_ODR |= (1 << 5);

// 3. Clear bit (turn off pin 5):
GPIO_ODR &= ~(1 << 5);

// 4. Toggle bit:
GPIO_ODR ^= (1 << 5);

// 5. Check bit:
if (GPIO_IDR & (1 << 5)) { /* pin is high */ }

// 6. Set a multi-bit field (bits 7:4 to value 0xA):
REG = (REG & ~(0xF << 4)) | (0xA << 4);
```

### 13.8 Endianness — **[MISSING]**

| Type | Byte order | Example: storing 0x12345678 | Used by |
|------|------------|---------------------------|---------|
| Big-endian | MSB first | `12 34 56 78` | Network protocols, Motorola, some ARM modes |
| Little-endian | LSB first | `78 56 34 12` | x86, ARM (default), RISC-V |

**Embedded relevance:** Network byte order is big-endian. ARM is little-endian by default. When receiving network packets, you must convert:
```c
uint32_t host_val = ntohl(network_val);  // Network to Host Long
uint16_t host_val = ntohs(network_val);  // Network to Host Short
```

---

## Appendix: Gap Analysis — What Was Missing From Your Notes

The following topics were **completely absent** from your original notes and have been added above:

| Topic | Section | Importance |
|-------|---------|------------|
| `clone()` system call | 1.5 | 🔴 Deep |
| Multilevel Feedback Queue | 2.2 | 🟢 Surface |
| CFS (Completely Fair Scheduler) | 2.3 | 🟡 Solid |
| CPU Affinity / `isolcpus` | 2.5 | 🟡 Solid |
| Reader-Writer Locks | 3.8 | 🟡 Solid |
| MMU vs MPU | 4.6 | 🔴 Deep |
| Virtual Memory (the concept itself) | 4.7 | 🟡 Solid |
| Memory-Mapped I/O vs Port I/O | 4.11 | 🔴 Deep |
| Slab Allocator | 4.12 | 🟡 Solid |
| Nested Interrupts | 5.6 | 🟡 Solid |
| Interrupt Latency | 5.7 | 🔴 Deep |
| Unix Domain Sockets | 6.6 | 🟡 Solid |
| Bounded Latency | 8.4 | 🔴 Deep |
| PREEMPT_RT Linux | 8.6 | 🔴 Deep |
| RCU (Read-Copy-Update) | 10.3 | 🔴 Deep |
| Per-CPU Variables | 10.4 | 🟡 Solid |
| Seqlocks | 10.5 | 🟡 Solid |
| Completions | 10.6 | 🟡 Solid |
| VFS | 12.1 | 🟢 Surface |
| Journaling | 12.3 | 🟢 Surface |
| Embedded File Systems (UBIFS, SquashFS, etc.) | 12.4 | 🟡 Solid |
| Linker Scripts | 13.6 | 🟡 Solid |
| Bit Manipulation Patterns | 13.7 | 🟡 Solid |
| Endianness | 13.8 | 🟡 Solid |
| Secure Boot (expanded) | 9.3 | 🟡 Solid |

---

> [!TIP]
> **Study priority for limited time:** Focus on sections marked 🔴 first: **Synchronization (§3)**, **Memory Management (§4)**, **Interrupts (§5)**, **Kernel Concurrency (§10)**, and **Device Drivers (§11)**. These make or break embedded interviews.
