# Part 6: Interrupts and RTOS

---

## Q1: What is an Interrupt? Working Flow

An interrupt is an electrical signal sent to the processor from an internal or external hardware source that temporarily suspends the processor's current execution thread so it can execute a specialized function called an Interrupt Service Routine (ISR).

### Detailed Step-by-Step Flow

```
   [ CPU executing normal code ] ──► [ Interrupt Triggered ]
                                               │
                                               ▼
                                     [ Finish Current Instr ]
                                               │
                                               ▼
                                      [ Save Context ]
                                 (Push registers to Stack)
                                               │
                                               ▼
                                    [ Look up Vector Table ]
                                     (Get address of ISR)
                                               │
                                               ▼
                                      [ Execute ISR ]
                                               │
                                               ▼
                                      [ Restore Context ]
                                (Pop registers from Stack)
                                               │
                                               ▼
                                 [ Resume Main Execution ]
```

1. **Interrupt Source Trigger:** Hardware line goes high/low (edge or level triggered).
2. **Instruction Completion:** The CPU finishes execution of the current assembly instruction.
3. **Context Save:** The CPU hardware/firmware automatically pushes core registers (like Program Counter, Status Register, general-purpose registers) onto the stack.
4. **ISR Lookup:** The CPU reads the vector table to find the address of the corresponding ISR.
5. **ISR Execution:** The CPU jumps to the ISR address and runs the code.
6. **Context Restore:** Upon hitting the special return instruction (e.g., `bx lr` or `iret`), registers are popped back from the stack.
7. **Resume Program:** The CPU returns to the exact address it was executing prior to the interrupt.

---

## Q2: Interrupt Vector Table (IVT)

The Interrupt Vector Table is an array of memory addresses located at a fixed location in memory (usually at address `0x00000000` or offset dynamically via a Vector Table Offset Register - VTOR). Each entry (vector) points to the start address of its corresponding ISR handler.

### Memory Layout (Example: ARM Cortex-M)
```
  Address        Vector Entry        Description
  +------------+--------------------+-----------------------------+
  | 0x00000000 | Initial MSP        | Stack Pointer starting value|
  +------------+--------------------+-----------------------------+
  | 0x00000004 | Reset Handler      | Jump point on startup/reset |
  +------------+--------------------+-----------------------------+
  | 0x00000008 | NMI Handler        | Non-Maskable Interrupt      |
  +------------+--------------------+-----------------------------+
  | 0x0000000C | HardFault Handler  | CPU Fault / Crash Handler   |
  +------------+--------------------+-----------------------------+
  | 0x000000XX | External IRQ0      | GPIO / UART / SPI interrupt |
  +------------+--------------------+-----------------------------+
```

---

## Q3: ISR Rules and Best Practices

### Rules for Writing an ISR
1. **Keep it short and fast:** Do not block. Execute minimum necessary code (e.g., read a register, place in queue).
2. **No blocking calls:** Do not use `delay()`, sleep, or wait for mutexes inside an ISR.
3. **No dynamic memory allocation:** Avoid `malloc` / `free` because they are non-deterministic and can lock the heap.
4. **Use `volatile` for shared variables:** Any variable modified in the ISR and read in the main loop must be declared `volatile` to prevent compiler register-caching optimizations.
5. **No heavy I/O operations:** Avoid calling `printf` or writing to files.
6. **Top-Half / Bottom-Half processing:** Perform critical work (top-half) in the ISR, then defer long-running computations (bottom-half) to a background thread or RTOS task using a queue or semaphore.

### Can an interrupt exist without an ISR?
Yes. If an interrupt is enabled but no ISR address is specified in the vector table, triggering it will load a null/garbage address into the Program Counter, leading to a CPU crash (HardFault or reset). To prevent this, standard templates point unused vectors to a default dummy handler containing a loop.

---

## Q4: Maskable vs Non-Maskable Interrupts (NMI)

- **Maskable Interrupts:** Interrupts that can be disabled (masked) by the CPU by clearing a bit in the interrupt mask register (e.g., standard timer or GPIO interrupts).
- **Non-Maskable Interrupts (NMI):** Interrupts that cannot be disabled by software. They have the highest priority and must be serviced immediately.
  - **Examples:** Power failure warning, hardware watchdog reset, ECC memory errors.

---

## Q5: Nested Interrupts (Preemption)

Nested interrupts allow a higher-priority interrupt to interrupt (preempt) a currently running lower-priority ISR.

If two interrupts are waiting and one is executing:
- If a new interrupt has **higher priority** than the executing one, it preempts the current ISR.
- If the new interrupt has **lower or equal priority**, it is set to **pending** and executes after the current ISR finishes.

---

## Q6: Interrupt Latency

