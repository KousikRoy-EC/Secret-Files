
------>     Schedule Algorithms in C    <------

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Structure to represent a process
typedef struct {
    int pid;          // Process ID
    int arrival_time; // Arrival time
    int burst_time;   // Burst time
    int priority;     // Priority (lower number = higher priority)
    int waiting_time; // Waiting time
    int turnaround_time; // Turnaround time
    int completion_time; // Completion time
    int remaining_time;  // Remaining time (for preemptive algorithms)
} Process;

// Function to calculate and display results
void calculate_times(Process processes[], int n) {
    int total_waiting = 0, total_turnaround = 0;
    
    printf("\nPID\tArrival\tBurst\tWaiting\tTurnaround\tCompletion\n");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n", 
               processes[i].pid, processes[i].arrival_time, 
               processes[i].burst_time, processes[i].waiting_time,
               processes[i].turnaround_time, processes[i].completion_time);
    }
    
    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / n);
}

// 1. FIRST COME FIRST SERVE (FCFS)
void fcfs(Process processes[], int n) {
    printf("\n=== FIRST COME FIRST SERVE (FCFS) ===\n");
    
    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    
    int current_time = 0;
    
    for (int i = 0; i < n; i++) {
        // If current time is less than arrival time, wait
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        
        current_time = processes[i].completion_time;
    }
    
    calculate_times(processes, n);
}

// 2. SHORTEST JOB FIRST (SJF) - Non-preemptive
void sjf_non_preemptive(Process processes[], int n) {
    printf("\n=== SHORTEST JOB FIRST (Non-Preemptive) ===\n");
    
    int completed = 0, current_time = 0;
    int is_completed[n];
    memset(is_completed, 0, sizeof(is_completed));
    
    while (completed != n) {
        int shortest = -1;
        int min_burst = INT_MAX;
        
        // Find the shortest job among arrived processes
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    shortest = i;
                }
            }
        }
        
        if (shortest == -1) {
            current_time++;
            continue;
        }
        
        processes[shortest].waiting_time = current_time - processes[shortest].arrival_time;
        processes[shortest].completion_time = current_time + processes[shortest].burst_time;
        processes[shortest].turnaround_time = processes[shortest].completion_time - processes[shortest].arrival_time;
        
        current_time = processes[shortest].completion_time;
        is_completed[shortest] = 1;
        completed++;
    }
    
    calculate_times(processes, n);
}

// 3. SHORTEST REMAINING TIME FIRST (SRTF) - Preemptive SJF
void srtf(Process processes[], int n) {
    printf("\n=== SHORTEST REMAINING TIME FIRST (SRTF) ===\n");
    
    // Initialize remaining times
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    int completed = 0, current_time = 0;
    int last_process = -1;
    
    while (completed != n) {
        int shortest = -1;
        int min_remaining = INT_MAX;
        
        // Find process with shortest remaining time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && 
                processes[i].remaining_time > 0 && 
                processes[i].remaining_time < min_remaining) {
                min_remaining = processes[i].remaining_time;
                shortest = i;
            }
        }
        
        if (shortest == -1) {
            current_time++;
            continue;
        }
        
        processes[shortest].remaining_time--;
        current_time++;
        
        if (processes[shortest].remaining_time == 0) {
            processes[shortest].completion_time = current_time;
            processes[shortest].turnaround_time = processes[shortest].completion_time - processes[shortest].arrival_time;
            processes[shortest].waiting_time = processes[shortest].turnaround_time - processes[shortest].burst_time;
            completed++;
        }
    }
    
    calculate_times(processes, n);
}

// 4. PRIORITY SCHEDULING - Non-preemptive
void priority_non_preemptive(Process processes[], int n) {
    printf("\n=== PRIORITY SCHEDULING (Non-Preemptive) ===\n");
    
    int completed = 0, current_time = 0;
    int is_completed[n];
    memset(is_completed, 0, sizeof(is_completed));
    
    while (completed != n) {
        int highest_priority = -1;
        int min_priority = INT_MAX;
        
        // Find highest priority process (lower number = higher priority)
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].priority < min_priority) {
                    min_priority = processes[i].priority;
                    highest_priority = i;
                }
            }
        }
        
        if (highest_priority == -1) {
            current_time++;
            continue;
        }
        
        processes[highest_priority].waiting_time = current_time - processes[highest_priority].arrival_time;
        processes[highest_priority].completion_time = current_time + processes[highest_priority].burst_time;
        processes[highest_priority].turnaround_time = processes[highest_priority].completion_time - processes[highest_priority].arrival_time;
        
        current_time = processes[highest_priority].completion_time;
        is_completed[highest_priority] = 1;
        completed++;
    }
    
    calculate_times(processes, n);
}

// 5. ROUND ROBIN
void round_robin(Process processes[], int n, int quantum) {
    printf("\n=== ROUND ROBIN (Time Quantum = %d) ===\n", quantum);
    
    // Initialize remaining times
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    int current_time = 0, completed = 0;
    
    while (completed != n) {
        int progress = 0;
        
        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time) {
                progress = 1;
                
                if (processes[i].remaining_time > quantum) {
                    current_time += quantum;
                    processes[i].remaining_time -= quantum;
                } else {
                    current_time += processes[i].remaining_time;
                    processes[i].remaining_time = 0;
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                    completed++;
                }
            }
        }
        
        if (!progress) current_time++;
    }
    
    calculate_times(processes, n);
}

// 6. MULTILEVEL QUEUE SCHEDULING
void multilevel_queue(Process processes[], int n) {
    printf("\n=== MULTILEVEL QUEUE SCHEDULING ===\n");
    printf("Queue 0: System processes (Priority 0-1)\n");
    printf("Queue 1: Interactive processes (Priority 2-3)\n");
    printf("Queue 2: Batch processes (Priority 4+)\n\n");
    
    // Separate processes into queues based on priority
    Process queue0[n], queue1[n], queue2[n];
    int q0_count = 0, q1_count = 0, q2_count = 0;
    
    for (int i = 0; i < n; i++) {
        if (processes[i].priority <= 1) {
            queue0[q0_count++] = processes[i];
        } else if (processes[i].priority <= 3) {
            queue1[q1_count++] = processes[i];
        } else {
            queue2[q2_count++] = processes[i];
        }
    }
    
    int current_time = 0;
    
    // Process Queue 0 (FCFS)
    printf("Processing Queue 0 (System):\n");
    for (int i = 0; i < q0_count; i++) {
        if (current_time < queue0[i].arrival_time) {
            current_time = queue0[i].arrival_time;
        }
        queue0[i].waiting_time = current_time - queue0[i].arrival_time;
        queue0[i].completion_time = current_time + queue0[i].burst_time;
        queue0[i].turnaround_time = queue0[i].completion_time - queue0[i].arrival_time;
        current_time = queue0[i].completion_time;
    }
    
    // Process Queue 1 (Round Robin with quantum 4)
    printf("Processing Queue 1 (Interactive):\n");
    for (int i = 0; i < q1_count; i++) {
        queue1[i].remaining_time = queue1[i].burst_time;
    }
    
    int q1_completed = 0;
    while (q1_completed != q1_count) {
        for (int i = 0; i < q1_count; i++) {
            if (queue1[i].remaining_time > 0 && queue1[i].arrival_time <= current_time) {
                if (queue1[i].remaining_time > 4) {
                    current_time += 4;
                    queue1[i].remaining_time -= 4;
                } else {
                    current_time += queue1[i].remaining_time;
                    queue1[i].remaining_time = 0;
                    queue1[i].completion_time = current_time;
                    queue1[i].turnaround_time = queue1[i].completion_time - queue1[i].arrival_time;
                    queue1[i].waiting_time = queue1[i].turnaround_time - queue1[i].burst_time;
                    q1_completed++;
                }
            }
        }
        if (q1_completed < q1_count && current_time < 100) current_time++; // Prevent infinite loop
    }
    
    // Process Queue 2 (FCFS)
    printf("Processing Queue 2 (Batch):\n");
    for (int i = 0; i < q2_count; i++) {
        if (current_time < queue2[i].arrival_time) {
            current_time = queue2[i].arrival_time;
        }
        queue2[i].waiting_time = current_time - queue2[i].arrival_time;
        queue2[i].completion_time = current_time + queue2[i].burst_time;
        queue2[i].turnaround_time = queue2[i].completion_time - queue2[i].arrival_time;
        current_time = queue2[i].completion_time;
    }
    
    // Display results for each queue
    if (q0_count > 0) {
        printf("\nQueue 0 Results:\n");
        calculate_times(queue0, q0_count);
    }
    if (q1_count > 0) {
        printf("\nQueue 1 Results:\n");
        calculate_times(queue1, q1_count);
    }
    if (q2_count > 0) {
        printf("\nQueue 2 Results:\n");
        calculate_times(queue2, q2_count);
    }
}

int main() {
    int n = 5; // Number of processes
    
    // Sample processes
    Process processes[] = {
        {1, 0, 8, 2, 0, 0, 0, 0},   // PID, Arrival, Burst, Priority
        {2, 1, 4, 1, 0, 0, 0, 0},
        {3, 2, 9, 3, 0, 0, 0, 0},
        {4, 3, 5, 4, 0, 0, 0, 0},
        {5, 4, 2, 0, 0, 0, 0, 0}
    };
    
    Process temp[5]; // Temporary array for each algorithm
    
    printf("SAMPLE PROCESSES:\n");
    printf("PID\tArrival\tBurst\tPriority\n");
    printf("--------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].arrival_time, 
               processes[i].burst_time, processes[i].priority);
    }
    
    // Test each algorithm
    
    // 1. FCFS
    memcpy(temp, processes, sizeof(processes));
    fcfs(temp, n);
    
    // 2. SJF Non-preemptive
    memcpy(temp, processes, sizeof(processes));
    sjf_non_preemptive(temp, n);
    
    // 3. SRTF (Preemptive SJF)
    memcpy(temp, processes, sizeof(processes));
    srtf(temp, n);
    
    // 4. Priority Non-preemptive
    memcpy(temp, processes, sizeof(processes));
    priority_non_preemptive(temp, n);
    
    // 5. Round Robin
    memcpy(temp, processes, sizeof(processes));
    round_robin(temp, n, 3);
    
    // 6. Multilevel Queue
    memcpy(temp, processes, sizeof(processes));
    multilevel_queue(temp, n);
    
    return 0;
}


 1. Shortest Job First (SJF) - Non-Preemptive
How it Works: The scheduler selects the process from the ready queue that has the smallest total burst time. Once selected, the process runs until it is completely finished.
Analogy: A doctor at a walk-in clinic who decides to treat all the patients with the quickest, simplest problems first to clear the waiting room faster.
Key Traits:
    Non-Preemptive: Doesn't interrupt a running process.
    Pro: Provably optimal in minimizing the average waiting time.
    Con: Can lead to starvation if short processes keep arriving, making a long process wait forever.


 2. Shortest Remaining Time First (SRTF) - Preemptive
How it Works: This is the preemptive version of SJF. The scheduler always runs the process that has the least amount of time left to execute. It will interrupt (preempt) a currently running process if a new process arrives with a shorter remaining time.
Analogy: An express checkout cashier who will pause scanning a large grocery order to let someone with just one item cut in and finish quickly.
Key Traits:
    Preemptive: Can interrupt a running process.
    Pro: Gives an even better average waiting time than non-preemptive SJF.
    Con: High overhead from frequent context switching and a high risk of starvation for longer processes.


 3. Priority Scheduling - Non-Preemptive
How it Works: Each process is assigned a priority. The scheduler always picks the waiting process with the highest priority (typically, the lowest priority number). It runs that process to completion. Ties are broken by FCFS.
Analogy: Boarding an airplane. First-class passengers (high priority) board before everyone else. Once a passenger is on the jet bridge, they aren't called back.
Key Traits:
    Non-Preemptive: Runs until finished.
    Pro: Allows critical processes to be run first.
    Con: Can lead to starvation of low-priority processes.


 4. Multilevel Queue Scheduling
How it Works: The ready queue is split into multiple separate queues (e.g., a high-priority "interactive" queue and a low-priority "batch" queue). Each queue has its own scheduling algorithm (e.g., Round Robin for interactive, FCFS for batch). The scheduler gives preference to higher-priority queues.
Analogy: An airport with separate, prioritized boarding lines. The first-class line is always served before the economy line. Processes don't move between lines.
Key Traits:
    Preemptive between queues: A batch process will be interrupted if an interactive one arrives.
    Pro: Very flexible; can assign the right scheduling policy to each process type.
    Con: Can lead to starvation of the lower-priority queues if high-priority queues are always busy.


 5. Round Robin (RR)
How it Works: A preemptive algorithm that gives each process a small, fixed time slice called a time quantum. It picks a process, lets it run for one quantum, and if it's not done, moves it to the back of the ready queue.
Analogy: A group of people taking turns telling a story. Each person gets to speak for exactly one minute before the turn passes to the next person in the circle.
Key Traits:
    Preemptive: Always interrupts a process after its time quantum expires.
    Pro: Very fair, no starvation, and excellent response time for interactive systems.
    Con: Performance is highly dependent on the quantum size. Too small a quantum causes too much overhead from context switching.
    Con: If the quantum is too large, it behaves like FCFS, leading to poor response times for interactive processes.
    Con: Can lead to high turnaround times for long processes if the quantum is too small.
    Con: Can lead to high overhead from frequent context switching, especially with many short processes.



------>     Multi Tasking and Multi Processing      <------

----------------------------------------------------------------------------------------------------------------------------
Feature	            Multitasking	                                        Multiprocessing
----------------------------------------------------------------------------------------------------------------------------
CPU Cores	        One	                                                Two or more
Execution Style	    Concurrent (tasks take turns rapidly)	                Parallel (tasks run at the same time)
Core Idea	        Context Switching	                                    Parallel Processing
Goal	            Provide responsiveness and interactivity.	            Increase raw computational power and speed.
Analogy	            One person juggling multiple jobs.	                    A team of people working on different jobs.
----------------------------------------------------------------------------------------------------------------------------


------>     Thread Lifecycle       <------

A thread moves through several states from its creation to its termination.

1) New: The thread has been created but has not yet been started. It exists as an object but is not yet eligible to run.
2) Runnable (Ready): The thread is ready to execute and is waiting for the CPU scheduler to select it. Calling the start() method on a new thread moves it to this state.
3) Running: The thread is currently being executed by the CPU. The scheduler can move it from Runnable to Running.
4) Blocked (Waiting): The thread is temporarily inactive because it's waiting for something to happen. For example, it might be waiting for an I/O operation to complete (like reading a file) or waiting to acquire a lock held by another thread. Once the event it's waiting for occurs, it moves back to the Runnable state.
5) Terminated (Dead): The thread has completed its execution or has otherwise been terminated. It cannot be run again.


Starvation typically occurs in scheduling algorithms where a flawed or biased selection process can cause certain 
processes to be overlooked indefinitely. The most common cause is priority-based scheduling.

Imagine a system where high-priority processes are always executed before low-priority ones. If there is a continuous stream of high-priority tasks entering the system, the scheduler will always be busy serving them. 
A low-priority process waiting in the queue will never get its turn, even though it's ready to run. This leads to starvation, where the low-priority process is starved of CPU time and never gets executed.


'------>     What is IPC?       <------

Inter Process Communication (IPC) is a set of methods used by processes to communicate with each other.
IPC is essential in multitasking and multiprocessing systems where multiple processes need to share data or synchronize their actions.

Inter-Process Communication (IPC) refers to the mechanisms an operating system provides to allow different 
processes to manage shared data and communicate with each other. By default, processes are isolated from each 
other for security and stability. IPC provides a safe and structured way to break this isolation when needed.

1. Pipes 
-----------
 
Pipes are the simplest form of IPC. A pipe is a unidirectional (one-way) communication channel that connects two related processes (usually a parent and child). Data is written to one end and read from the other as a continuous stream of bytes.

  Key Characteristics:
      Unidirectional: Data flows in one direction only.
      Related Processes: Requires a common ancestor process (usually created before a `fork()`).
      Byte Stream: No concept of message boundaries. If you write 10 bytes then 20 bytes, the reader might get all 30 bytes in one go.
      Synchronized: The OS handles it. The `read()` call will wait if the pipe is empty.
  Best For: Simple, one-way communication between a parent and child process, like redirecting the output of one command to the input of another (`ls -l | grep .c`).

# Example Code with Comments

#include <stdio.h>
#include <unistd.h>

int main() {
    int pipefd[2]; // Array to hold file descriptors: pipefd[0] is for reading, pipefd[1] is for writing.
    pid_t pid;
    char buffer[30];

    // Create the pipe. This must be done BEFORE forking.
    pipe(pipefd);

    // Create a child process. Both parent and child now have the pipe file descriptors.
    pid = fork();

    if (pid == 0) {
        // --- CHILD PROCESS --- (Will only read)
        // Close the unused write end of the pipe. It's good practice.
        close(pipefd[1]);

        // Read from the pipe. This call will BLOCK (wait) until the parent writes something.
        read(pipefd[0], buffer, sizeof(buffer));
        printf("CHILD RECEIVED: %s\n", buffer);

        // Clean up by closing the read end.
        close(pipefd[0]);

    } else {
        // --- PARENT PROCESS --- (Will only write)
        // Close the unused read end of the pipe.
        close(pipefd[0]);

        // Write a string into the pipe. This data is now available for the child to read.
        write(pipefd[1], "Hello from parent", 18);

        // Close the write end. This sends an "end-of-file" to the reader.
        close(pipefd[1]);
    }
    return 0;
}


2. Message Queues
-------------------

A message queue is a linked list of messages maintained by the kernel. Any process with the right permissions can add or remove messages from the queue. Unlike pipes, processes dont need to be related.

  Analogy: A restaurant ticket rail. Waiters (producers) put order tickets (messages) on the rail. Chefs (consumers) pick them up when ready.
  Key Characteristics:
      Unrelated Processes: Any process can access a queue if it has the key.
      Message Boundaries: Preserves message structure. If you send a 10-byte message, you receive a 10-byte message.
      Asynchronous: The sender can place a message and continue without waiting for a receiver.
      Persistent: The queue and its messages exist in the kernel and persist even if no process is currently using them.
      Types/Priorities: Messages can be tagged with a type, allowing receivers to selectively retrieve messages.
  Best For: Communication between multiple, unrelated processes that might not be running at the same time. Good for when you need structured, message-based communication.


3. Shared Memory
-------------------

Shared memory is the fastest IPC method. It allows multiple processes to map the same block of physical RAM into their own virtual address spaces. Once mapped, communication is as fast as reading from or writing to a regular variable.

  Analogy: A shared whiteboard. Everyone in the room can read from or write to the same board instantly.
  Key Characteristics:
      Fastest IPC: No kernel involvement is needed after the initial setup. Data is not copied.
      Unrelated Processes: Any process can attach a shared memory segment.
      No Synchronization: This is its biggest challenge. It provides no mechanism to prevent two processes from writing at the same time (a race condition). You must implement your own synchronization using tools like semaphores or mutexes.
  Best For: High-performance applications sharing large amounts of data, like databases, video processing, or scientific computing.



------->    Thread Synchronization      <-------

Mutex (Mutual Exclusion)

A mutex acts like a key to a specific section of code, ensuring that only one thread can execute it at a time. It is a simple binary lock (locked or unlocked). 
A thread must acquire the lock before entering the critical section and release it upon exiting. Importantly, only the thread that locked the mutex is allowed to unlock it. 
Other threads that try to acquire a locked mutex will be put to sleep by the OS until the lock is released.

Semaphore

A semaphore is a more general synchronization tool that acts as a counter for a pool of available resources. 
It is used to control access for a limited number of threads.