Interrupt latency is the time delay between the hardware trigger of the interrupt and the execution of the first instruction in the ISR.

```
IR Line:  ───┐
             └───────────────────────────────────────
             ^ Interrupt Triggered
               |<-------- Latency -------->|
CPU Exec: ──[Normal Instr]──[Context Save]──[Start of ISR]
```

### How to measure it?
1. **Oscilloscope:** Toggle a GPIO pin at the start of the ISR. Measure the time delay from the hardware trigger event to the GPIO toggle using an oscilloscope.
2. **Hardware Timers:** Capture a free-running timer register on trigger, and subtract that value from the timer value at the start of the ISR.

---

## Q7: Interrupt Coalescing

### Answer
Interrupt coalescing is a technique where a hardware peripheral (like a network interface card - NIC) buffers multiple incoming events and generates a single interrupt for all of them, rather than triggering an interrupt for every single event. This prevents the CPU from being overwhelmed by interrupts (interrupt storm).

---

## Q8: OS vs RTOS

| Feature               | General Purpose OS (Windows/Linux)      | RTOS (FreeRTOS, VxWorks)              |
|-----------------------|-----------------------------------------|---------------------------------------|
| **Determinism**       | Non-deterministic (Best effort latency) | Deterministic (Strict latency limits) |
| **Scheduling**        | Fairness-based (Time slicing sharing)   | Priority-based preemptive scheduling  |
| **Footprint**         | Large (Gigabytes)                       | Extremely small (Kilobytes)           |
| **Interrupt Latency** | Variable                                | Minimal and bounded                   |

---

## Q9: Preemptive Scheduling preemption flow

In a preemptive RTOS, the scheduler guarantees that the highest-priority task ready to run is always allocated CPU execution.

```
Priority 4:  (High)                   [Task 4 Runs]
Priority 3:            [Task 3 Runs]────────────────► (Preempted)
Priority 2: [Task 2] ────────────────► (Preempted)
Priority 1:  (Low)
```

1. **Preemption Event:** If Task 2 (Priority 2) is running and Task 3 (Priority 3) changes state from Blocked to Ready (e.g., timer expires or semaphore released):
2. **Context Switch:** The scheduler runs immediately, saves Task 2's stack, loads Task 3's stack, and resumes execution of Task 3. Task 2 is preempted.

---

## Q10: Priority Inversion and Priority Inheritance

Priority inversion is a bug where a high-priority task is blocked waiting for a resource held by a low-priority task, while a medium-priority task preempts the low-priority task, indirectly delaying the high-priority task.

```
Priority Inversion Scenario:
1. Low Task (P1) grabs Mutex M.
2. High Task (P3) wakes up, tries to grab Mutex M, blocks.
3. Medium Task (P2) wakes up and preempts Low Task (P1).
Result: High Task (P3) is blocked by Medium Task (P2) indefinitely!
```

### Solution: Priority Inheritance
When High Task (P3) blocks on the mutex held by Low Task (P1), the system temporarily raises P1's priority to P3. This prevents the Medium Task (P2) from preempting P1, allowing P1 to release the mutex quickly. Once released, P1's priority drops back to normal.

---

## Q11: RTOS Synchronization Techniques

### 1. Mutex (Mutual Exclusion)
- Used to protect shared resources (critical sections).
- Has an owner (only the thread that locked it can unlock it).
- Implements **priority inheritance** to prevent priority inversion.

### 2. Semaphores
- **Binary Semaphore:** Used for signaling (one task signals another). Has no owner concept and no priority inheritance.
- **Counting Semaphore:** Tracks resource counts (e.g., number of items in a queue).

### 3. Spinlock vs Mutex
- **Mutex:** Puts the waiting thread to sleep (blocked state), context switching to another thread. Good for long wait periods.
- **Spinlock:** Keeps the CPU busy-waiting in a loop checking the lock. Good for short wait periods, and mandatory inside ISR context where sleeping is illegal.

---

## Q12: Deadlocks

A deadlock occurs when two or more threads are unable to proceed because each is waiting for the other to release a resource.

```
Thread A: Locks Mutex 1, waits for Mutex 2.
Thread B: Locks Mutex 2, waits for Mutex 1.
```

### How to avoid:
1. **Lock ordering:** Always acquire resources in the same order across all threads.
2. **Timeout:** Use lock acquisition calls that timeout rather than blocking indefinitely (e.g., `pthread_mutex_trylock`).


## Q14: Static Variable inside an RTOS Task

### Answer
A static variable defined inside an RTOS task function behaves like a global static variable. It is stored in the global **`.data`** or **`.bss`** memory segment, NOT on the task's stack.
- **Trap:** Since the variable is shared across the global segment, if you run multiple instances of the same task function, they will all share and modify the exact same static variable instance, leading to data corruption.