Counting Semaphore: Manages access to 'N' identical resources. A thread "waits" on the semaphore, which decrements the count. If the count is zero, the thread sleeps. When a thread is finished, it "posts" to the semaphore, incrementing the count and potentially waking up a sleeping thread.
Binary Semaphore: A semaphore with a count of 1, which behaves very similarly to a mutex.

Spinlock

A spinlock is a type of lock where a waiting thread does not go to sleep. Instead, it "spins" in a tight loop, repeatedly checking if the lock is available. 
This busy-waiting consumes CPU cycles but avoids the overhead of a context switch (the process of the OS putting a thread to sleep and waking it up later). 
Spinlocks are highly efficient but only suitable for critical sections that are extremely short.

Condition Variable

A condition variable allows threads to synchronize based on the actual state of data, not just on access control. It enables threads to wait efficiently until a specific condition becomes true. 
It must always be used in conjunction with a mutex. A thread locks the mutex, checks a condition, and if the condition isn't met, it calls `wait()`. 
This atomically releases the mutex and puts the thread to sleep. Another thread can later change the data, lock the mutex, and then call `signal()` or `broadcast()` on the condition variable to wake up the waiting thread(s).


------>     Memory Management in C    ------

 Stack Memory
The stack is a region of memory that stores temporary variables created by functions. It operates on a Last-In-First-Out (LIFO) principle.

Characteristics:
- Automatic management: Variables are automatically created when entering a function and destroyed when exiting
- Fast access: Direct memory access through stack pointer manipulation
- Limited size: Typically 1-8 MB per thread
- Stores: Function parameters, return addresses, local variables

Example:

void function1() {
    int a = 10;        // Allocated on stack
    char buffer[100];  // Allocated on stack
    function2();       // Stack grows
}  // Variables automatically deallocated when function ends

void function2() {
    int b = 20;        // New stack frame created
    return;            // Stack frame destroyed
}


Stack Layout:

High Memory Address
|------------------|
| function2 frame  |  <- Current Stack Pointer
|   int b = 20     |
|------------------|
| function1 frame  |
|   char buffer[]  |
|   int a = 10     |
|------------------|
| Return address   |
|------------------|
Low Memory Address


 Heap Memory
The heap is a region for dynamic memory allocation where data can be allocated and deallocated in any order.

Characteristics:
- Manual management: Programmer must explicitly allocate and free memory
- Slower access: Requires pointer dereferencing and memory management overhead
- Large size: Limited only by available system memory
- Stores: Dynamic data structures, large objects

Example:

void heapExample() {
    // Dynamic allocation on heap
    int* ptr = (int*)malloc(sizeof(int) * 100);
    
    if (ptr != NULL) {
        ptr[0] = 42;  // Accessing heap memory
        ptr[99] = 100;
        
        // Must manually free
        free(ptr);
    }
}

## Dynamic vs Static Memory Allocation

 Static Memory Allocation
Memory is allocated at compile time and remains fixed throughout program execution.

Characteristics:
- Compile-time allocation: Size determined before program runs
- Fixed size: Cannot be changed during execution
- Automatic management: No need for malloc/free
- Fast access: Direct memory access

Example:

// Global static allocation
int globalArray[1000];  // 4000 bytes allocated at compile time

void staticExample() {
    // Local static allocation
    static int counter = 0;     // Allocated once, persists across calls
    int localArray[50];         // Stack allocation, fixed size
    
    counter++;
}

 Dynamic Memory Allocation
Memory is allocated at runtime based on program needs.

Characteristics:
- Runtime allocation: Size can be determined during execution
- Variable size: Can allocate exactly what's needed
- Manual management: Requires explicit allocation/deallocation
- Flexible: Can grow/shrink as needed

Example:

void dynamicExample() {
    int size;
    printf("Enter array size: ");
    scanf("%d", &size);
    
    // Dynamic allocation based on user input
    int* dynamicArray = (int*)malloc(size * sizeof(int));
    
    if (dynamicArray != NULL) {
        // Use the array
        for (int i = 0; i < size; i++) {
            dynamicArray[i] = i * 2;
        }
        
        // Resize if needed
        dynamicArray = (int*)realloc(dynamicArray, size * 2 * sizeof(int));
        
        free(dynamicArray);  // Must free manually
    }
}


## Fragmentation

Fragmentation occurs when memory becomes divided into small, unusable pieces.

 Internal Fragmentation
Wasted space within allocated memory blocks.

Causes:
- Fixed-size allocation schemes
- Padding for alignment requirements
- Allocating more memory than requested

Example:

// If allocator uses 64-byte blocks
char* ptr = malloc(10);  // Only 10 bytes needed
// But 64 bytes allocated, 54 bytes wasted (internal fragmentation)

Visual representation:

|-- 64-byte block --|
|used(10)|waste(54) |


 External Fragmentation
Free memory exists but is scattered in small, non-contiguous chunks.

Example scenario:

void fragmentationDemo() {
    char* ptr1 = malloc(100);  // Block A
    char* ptr2 = malloc(100);  // Block B  
    char* ptr3 = malloc(100);  // Block C
    
    free(ptr2);  // Frees middle block
    
    // Now we have: [A][FREE(100)][C]
    // If we need 200 bytes, we can't allocate even though
    // total free space might be available
    char* ptr4 = malloc(200);  // Might fail due to fragmentation
}

Memory layout:

Before fragmentation:
|Block A(100)|Block B(100)|Block C(100)|Free Space|

After freeing Block B:
|Block A(100)|Free(100)   |Block C(100)|Free Space|
             ↑ Can't use for large allocation


 Solutions to Fragmentation:

1. Compaction: Moving allocated blocks to eliminate gaps
2. Buddy System: Using power-of-2 sized blocks
3. Memory Pools: Pre-allocating fixed-size chunks

## Memory Protection and Segmentation

 Memory Protection
Mechanisms to prevent unauthorized access to memory regions.

Protection Mechanisms:

1. Base and Limit Registers

// Each process has base and limit registers
struct process_memory {
    void* base_address;     // Starting address
    size_t limit;          // Maximum offset
};

// Hardware checks on every memory access:
// if (address < base || address >= base + limit)
//     generate_protection_fault();
```

2. Protection Bits
```c
// Memory pages have protection attributes
typedef enum {
    READ_ONLY    = 0x1,
    READ_WRITE   = 0x3,
    EXECUTE      = 0x4,
    USER_ACCESS  = 0x8
} protection_flags_t;
```

 Segmentation
Dividing program memory into logical segments with different purposes.

Common Segments:
- Code Segment: Contains executable instructions
- Data Segment: Contains global and static variables
- Stack Segment: Contains local variables and function calls
- Heap Segment: Contains dynamically allocated memory

Example Program Layout:

// Global variables (Data segment)
int globalVar = 42;
static int staticVar = 100;

// Function code (Code segment)
void exampleFunction() {
    // Local variables (Stack segment)
    int localVar = 10;
    char buffer[256];
    
    // Dynamic allocation (Heap segment)
    int* heapVar = malloc(sizeof(int));
    *heapVar = 20;
    
    free(heapVar);
}

int main() {
    exampleFunction();
    return 0;
}


Memory Layout with Segmentation:

High Memory
|-------------------|
| Stack Segment     | ← Growing downward
| (Local variables) |
|-------------------|
| Free Space        |
|-------------------|
| Heap Segment      | ← Growing upward
| (malloc'd memory) |
|-------------------|
| Data Segment      |
| (Global vars)     |
|-------------------|
| Code Segment      |
| (Program code)    |
|-------------------|
Low Memory



-------> what is paging in os <--------

Paging is a memory management scheme used by operating systems to eliminate the need for contiguous allocation of physical memory. It allows processes to be stored in non-contiguous memory blocks, optimizing memory utilization and enabling features like virtual memory.

---

### Core Concepts
1. Pages and Frames:
   - Logical Memory (Virtual Memory): Divided into fixed-size blocks called pages.
   - Physical Memory (RAM): Divided into fixed-size blocks called frames, matching page sizes.
   - *Example*: If a system uses 4KB pages, both logical and physical memory are split into 4KB chunks.

2. Page Table:
   - A data structure maintained by the OS for each process.
   - Maps logical page numbers to physical frame numbers.
   - Each entry contains metadata (e.g., validity, permissions).

3. Address Translation:
   - A logical address is split into:
     - Page Number: Indexes the page table.
     - Offset: Specifies the location within the page/frame.
   - The OS translates the logical address to a physical address using the page table.

---

### How Paging Manages Memory
#### Step-by-Step Process
1. Process Initialization:
   - When a process starts, the OS allocates frames in physical memory.
   - The page table is initialized with mappings between logical pages and physical frames.

2. Address Translation:
   - Suppose a process references a logical address `0x12345`.
   - The OS splits this into:
     - Page Number: `0x12` (determines which page table entry to use).
     - Offset: `0x345` (location within the page).
   - The page table entry for page `0x12` returns a frame number (e.g., `0x56`).
   - The physical address is calculated as:  
     `(Frame Number × Page Size) + Offset` → `(0x56 × 4096) + 0x345`.

3. Handling Page Faults:
   - If a process accesses a page not in physical memory (invalid bit set):
     - Trap to OS: The CPU triggers a page fault interrupt.
     - OS Actions:
       1. Check if the access is valid (e.g., no permission violation).
       2. Find a free frame (or evict a page via algorithms like LRU).
       3. Load the required page from disk into the free frame.
       4. Update the page table with the new mapping.
       5. Resume the interrupted instruction.

4. Memory Protection:
   - Page table entries include read/write/execute permissions.
   - Attempts to violate these trigger protection faults.

---

### Advantages of Paging
1. Eliminates External Fragmentation:
   - Processes can occupy non-contiguous frames, avoiding wasted gaps between allocations.

2. Simplifies Allocation:
   - Any free frame can be assigned to a process, reducing allocation complexity.

3. Enables Virtual Memory:
   - Pages can reside on disk (swap space) when not actively used, allowing processes to exceed physical RAM limits.

4. Isolation and Security:
   - Page tables enforce memory isolation between processes; one process cannot access another’s pages.

---

### Challenges & Solutions
1. Internal Fragmentation:
   - Wasted space within the last page of a process (fixed page size).
   - *Mitigation*: Use smaller page sizes (trade-off with increased page table size).

2. Page Table Overhead:
   - Large page tables consume significant memory.
   - *Solutions*:
     - Multi-Level Paging: Hierarchical page tables (e.g., 2-level for 32-bit systems).
     - Inverted Page Tables: Single table shared across processes (used in 64-bit systems).
     - Translation Lookaside Buffer (TLB): Hardware cache storing recent translations to reduce page table lookups.

3. Page Replacement Algorithms:
   - Deciding which page to evict during a page fault (e.g., FIFO, LRU, Clock Algorithm).



-----------------  What is Deadlock in OS?  -----------------

Deadlock in operating systems is a situation where a set of processes are blocked because each process is holding a resource and waiting for another resource held by another process. In other words, a deadlock occurs when two or more processes are unable to proceed because each is waiting for the other to release a resource.

Conditions for Deadlock

For a deadlock to occur, the following four conditions must hold simultaneously:
1. **Mutual Exclusion**: At least one resource must be held in a non-shareable mode.
2. **Hold and Wait**: A process holding at least one resource is waiting to acquire additional resources.
3. **No Preemption**: Resources cannot be forcibly taken from a process holding them.
4. **Circular Wait**: There must be a circular chain of processes, each waiting for a resource held by the next process in the chain.

Deadlock Prevention Strategies

1. Resource Allocation Graph : Use a graph to represent resource allocation and requests. If a request would create a cycle, it is denied.
2. Banker's Algorithm : A dynamic approach that checks resource availability before allocation, ensuring the system remains in a safe state.
3. Avoidance of Hold and Wait : Require processes to request all resources at once, preventing hold and wait conditions.

Deadlock Prevention Techniques

1. Attacking Mutual Exclusion
This is often not possible. Some resources, like printers or files being written to, are inherently non-sharable. Forcing them to be sharable could lead to data corruption. However, for resources that can be managed (e.g., read-only files), sharing can be allowed.

2. Attacking Hold and Wait
To prevent this condition, you can enforce a rule that a process must either request all its required resources at once or release all its currently held resources before requesting new ones.

Method 1: Request all resources initially. A process must acquire all the resources it will ever need before it begins execution. This can lead to low resource utilization.

Method 2: Release resources before new requests. A process must release any resources it's holding before it can request any new ones.

3. Attacking No Preemption
If a process holding some resources requests another resource that cannot be immediately allocated to it, the OS could preempt (take away) all resources currently held by this process. These preempted resources are added to the list of resources for which the process is waiting. The process will be restarted only when it can regain its old resources, as well as the new ones it is requesting. This is often difficult to implement and may not be suitable for all resource types.

4. Attacking Circular Wait 
This is the most practical and commonly used deadlock prevention technique. The strategy is to impose a total ordering of all resource types and require that each process requests resources in an increasing order of enumeration.

For example, if the resource order is R_1,R_2,R_3, a process can request R_1 and then R_2, but it cannot request R_2 and then R_1. This ensures that a circular dependency can never form.
 



















To learn about the process lifecycle from scratch using Chapters 24–26 of "The Linux Programming Interface," we must look at how a process is born, how it runs, how it dies, and what happens after it dies. The lifecycle revolves around four key system calls: **fork()**, **exit()**, **wait()**, and **execve()**.

Here is a detailed breakdown of the process lifecycle.

### 1. Process Creation: `fork()`
In Linux, a new process is not created from scratch; it is created by cloning an existing one. The process that initiates the creation is called the **parent**, and the new process is called the **child**.

*   **The Mechanism:** The `fork()` system call creates a new process by making an almost exact duplicate of the parent. The child receives copies of the parent's stack, data, heap, and text segments.
*   **Distinguishing Parent from Child:** After `fork()` is called, two processes are running the same code. To distinguish them, the code checks the return value of `fork()`:
    *   **In the Parent:** `fork()` returns the **Process ID (PID)** of the newly created child.
    *   **In the Child:** `fork()` returns **0**.
    *   **Error:** If creation fails, it returns **-1**.
*   **Memory Efficiency (Copy-on-Write):** The kernel does not physically copy every byte of memory immediately. It marks the memory pages as read-only. If either the parent or child tries to modify a page, the kernel makes a duplicate of that specific page only at that moment. This technique, known as **copy-on-write**, makes `fork()` very fast.
*   **File Sharing:** The child receives duplicates of all the parent's open file descriptors. Because they refer to the same open file description, the parent and child share the file offset. If the child reads part of a file, the parent's current offset in that file moves forward as well.

**Note on `vfork()`:** Linux also provides `vfork()`, a specialized system call where the child shares the parent's memory *without* copying. The parent is suspended until the child either terminates or executes a new program. This is historically faster but dangerous because the child can alter the parent's variables.

### 2. Changing the Program: `execve()`
While `fork()` creates a duplicate process, you often want the child to run a *different* program. This is done using `execve()` (or the `exec` family of functions).
*   **Purpose:** `execve()` loads a new program into the process's memory. It discards the old program text, stack, data, and heap, replacing them with those of the new program.
*   **The Flow:** The typical lifecycle flow is: The parent calls `fork()`; the child process starts running; the child immediately calls `execve()` to replace itself with a new program (like `ls` or `grep`).

### 3. Process Termination: `exit()`
A process can terminate normally (by calling `exit`) or abnormally (by being killed by a signal).
*   **The Functions:**
    *   **`_exit(status)`:** This is the raw system call that terminates the process and returns the `status` integer to the kernel.
    *   **`exit(status)`:** This is a library function layered on top of `_exit()`. It performs cleanup steps before terminating, such as flushing `stdio` buffers (like `printf` data waiting to be written) and calling exit handlers registered with `atexit()`.
*   **Best Practice:** Typically, only one process (usually the parent) should use `exit()`, while the other (the child) uses `_exit()`. This prevents buffered I/O data from being flushed and printed twice.
*   **Exit Handlers:** You can register functions using `atexit()` that will run automatically when the process terminates. They are called in reverse order of their registration.

### 4. Monitoring Child Processes: `wait()`
When a child process finishes, it doesn't disappear immediately; the parent must acknowledge its death. This is handled via **waiting**.
*   **`wait(&status)`:** This system call suspends the parent's execution until one of its children terminates. It returns the PID of the terminated child and stores the termination status (exit code) in the `status` variable.
*   **`waitpid()`:** This is a more flexible version that allows the parent to:
    *   Wait for a *specific* child PID.
    *   Perform a nonblocking check (using `WNOHANG`) to see if a child has exited without getting stuck waiting.
*   **Inspecting Status:** The integer status returned by `wait` is encoded. You must use macros to read it, such as `WIFEXITED(status)` (did it exit normally?) and `WEXITSTATUS(status)` (what was the exit number?).

### 5. Orphans and Zombies
Two specific states occur depending on who dies first—the parent or the child.
*   **Zombies:** If a child terminates but the parent has not yet called `wait()` to read its status, the child becomes a **zombie**. The kernel keeps a small entry (PID and termination status) so the parent can eventually retrieve it. If a parent creates many children but never `wait()`s for them, the system tables can fill up with zombies, preventing new processes from starting.
*   **Orphans:** If a parent terminates *before* the child, the child becomes an **orphan**. The `init` process (PID 1) adopts the orphan. `init` automatically performs `wait()` on its adopted children to clean them up, ensuring they don't remain zombies.

### 6. The `SIGCHLD` Signal
To avoid blocking the parent process while waiting for a child to finish, the kernel sends the **`SIGCHLD`** signal to the parent whenever a child terminates.
*   The parent can establish a signal handler for `SIGCHLD`.
*   Inside the handler, the parent calls `waitpid()` with the `WNOHANG` flag to reap the dead child immediately without interrupting the parent's main work.

### Summary Analogy
You can view the process lifecycle like a **Manager (Parent)** and a **Worker (Child)**:
1.  **Fork:** The Manager hires a new Worker who is an exact clone of the Manager (same knowledge, same files).
2.  **Exec:** The Worker usually immediately looks at a new job description (`exec`) and transforms into the specific specialist needed for that task (e.g., changing from a clone of the Manager into a Printer Specialist).
3.  **Exit:** When the job is done, the Worker reports "I'm finished" (`exit`) but stays in the office lobby with their report card.
4.  **Zombie:** If the Manager ignores the Worker, the Worker sits in the lobby forever (Zombie), taking up a seat.
5.  **Wait:** The Manager must eventually go to the lobby (`wait`) to collect the report card and dismiss the Worker.
6.  **Orphan:** If the Manager quits the company while the Worker is still active, the building administrator (`init`) takes over supervision of the Worker.







### 1. What is a Thread? (Chapter 29)

**Concept Explanation**
A thread is an independent stream of execution within a process. While a process allows multiple tasks to run concurrently by separating them into different memory spaces, threads allow multiple tasks to run concurrently *within* a shared memory space.

In Linux, threads are often referred to as "Kernel Scheduling Entities" (KSEs). A traditional UNIX process is simply a process containing a single thread. Multithreaded programming maps multiple threads to these KSEs so the kernel can schedule them across available CPUs.

**Memory-Level View**
*   **Shared Memory:** All threads within a process share the same global memory. This includes the initialized data segment, uninitialized data (bss), and the heap. This sharing allows for very fast communication but introduces synchronization risks.
*   **Private Memory:** Each thread maintains its own:
    *   **Stack:** Contains local variables and function call linkage.
    *   **Registers:** Including the Program Counter (PC) and Stack Pointer (SP).
    *   **Thread-Specific Data:** (e.g., `errno`).
*   **Layout:** In virtual memory, the main thread uses the standard stack area. Additional thread stacks are allocated (often from the heap area or `mmap`) and reside within the same virtual address space.

**Linux/Embedded Relevance**
Linux uses the 1:1 threading model (NPTL - Native POSIX Threads Library), meaning every user-level thread maps directly to a kernel-level thread. Thread creation is roughly 10x faster than process creation (`fork()`) because page tables and file descriptor tables do not need to be duplicated, only shared.

---

### 2. POSIX Threads (pthreads): Lifecycle (Chapter 29)

**Concept Explanation**
The Pthreads API defines the lifecycle of a thread from creation to termination.

*   **Creation:** `pthread_create()` creates a new thread. It starts executing a specific function immediately.
*   **Termination:** A thread ends if it returns from its start function, calls `pthread_exit()`, or is canceled.
*   **Join:** `pthread_join()` is similar to `waitpid()` for processes. It blocks the calling thread until the target thread terminates and retrieves its return value.
*   **Detach:** `pthread_detach()` marks a thread as "detached." When a detached thread terminates, its resources are automatically recovered by the system. You cannot join a detached thread.

**Simple C Example**
```c
#include <pthread.h>
#include <stdio.h>

void *threadFunc(void *arg) {
    char *str = (char *) arg;
    printf("%s\n", str);
    return (void *) 100; // Return status
}

int main() {
    pthread_t t1;
    void *res;
    // Create
    pthread_create(&t1, NULL, threadFunc, "Hello Embedded World");
    // Join (Wait)
    pthread_join(t1, &res); 
    printf("Thread returned: %ld\n", (long)res);
}
```

**Memory-Level View**
When `pthread_create` is called, the library allocates a new stack for the thread. The thread ID (`pthread_t`) serves as a handle to access the thread's control block structure in user space.

**Common Mistakes**
*   **Memory Leaks:** Failing to `join` a joinable thread creates a "zombie thread," wasting system resources. You must either join it or detach it.
*   **Stack References:** Passing a pointer to a local variable on the creating thread's stack to the new thread. If the creating function returns, that stack frame is invalidated, leading to data corruption.

---

### 3. Thread Scheduling Basics (Chapter 35)

**Concept Explanation**
Linux uses a preemptive priority-based scheduler.
*   **SCHED_OTHER:** The default round-robin time-sharing policy. Nice values (-20 to +19) weight the CPU time a thread gets,.
*   **Realtime Policies (SCHED_FIFO / SCHED_RR):** Critical for embedded systems. These threads have priorities (1-99) and **always** preempt `SCHED_OTHER` tasks.
    *   *SCHED_FIFO:* Runs until it yields or blocks.
    *   *SCHED_RR:* Runs until time slice expires or it blocks.

**Execution Timeline**
1.  **Context Switch:** The kernel saves the current thread's registers (PC, SP) to the kernel stack.
2.  **Scheduler:** The scheduler picks the next KSE.
3.  **Restore:** The kernel restores the registers of the new thread.

**Embedded/Linux Relevance**
On multiprocessor systems, Linux attempts "soft CPU affinity," trying to keep a thread on the same CPU to maximize cache hits. You can force "hard affinity" using `sched_setaffinity()` to bind time-critical threads to specific cores, preventing cache invalidation overhead,.

---

### 4. Race Conditions (Chapter 30)

**Concept Explanation**
A race condition occurs when the outcome of a program depends on the nondeterministic interleaving of threads accessing shared data.

**Real Example (Read-Modify-Write)**
Two threads try to increment a global variable `glob`.
1.  Thread A reads `glob` (value 0) into a register.
2.  **Context Switch happens.**
3.  Thread B reads `glob` (still 0) into a register.
4.  Thread B increments and writes 1 to RAM.
5.  **Context Switch back to A.**
6.  Thread A (still holding 0 in register) increments and writes 1 to RAM.
*Result:* `glob` is 1, but it should be 2.

**Memory-Level View**
The CPU executes instructions atomically, but C statements (like `glob++`) translate to multiple assembly instructions (Load, Increment, Store). The interruption can happen between the Load and the Store.

**Common Mistakes**
*   Assuming `i++` is atomic.
*   Testing a condition and acting on it without a lock (Time-of-check, Time-of-use bugs).

---

### 5. Mutexes (Chapter 30)

**Concept Explanation**
A Mutex (Mutual Exclusion) acts as a lock. It ensures that only one thread accesses a critical section (shared resource) at a time.

**Internal Working**
Internally, Linux mutexes use **futexes** (fast user-space mutexes).
1.  **Fast Path:** An atomic integer decrement in user space. If the count goes from 1 to 0, the lock is acquired without a system call.
2.  **Slow Path:** If contention exists (count was already 0), the thread makes a `futex()` system call to sleep in the kernel until woken.

**Deadlock & Starvation**
*   **Deadlock:** Thread A holds Mutex 1 and waits for Mutex 2. Thread B holds Mutex 2 and waits for Mutex 1. Both wait forever.
*   **Fix:** Always acquire mutexes in a fixed hierarchy (e.g., always Lock 1, then Lock 2).

**Simple C Example**
```c
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int glob = 0;

void *threadFunc(void *arg) {
    pthread_mutex_lock(&mtx);   // Atomic lock
    glob++;                     // Critical section
    pthread_mutex_unlock(&mtx); // Release
    return NULL;
}
```

**Embedded/Linux Relevance**
Priority Inversion occurs when a low-priority thread holds a mutex needed by a high-priority thread. A medium-priority thread preempts the low one, effectively stalling the high-priority thread. Linux Pthreads supports Priority Inheritance protocols to solve this (though not explicitly detailed in these specific text excerpts, this is the standard embedded solution).

---

### 6. Condition Variables (Chapter 30)

**Concept Explanation**
Mutexes protect data, but they don't allow a thread to wait for *state changes* (e.g., "buffer is not empty"). Polling a variable inside a mutex lock wastes CPU. Condition variables allow a thread to sleep until another thread signals that a condition has changed.

**Wait/Signal/Broadcast**
*   **Wait:** `pthread_cond_wait(&cond, &mtx)` atomically unlocks the mutex and puts the thread to sleep. When woken, it automatically re-locks the mutex before returning.
*   **Signal:** `pthread_cond_signal` wakes one waiting thread.
*   **Broadcast:** `pthread_cond_broadcast` wakes all waiting threads.

**The Predicate Loop (Why Mutex is Insufficient)**
You must always wait in a loop:
```c
pthread_mutex_lock(&mtx);
while (avail == 0) { // The Predicate
    pthread_cond_wait(&cond, &mtx);
}
// Do work
pthread_mutex_unlock(&mtx);
```
**Why?** "Spurious wakeups" can occur where the thread wakes up without a signal. Also, another thread might have snatched the resource between the signal and the wakeup.

---

### 7. Producer–Consumer Problem (Chapter 30)

**Step-by-Step Execution**
This is the classic threading pattern involving a shared buffer.

1.  **Shared State:** A buffer, a count of items (`avail`), a Mutex (`mtx`), and a Condition Variable (`cond`).
2.  **Consumer:**
    *   Locks `mtx`.
    *   Checks `while (avail == 0)`.
    *   If 0, calls `pthread_cond_wait`. (Releases `mtx`, sleeps).
    *   *...Time passes...*
    *   Wakes up (signaled). Re-acquires `mtx`.
    *   Checks `avail` again. It is > 0.
    *   Consumes item. Decrements `avail`.
    *   Unlocks `mtx`.
3.  **Producer:**
    *   Locks `mtx`.
    *   Produces item. Increments `avail`.
    *   Unlocks `mtx`.
    *   Calls `pthread_cond_signal`. (Wakes the consumer).

**Memory-Level View**
The `mtx` and `cond` structures reside in global memory. The "Wait" operation involves the thread changing its state from `RUNNING` to `INTERRUPTIBLE` (sleeping) in the kernel task structure, removing itself from the run queue.

---

### 8. Common Bugs and Debugging

**Common Bugs**
*   **Data Races:** Accessing shared data without mutexes.
*   **Deadlocks:** Inconsistent locking order.
*   **Detached Thread Race:** Assuming a detached thread has finished using resources before the main thread exits.
*   **Signal Handling:** Mixing signals and threads is dangerous. Per-thread signal masks exist, but signal actions are process-wide. A common fix is to dedicate one thread to call `sigwait()` while blocking signals in all others.

**How to Debug**
*   **Pthreads Return Values:** Unlike standard system calls, Pthreads functions return `0` on success and a positive error number (like `EPERM`) on failure. Do **not** check `errno`.
*   **Compile Options:** Use `cc -pthread` to define `_REENTRANT` and link `libpthread`.
*   **Tools:**
    *   `strace`: To trace system calls (like `futex`).
    *   **Helgrind / DRD (Valgrind tools):** Specifically designed to detect race conditions and locking errors that static analysis misses. (Referenced as malloc debugging in, applicable here).

**Embedded Note**
In resource-constrained embedded systems, stack space is critical. The default stack size on Linux/x86 is 2MB, which is massive for embedded. You should use `pthread_attr_setstacksize()` to lower this limit (e.g., to 16KB or 64KB) to avoid running out of RAM when creating multiple threads.



Based on "The Linux Programming Interface," the following is an in-depth analysis of the requested IPC mechanisms.

### 1. Pipes (Chapter 44)

**Why it exists:**
Pipes are the oldest form of IPC on UNIX. They solve the problem of passing data between **related** processes (typically a parent and child created via `fork()`). They are the mechanism underlying shell pipelines (e.g., `ls | wc`), allowing the standard output of one process to be directly connected to the standard input of another without temporary files.

**Internal Working:**
*   **Byte Stream:** Internally, a pipe is a limited-capacity buffer maintained in kernel memory. It functions as a byte stream; there are no message boundaries. The reader reads data in the order it was written.
*   **Data Flow:** Pipes are unidirectional. One end is for reading, the other for writing. Data written to the write-end is buffered by the kernel until read from the read-end.

**Process Memory Involvement:**
Pipes utilize a **Data-Transfer** model. This requires two memory copies:
1.  From the writer's user-space memory to the kernel buffer.
2.  From the kernel buffer to the reader's user-space memory.

**Lifecycle:**
*   **Creation:** Created via `pipe()`, which returns two file descriptors (FDs).
*   **Usage:** Usually followed by a `fork()`. The parent closes one end (e.g., read), and the child closes the other (e.g., write). Communication occurs via standard `read()` and `write()` calls.
*   **Teardown:** The pipe is destroyed and kernel resources released only when the last open file descriptor referring to it is closed.

**Blocking vs. Non-blocking:**
*   **Blocking (Default):** A `read()` blocks if the pipe is empty. A `write()` blocks if the pipe is full.
*   **Non-blocking (`O_NONBLOCK`):** A `read()` on an empty pipe fails with `EAGAIN`. A `write()` to a full pipe fails with `EAGAIN`.

**Synchronization Guarantees:**
*   **Implicit Synchronization:** The kernel automatically handles flow control. The writer pauses if the buffer is full; the reader pauses if the buffer is empty.
*   **Atomicity:** Writes of up to `PIPE_BUF` bytes (4096 bytes on Linux) are guaranteed to be atomic. If multiple processes write to the same pipe, data chunks smaller than this limit will not be interleaved.

**Limits and Performance:**
*   **Capacity:** In Linux kernels since 2.6.11, the pipe capacity is 65,536 bytes. Older kernels used the system page size (4096 bytes).
*   **Performance:** Efficiency depends on buffer sizes to minimize context switches. Transfers involve kernel overhead due to data copying.

**Security and Permission:**
Access is restricted to processes that inherit the file descriptors. There are no filesystem permissions or names associated with anonymous pipes; security is enforced by process lineage (only related processes have access).

**Failure Modes and Corner Cases:**
*   **SIGPIPE:** If a process writes to a pipe for which no process has an open read descriptor, the kernel sends the `SIGPIPE` signal, killing the writer by default. If handled/ignored, `write()` returns `EPIPE`.
*   **EOF:** The reader sees End-of-File (0 return from `read()`) only when *all* write descriptors referencing the pipe are closed.

***

### 2. FIFOs (Named Pipes)

**Why it exists:**
FIFOs solve the primary limitation of pipes: they allow communication between **unrelated** processes. Unlike pipes, a FIFO has a name in the file system, allowing any process with appropriate permissions to open it.

**Internal Working:**
Internally, a FIFO acts exactly like a pipe (a kernel buffer). However, it is represented by a file of type `p` in the filesystem. Opening the FIFO maps the file system name to the kernel buffer.

**Process Memory Involvement:**
Identical to pipes: Data flows from User Space (Writer) → Kernel Buffer → User Space (Reader).

**Lifecycle:**
*   **Creation:** Created using `mkfifo()`. This creates the filesystem entry.
*   **Usage:** Processes use `open()` to get a file descriptor, then `read()`/`write()`.
*   **Teardown:** Persistence is **Process-Persistent**. Data exists only while the FIFO is open. However, the *name* persists in the filesystem until explicitly removed via `unlink()`.

**Blocking vs. Non-blocking:**
*   **Open Blocking:** Opening a FIFO for reading blocks until another process opens it for writing, and vice versa. This provides a rendezvous point.
*   **O_NONBLOCK:**
    *   Open for read: Returns immediately.
    *   Open for write: Fails with `ENXIO` if no reader is currently present.

**Synchronization Guarantees:**
Identical to pipes. Writes up to `PIPE_BUF` are atomic.

**Limits and Performance:**
Identical to pipes regarding buffer capacity and throughput.

**Security and Permission:**
Security is handled via standard UNIX file system permissions (Owner/Group/Other r/w/x bits) assigned at creation time.

**Failure Modes and Corner Cases:**
*   **Deadlock:** If a single process tries to open a FIFO for writing without `O_NONBLOCK` and no reader exists, it will block forever.
*   **Read/Write Mode:** Opening a FIFO with `O_RDWR` is possible but non-standard; it bypasses the blocking synchronization and means the process effectively never sees EOF (since it holds a write descriptor itself).

***

### 3. POSIX Message Queues (Chapter 52)

**Why it exists:**
Message queues solve the "byte stream" limitation of pipes/FIFOs. They preserve message boundaries (a reader reads exactly one message as written). They also allow **priority** based retrieval, whereas pipes are strictly FIFO (First-In, First-Out).

**Internal Working:**
*   **Structure:** A linked list of messages stored in kernel memory.
*   **Attributes:** Each queue has `mq_maxmsg` (max number of messages) and `mq_msgsize` (max size per message).
*   **Priority:** Every message has an integer priority. Messages are ordered by priority (high to low), then by arrival time.

**Process Memory Involvement:**
Data-Transfer model. Messages are copied from user space into the kernel queue, then copied back to the receiver's user space.

**Lifecycle:**
*   **Creation:** `mq_open()` with `O_CREAT`. Uses a name starting with `/`.
*   **Usage:** `mq_send()` and `mq_receive()`.
*   **Teardown:** **Kernel-Persistent**. The queue and its data survive until the system shuts down or `mq_unlink()` is called to remove the name *and* all processes close their descriptors.

**Blocking vs. Non-blocking:**
*   **Blocking:** `mq_send()` blocks if the queue is full. `mq_receive()` blocks if the queue is empty.
*   **Non-blocking (`O_NONBLOCK`):** `mq_send()` fails with `EAGAIN` if full; `mq_receive()` fails with `EAGAIN` if empty.
*   **Timed:** `mq_timedsend()` and `mq_timedreceive()` allow blocking with a timeout.

**Synchronization Guarantees:**
*   **Ordering:** High-priority messages are always delivered before low-priority ones.
*   **Notification:** Supports asynchronous notification (`mq_notify`), allowing a process to receive a signal or instantiate a thread when a message arrives in an empty queue.

**Limits and Performance:**
*   **Limits:** Defined in `/proc/sys/fs/mqueue/`. Limits include maximum messages per queue and maximum message size.
*   **Performance:** Generally slower than shared memory due to kernel copying, but faster than pipes for discrete packetized data.

**Security and Permission:**
Uses file-like permission bits (read/write) specified during `mq_open()`.

**Failure Modes and Corner Cases:**
*   **EMSGSIZE:** If a message is larger than the queue's `mq_msgsize` attribute, the send fails immediately (it is not fragmented).

***

### 4. Semaphores (Chapter 53)

**Why it exists:**
Semaphores are for **Synchronization**, not data transfer. They solve race conditions by coordinating access to shared resources (like Shared Memory).

**Internal Working:**
*   **Structure:** A kernel-maintained integer value that cannot drop below zero.
*   **Operations:**
    *   `post` (increment): Releases a resource.
    *   `wait` (decrement): Reserves a resource. If the value is 0, the process blocks until it becomes > 0.

**Process Memory Involvement:**
*   **Named Semaphores:** The semaphore state is maintained by the kernel.
*   **Unnamed Semaphores:** The semaphore structure (`sem_t`) resides in user-space memory (shared memory or thread-shared heap), but operations likely involve kernel syscalls (futexes) for contention.

**Lifecycle:**
*   **Named (`sem_open`):** Kernel-Persistent. Exists until `sem_unlink` and all references closed. Identified by name (e.g., `/somename`).
*   **Unnamed (`sem_init`):** Process-Persistent (or kernel persistent if inside shared memory). Exists as long as the memory containing it is valid.

**Blocking vs. Non-blocking:**
*   `sem_wait()` blocks if value is 0.
*   `sem_trywait()` returns `EAGAIN` if value is 0.
*   `sem_timedwait()` blocks with a timeout.

**Synchronization Guarantees:**
Operations are atomic. If multiple processes wait on a semaphore, the order of waking is not strictly guaranteed by POSIX, though scheduling priority usually applies.

**Limits and Performance:**
*   **Limits:** `SEM_VALUE_MAX` (typically 2^31 - 1).
*   **Performance:** Very fast, especially on Linux (NPTL) where uncontended operations may happen largely in user space (using atomic assembly instructions/futexes) without full kernel entry overhead.

**Security and Permission:**
*   **Named:** File-like permissions set at creation.
*   **Unnamed:** Dependent on the permissions of the underlying shared memory region.

**Failure Modes and Corner Cases:**
*   **Deadlock:** If a process crashes while holding a semaphore (having decremented it), other processes waiting on it may block forever. POSIX semaphores generally lack the "UNDO" feature found in System V semaphores that attempts to fix this.

***

### 5. Shared Memory (Chapter 54)

**Why it exists:**
Shared Memory allows multiple processes to share the same region of physical memory. It solves the performance overhead of data transfer mechanisms; it is the **fastest** IPC because no data copying between user and kernel space is required.

**Internal Working:**
The kernel modifies the page table entries of the participating processes so that different virtual addresses in each process point to the exact same physical RAM pages.

**Process Memory Involvement:**
Zero-copy. Data is written directly to physical RAM by one process and read directly by another. The kernel is involved only in setup (`shm_open`, `mmap`).

**Lifecycle:**
*   **Creation:** `shm_open()` creates a POSIX shared memory object.
*   **Usage:** `ftruncate()` sets the size. `mmap()` maps it into the process's address space.
*   **Teardown:** `munmap()` unmaps it. `shm_unlink()` removes the name. **Kernel-Persistent**: The data remains in memory even if no process has it open, until explicitly unlinked or system reboot.

**Blocking vs. Non-blocking:**
There is **no** intrinsic blocking or non-blocking behavior. Reading from a memory address is immediate.

**Synchronization Guarantees:**
**None.** This is the major complexity of shared memory. If Process A writes while Process B reads, data corruption occurs. Synchronization (using Semaphores or Mutexes) must be implemented explicitly by the programmer.

**Limits and Performance:**
*   **Limits:** Limited by available system memory and swap space (`/dev/shm` is typically a `tmpfs` mount).
*   **Performance:** Extremely high bandwidth and low latency.

**Security and Permission:**
Permissions are set via `shm_open()` using standard file modes (e.g., `0660`).

**Failure Modes and Corner Cases:**
*   **Corruption:** Without external synchronization, race conditions guarantee data corruption.
*   **Leaks:** If a program crashes without `shm_unlink()`, the memory segment persists, consuming RAM until a reboot or manual deletion.






Based on Chapters 20, 21, and 22 of *The Linux Programming Interface*, here is a detailed explanation of the internal workings of signals.

### 1. What is a Signal and Why Does it Exist?
A signal is a software interrupt. It is a limited form of interprocess communication (IPC) used to notify a process that a specific event has occurred.
*   **Purpose:** They exist to bridge the gap between the kernel and user-space processes, or between different processes. They allow the kernel to inform a process of hardware exceptions (like division by zero), user input (like interrupting a program), or system events (like a child process terminating).
*   **Representation:** Each signal is defined as a unique small integer (e.g., 1 to 31 for standard signals). Programs use symbolic names (like `SIGINT` or `SIGSEGV`) rather than numbers.

### 2. Signal Lifecycle (Generation → Delivery → Handling)
The life of a signal occurs in three distinct stages:
1.  **Generation:** A signal is generated when the event that causes it occurs. This can be done by the kernel (e.g., hardware fault), by another process (using `kill()`), or by the process itself.
2.  **Pending:** Between generation and delivery, a signal is "pending." The kernel maintains a record of pending signals for a process. If a signal is generated but the process has currently blocked it (masked it), the signal remains pending until it is unblocked.
    *   *Note on Queuing:* Standard signals (1–31) are **not queued**. If a standard signal is generated multiple times while it is already pending, it is only recorded once. It will be delivered only once when unblocked. Real-time signals (32+) are queued.
3.  **Delivery:** A signal is delivered when the kernel forces the process to react to it. This reaction is the "handling" of the signal.

**When is a signal delivered?**
For asynchronously generated signals, the kernel delivers a pending signal when the process switches from kernel mode back to user mode. This happens, for example, when the process is rescheduled after a timeout (start of a time slice) or when returning from a system call.

### 3. Kernel vs. User-Space Responsibilities
*   **Kernel Responsibility:** The kernel tracks the state of signals (pending vs. blocked) in kernel data structures for each process. When a signal is delivered, the kernel suspends the normal execution flow of the process. If a custom handler is established, the kernel constructs a frame on the process's stack and modifies the program counter to jump to the handler function.
*   **User-Space Responsibility:** The process (user code) defines how to handle the signal (ignore it, default action, or run a specific C function). If a handler is used, the code within that handler executes in user mode. When the handler returns, control passes back to the kernel (via a special trampoline code) to restore the process context and resume the main program.

### 4. Default Actions vs. Custom Handlers
Upon delivery, the kernel executes a specific action based on the signal's **disposition**:
*   **Default Actions:** Every signal has a default action defined by the kernel. These include:
    *   **Term:** Terminate the process (e.g., `SIGTERM`, `SIGKILL`).
    *   **Core:** Terminate and produce a core dump for debugging (e.g., `SIGSEGV`, `SIGQUIT`).
    *   **Ign:** Ignore the signal (e.g., `SIGCHLD` is ignored by default).
    *   **Stop:** Suspend execution (e.g., `SIGSTOP`).
    *   **Cont:** Resume execution if stopped (e.g., `SIGCONT`).
*   **Custom Handlers:** A program can use `sigaction()` (or the older `signal()`) to change the disposition. The program can choose to Ignore the signal (except `SIGKILL`/`SIGSTOP`) or provide the address of a programmer-defined function (handler) to run.

### 5. Synchronous vs. Asynchronous Signals
*   **Synchronous:** Generated by the execution of a specific machine-language instruction. Examples include hardware exceptions like `SIGSEGV` (invalid memory reference) or `SIGFPE` (floating-point exception), or a process sending a signal to itself via `raise()`. These are delivered immediately; the predictability of delivery is precise.
*   **Asynchronous:** Generated by external events unrelated to the current instruction flow. Examples include the user typing Control-C (`SIGINT`), a timer expiring (`SIGALRM`), or a child process exiting (`SIGCHLD`). The process cannot predict exactly when these will interrupt its execution.

### 6. Process and Thread Signal Model
While `fork()` and `exec()` control process attributes, threads introduce complexity:
*   **Signal Dispositions are Process-Wide:** All threads in a process share the same signal handlers. If one thread changes the handler for `SIGINT`, it changes for all threads.
*   **Signal Masks are Per-Thread:** Each thread has its own signal mask. Thread A can block `SIGINT` while Thread B unblocks it.
*   **Delivery Rules:**
    *   **Thread-Directed:** Signals generated by a hardware fault (like `SIGSEGV`) or specific thread-kill functions (`pthread_kill`) are delivered to the specific thread that caused/targeted them.
    *   **Process-Directed:** Signals sent to the process ID (e.g., via `kill()`) are handled by **one** arbitrarily selected thread that does not currently have the signal blocked.

### 7. Signal Masks and Blocking
The signal mask is a set of signals that are currently blocked.
*   **Behavior:** If a signal is generated but is in the blocked mask, it remains in the **pending** state. It is not discarded (unless it is a standard signal generated multiple times, in which case extras are lost). It will be delivered once the signal is unblocked.
*   **Manipulation:** A process modifies its mask using `sigprocmask()` (or `pthread_sigmask()` for threads). You can add, remove, or set the mask.
*   **Critical Sections:** Blocking is essential for preventing race conditions. You block signals to protect critical code sections from being interrupted by a handler.

### 8. Interaction with System Calls (EINTR, Restart)
A conflict arises when a signal handler interrupts a "slow" system call (like `read()` from a terminal or `pause()`):
*   **Interruption:** By default, if a handler catches a signal while a system call is blocked, the system call fails, returning `-1`, and sets `errno` to `EINTR`.
*   **Manual Restart:** The programmer must manually check for `EINTR` and loop to call the function again.
*   **Automatic Restart (`SA_RESTART`):** When establishing a handler with `sigaction()`, you can set the `SA_RESTART` flag. This tells the kernel to automatically restart certain interrupted system calls (like `read`, `write` on slow devices, `wait`) instead of returning `EINTR`. Note that some system calls (like `nanosleep`, `select`, `poll`) are **never** restarted, regardless of this flag.

### 9. Signal Queuing and Real-time Signals
*   **Standard Signals (1-31):** Not queued. If `SIGINT` is blocked and generated 3 times, unblocking it results in only **one** `SIGINT` delivery.
*   **Real-time Signals:** Defined in POSIX.1b to address limitations.
    *   **Queuing:** Multiple instances of the same real-time signal are queued and delivered.
    *   **Data Payload:** Can carry an integer or pointer value (`sigqueue()`).
    *   **Ordering:** Delivered in ascending order of signal number (lowest number = highest priority). Standard signals have undefined order.
    *   **Range:** Linux provides 32 real-time signals (typically 32 to 63).

### 10. Performance and Reliability Implications
*   **Reliability:** Use `sigaction()` instead of `signal()`. Early UNIX `signal()` semantics were "unreliable" (disposition reset to default after handling, leading to race conditions where signals could kill the process before the handler re-established itself). `sigaction()` provides "reliable" signals where the kernel blocks the signal while its handler is running.
*   **Reentrancy:** Signal handlers are asynchronous. If a handler interrupts `malloc()` (which uses global locks) and then calls `malloc()` itself, the program can deadlock or corrupt memory. Handlers must only call **async-signal-safe** functions (functions that are reentrant or not interruptible).
*   **Global Variables:** When sharing globals between a handler and the main program, use the `volatile sig_atomic_t` type to ensure atomic reads/writes.
*   **Performance:** Signals are expensive. They involve context switches and kernel bookkeeping. High-frequency signaling (like using signals for intense IPC) is inefficient compared to pipes or shared memory.




 Signals are not instantaneous. A pending signal is delivered only when the receiving process switches from kernel mode back to user mode (e.g., returning from a system call or being rescheduled

 Concept: Reentrancy
• Explanation: A function is reentrant if it can be safely interrupted and called again by another thread (or signal handler) before the first call completes.
• Constraint: Signal handlers must only call async-signal-safe functions. For example, printf() is unsafe because it uses internal buffers/locks. If a program is interrupted inside printf and the signal handler calls printf again, the buffers may become corrupted.



If the Child (or Parent) tries to Write to that memory:

The CPU triggers a "Page Fault" (Write protection error).

The Kernel pauses, allocates a new physical page, copies only that specific 4KB page, and updates the Page Table.

It marks the page as Read/Write and resumes.


------>     RTOS vs GPOS (Real-Time vs General Purpose)     <------

----------------------------------------------------------------------------------------------------------------------------
Feature             RTOS (Real-Time OS)                         GPOS (General-Purpose OS)
----------------------------------------------------------------------------------------------------------------------------
Determinism         Highly Deterministic (Fixed response time)  Non-deterministic (Variable response)
Goal                Meet strict timing deadlines                Minimize average response time / Max throughput
Scheduling          Priority-based Preemptive                   Fairness-based (e.g., Round Robin, CFS)
Interrupt Latency   Very Low and bounded                        Higher and variable
Examples            FreeRTOS, QNX, VxWorks                      Linux, Windows, macOS
----------------------------------------------------------------------------------------------------------------------------

1. Hard Real-Time: A system where missing a single deadline results in total system failure (e.g., Car Airbag, Pacemaker).
2. Soft Real-Time: A system where missing a deadline degrades performance but is acceptable (e.g., Video Streaming, Gaming).

Analogy: A GPOS is like a massive department store (efficient for many people), while an RTOS is like an emergency room (prioritizes high-stakes patients immediately).


------>     Interrupt Handling in Embedded Systems     <------

In embedded systems, an Interrupt Service Routine (ISR) is a callback function called by the hardware.

Key Constraints of ISRs:
- Keep it short: Excessive time in ISR blocks other interrupts (Jitter).
- No blocking/sleeping: ISRs cannot call functions that might sleep (e.g., malloc, printf, mutex_lock).
- No Floating Point: On many architectures, the ISR doesn't save/restore FPU registers.

Top Half vs. Bottom Half (Linux context):
- Top Half: The actual ISR. Performs minimum work (clears hardware flags, schedules bottom half) and returns. Runs with interrupts disabled.
- Bottom Half: Performs the heavy lifting (data processing, copying). Runs with interrupts enabled.

Types of Bottom Halves:
1. Softirqs: High-priority, runs in interrupt context, must be reentrant (can run on multiple CPUs).
2. Tasklets: Built on top of Softirqs, but easier to use; guaranteed to run on only one CPU at a time.
3. Workqueues: Runs in process context (a kernel thread). Can sleep! Used for long-running tasks.
4. Threaded IRQs: Modern Linux approach where the ISR simply wakes up a dedicated high-priority thread to do the work.


------>     Priority Inversion      <------

Priority Inversion occurs when a high-priority task is blocked by a low-priority task that is holding a resource (like a mutex), and a medium-priority task preempts the low-priority task, indefinitely delaying the high-priority task.

The Mars Pathfinder Bug: A classic real-world example of priority inversion on a 1997 Mars mission caused the spacecraft to reset repeatedly.

Solutions:
1. Priority Inheritance Protocol (PIP): The low-priority task "inherits" the priority of the high-priority task it is blocking until it releases the resource. This prevents medium-priority tasks from preempting it.
2. Priority Ceiling Protocol (PCP): Each resource is assigned a priority ceiling (the highest priority of any task that might use it). A task can only acquire a resource if its priority is strictly higher than the ceilings of all resources currently held by other tasks.


------>     Embedded Memory & Performance      <------

1. DMA (Direct Memory Access):
Allows hardware peripherals (e.g., SPI, Ethernet) to transfer data directly to/from RAM without involving the CPU. This saves CPU cycles but introduces Cache Coherency issues.

2. Cache Coherency:
If the CPU reads a RAM location that was updated by DMA, it might read stale data from its Cache.
Solution: Invalidate cache (before CPU read) or Flush cache (after CPU write).

3. Memory Barriers (Fences):
Embedded CPUs and compilers reorder instructions for performance. A Memory Barrier ensures all memory operations before the barrier are completed before any after it.
Example: `__sync_synchronize()` in GCC.

4. Register Access:
Accessing hardware registers is done via pointers to fixed physical addresses.
```c
#define UART_BASE 0x4000C000
#define UART_DR   (*(volatile uint32_t *)(UART_BASE + 0x00))
```
Note: `volatile` is mandatory to prevent the compiler from optimizing out repeated reads from the same address.


------>     Linux/Embedded Boot Flow      <------

1. Reset Vector: CPU starts at a fixed address (e.g., 0xFFFF0) and executes the first instruction.
2. ROM Bootloader (Primary): Small code burnt into the silicon that initializes basic RAM and loads secondary bootloader.
3. Secondary Bootloader (e.g., U-Boot):
   - Initializes DDR RAM, Clocks, and Storage.
   - Loads the Kernel Image (zImage/uImage) into RAM.
   - Passes Boot Arguments (bootargs) and Device Tree (DTB) to the kernel.
4. Kernel Initialization:
   - Self-decompressing kernel.
   - Sets up virtual memory (Paging) and Interrupt vectors.
   - Mounts the Root File System (initramfs or eMMC/SD partition).
5. User Space (Init): Kernel executes the first user-space process (`/sbin/init` or systemd), which starts services.


------>     Atomic Operations & Concurrency      <------

1. Atomic Operations:
Instructions that complete in a single, uninterruptible step (e.g., `LDREX`/`STREX` on ARM). They are the foundation of locks.

2. Mutex vs. Spinlock (Embedded context):
- Mutex: Puts the thread to sleep if the lock is held. Better for long critical sections. Cannot be used in ISRs.
- Spinlock: Busy-waits (spins) if the lock is held. Better for very short critical sections. MANDATORY for ISR-to-Thread synchronization.

3. Lock-free Programming:
Using atomic operations like Compare-And-Swap (CAS) to update shared data without using traditional locks, avoiding Deadlocks and Priority Inversion.
------>     Linux Device Driver Architecture      <------

In Linux, "Everything is a file." Device drivers bridge the gap between hardware and the filesystem.

1. Major and Minor Numbers:
- Major Number: Identifies the driver associated with the device (e.g., 1 for RAM, 4 for TTY).
- Minor Number: Identifies the specific device instance handled by that driver.

2. The file_operations Structure:
The heart of a character driver. It maps standard system calls (open, read, write, ioctl) to driver-specific functions.
```c
struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = my_driver_read,
    .write   = my_driver_write,
    .unlocked_ioctl = my_driver_ioctl,
    .open    = my_driver_open,
    .release = my_driver_close,
};
```

3. Character vs. Block Devices:
- Character Devices: Access data as a stream of bytes (e.g., Keyboard, Serial port).
- Block Devices: Access data in fixed-size blocks, supports seeking (e.g., Hard drive, SD card).


------>     User-Kernel Space Copying      <------

Kernel memory and User-space memory are strictly isolated for security. A driver cannot simply dereference a user-space pointer.

1. copy_to_user(void __user *to, const void *from, unsigned long n):
- Used in the read syscall to send data from a kernel buffer to a user-space buffer.
- Returns the number of bytes that could NOT be copied (Success = 0).

2. copy_from_user(void *to, const void __user *from, unsigned long n):
- Used in the write syscall to receive data from user-space into a kernel buffer.

Why use these?
- Memory safety: Prevents kernel from crashing if the user passes an invalid pointer.
- Paging: Handles cases where the user-space page is swapped out to disk or not yet allocated (Page Faults).

------>     Device Trees (DT)      <------

In many embedded architectures (like ARM and RISC-V), hardware is not self-discoverable (unlike PCI). The Device Tree is a data structure that describes the hardware layout to the OS kernel.

1. Source and Binary:
- .dts: Device Tree Source (human-readable text).
- .dtb: Device Tree Blob (compiled binary passed to the kernel by the bootloader).
- dtc: Device Tree Compiler (converts .dts to .dtb).

2. Structure:
Nodes represent devices, and properties represent configurations.
```dts
uart@4000C000 {
    compatible = "arm,pl011";
    reg = <0x4000C000 0x1000>;
    interrupts = <33>;
    status = "okay";
};
```
- compatible: Used by the kernel to find the correct driver (the "matching" process).
- reg: Base address and size of the device registers.


------>     Power Management      <------

Power efficiency is critical in battery-operated embedded devices.

1. System Sleep States:
- S0: Working state.
- S3: Suspend to RAM (Sleep). Context is saved in RAM; most hardware is off.
- S4: Suspend to Disk (Hibernate). Context is saved on non-volatile storage.

2. DVFS (Dynamic Voltage and Frequency Scaling):
Adjusting the CPU clock speed and voltage based on the current workload to save power during idle periods.

3. Idle States (C-states):
The CPU enters low-power "C-states" when there are no tasks in the run queue. Deeper C-states save more power but have longer wakeup latencies.


------>     Kernel Modules (LKM)      <------

Loadable Kernel Modules allow the kernel to be extended at runtime without rebooting.

1. Management Commands:
- insmod: Loads a module manually.
- modprobe: Loads a module and its dependencies.
- lsmod: Lists currently loaded modules.
- rmmod: Removes a module.

2. Module Lifecycle:
- module_init(): Macro defining the entry point called when the module is loaded.
- module_exit(): Macro defining the exit point called when the module is unloaded.

Analogy: Kernel modules are like "plugins" for an application; they add features only when needed.

------>     Reliability: Watchdogs & BOD      <------

In embedded systems, hardware reliability mechanisms prevent the system from staying in a "hung" or "deadlocked" state.

1. Watchdog Timer (WDT):
A hardware timer that resets the system if it is not "kicked" or "fed" (reset) by the software within a specific interval.
- Purpose: Recovers the system from software crashes, infinite loops, or deadlocks.
- Windowed Watchdog: Requires the "kick" to happen within a specific time window (not too early, not too late), providing even stricter reliability.

2. Brownout Detection (BOD):
A hardware circuit that monitors the supply voltage (VCC).
- Purpose: If the voltage drops below a safe threshold, the BOD triggers a system reset. This prevents the CPU from executing "garbage" instructions due to unstable power, which could corrupt memory or storage.


------>     Firmware Updates (OTA)      <------

Over-the-Air (OTA) updates are critical for fixing bugs and adding features after deployment.

1. A/B Partitioning (Dual-Bank Update):
The system has two identical sets of partitions: Slot A (Active) and Slot B (Inactive).
- Process: The update is written to Slot B while the system runs on Slot A. After a successful write and verification, the bootloader is instructed to boot from Slot B on the next reset.
- Rollback: If Slot B fails to boot, the bootloader can automatically revert to Slot A, ensuring the device remains functional.

2. Bootloader's Role:
The bootloader (like U-Boot) must verify the integrity (checksum/hash) and authenticity (digital signature) of the firmware image before jumping to it.


------>     Embedded Security      <------

Security in embedded systems focuses on protecting the device's intellectual property and data.

1. Secure Boot:
A cryptographic process ensuring that only signed, authorized code can execute on the device.
- Chain of Trust: The CPU starts from a "Root of Trust" (immutable code in ROM) that verifies the first bootloader, which in turn verifies the next stage (Kernel), and so on.

2. Trusted Execution Environment (TEE):
A secure area of the main processor (e.g., ARM TrustZone) that runs in parallel with the main OS (Rich OS like Linux).
- Purpose: Handles sensitive tasks like encryption keys, biometric data, and DRM, completely isolated from potential vulnerabilities in the main OS.

3. Hardware Security Module (HSM) / SE (Secure Element):
A dedicated chip or hardware block used for secure key storage and cryptographic operations (AES, RSA, ECC).
