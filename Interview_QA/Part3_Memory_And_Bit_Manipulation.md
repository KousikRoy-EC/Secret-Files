# Part 3: Memory and Bit Manipulation

## Q1: C Program Memory Layout Details

A C program's compiled image is loaded into RAM with a specific layout structure.

```
       High Memory Address (0xFFFFFFFF)
      +----------------------------------+
      |  Environment Variables & Args    |
      +----------------------------------+
      |  Stack Segment (Grows Downward)  |
      |   - Local variables, frames      |
      |   |                              |
      |   v                              |
      |                                  |
      |   ^                              |
      |   |                              |
      |  Heap Segment (Grows Upward)     |
      |   - Dynamic allocations (malloc) |
      +----------------------------------+
      |  Uninitialized Data (.bss)       |
      |   - Global/static (init to 0)    |
      +----------------------------------+
      |  Initialized Data (.data)        |
      |   - Global/static (explicit init)|
      +----------------------------------+
      |  Read-Only Data (.rodata)        |
      |   - String constants / literal   |
      +----------------------------------+
      |  Text / Code Segment             |
      |   - Read-only machine code       |
      +----------------------------------+
       Low Memory Address (0x00000000)
```

### Where initialized and uninitialized variables are stored:
- **`.text` segment:** Contains the compiled binary instructions of the program. Read-only.
- **`.rodata` segment:** Read-only data segment (e.g., `const char *p = "hello"`).
- **`.data` segment (Initialized):** Contains global and static variables that are explicitly initialized in the code. E.g., `int a = 10;`.
- **`.bss` segment (Block Started by Symbol):** Contains global and static variables that are either uninitialized or initialized to zero. E.g., `int b = 0;` or `static int c;`. The OS zeroes out this segment when loading the program.
- **Stack:** Stores function parameters, return addresses, and local variables.
- **Heap:** Allocated dynamically at runtime (via `malloc`, `calloc`).

### Example
```c
int a = 10;        // Stored in .data segment (Initialized global)
int b;             // Stored in .bss segment (Uninitialized global, defaulted to 0)
static int c = 20; // Stored in .data segment (Initialized static)

int main() {
    int d = 30;    // Stored on the Stack (Local variable)
    static int e;  // Stored in .bss segment (Uninitialized static)
    char *p = "Hello"; // "Hello" in .rodata, p (pointer) on Stack
    return 0;
}
```

---

## Q2: Static Memory vs Dynamic Memory Difference

| Feature             | Static Memory Allocation                       | Dynamic Memory Allocation                  |
|---------------------|------------------------------------------------|--------------------------------------------|
| **Allocation Time** | Compile / Load time                            | Runtime (execution time)                   |
| **Storage Area**    | `.data`, `.bss` (or Stack for local variables) | Heap segment                               |
| **Size**            | Fixed size defined at compile-time             | Variable size adjusted at runtime          |
| **Lifetime**        | Duration of block (stack) or program (global)  | Controlled by programmer (`malloc`/`free`) |
| **Access Speed**    | Very fast (direct memory access)               | Slower (requires pointer dereferencing)    |

---

## Q3: Stack vs Heap Memory

```
  Stack: LIFO (Last-In-First-Out)          Heap: Random Access Allocation
  +-----------------------+                +-----------------------+
  | Function A Frame      |                | Chunk 1 (Allocated)   |
  +-----------------------+                +-----------------------+
  | Function B Frame      |                | Free Space            |
  +-----------------------+                +-----------------------+
  | Function C Frame      |                | Chunk 2 (Allocated)   |
  +-----------------------+                +-----------------------+
```

### Key Differences
1. **Management:** 
   - **Stack** is managed automatically by the CPU. Allocation/deallocation happens on function entry/exit.
   - **Heap** is managed manually by the programmer using system calls.
2. **Access speed:** Stack is much faster because it uses a dedicated CPU register (stack pointer) to track allocation, and cache hits are high. Heap requires tracking free lists and page table lookups.
3. **Fragmentation:** Stack has zero fragmentation (always contiguous). Heap can suffer from internal and external fragmentation.

---

## Q4: Stack Overflow vs Segmentation Fault

### Stack Overflow
Occurs when the stack pointer exceeds the bounds allocated for the stack segment (typically a few megabytes on OS, or kilobytes on MCU). Usually caused by infinite recursion or excessively large local arrays.

```c
// Example of Stack Overflow
void recursive_function() {
    int large_array[1000]; // Allocation on stack
    recursive_function();  // Infinite recursion: runs out of stack space
}
```

### Segmentation Fault
Occurs when a program attempts to access a memory region that it has no permission to access (e.g., writing to read-only memory, dereferencing a NULL or invalid pointer).

```c
// Example of Segmentation Fault
int main() {
    int *ptr = NULL;
    *ptr = 10; // Crash: Dereferencing NULL pointer
    
    char *str = "hello";
    str[0] = 'H'; // Crash: Writing to read-only .rodata segment
    return 0;
}
```

---

## Q5: Dangling Pointer, NULL Pointer, and Void Pointer

- **NULL Pointer:** A pointer that points to address `0` (or `(void*)0`). Used to represent that the pointer is uninitialized or invalid.
- **Void Pointer (`void*`):** A generic pointer that can point to any type. Needs casting before dereferencing.
- **Dangling Pointer:** A pointer pointing to a memory location that has been freed or deallocated. Accessing it causes Undefined Behavior.

### How to avoid Dangling Pointer?
Always set the pointer to `NULL` immediately after freeing it.
```c
#include <stdlib.h>

int main() {
    int *ptr = (int*)malloc(sizeof(int));
    *ptr = 42;
    
    free(ptr);
    // ptr is now a DANGLING pointer!
    
    ptr = NULL; // Safe! No longer dangling
    return 0;
}
```

---

## Q6: Memory Leaks

### Answer
A memory leak occurs when a program allocates memory on the heap but fails to release it back to the operating system after it is no longer needed. Over time, memory leaks consume RAM, eventually causing the system to run out of memory (OOM crash).

### Does heap memory get freed automatically?
- **Within execution:** No. Heap memory remains allocated until the programmer explicitly calls `free()`.
- **On Program Exit:** Yes, the OS automatically reclaims the entire heap space allocated to the process when it terminates. (However, in long-running servers or bare-metal embedded systems that run indefinitely, memory leaks are catastrophic).

### How to avoid:
- Match every `malloc`/`calloc` with a corresponding `free`.
- Use static analysis tools (like Coverity) or dynamic analysis tools (like Valgrind).

---

## Q7: `malloc()` vs `calloc()`

| Feature            | `malloc()`                                           | `calloc()`                                |
|--------------------|------------------------------------------------------|-------------------------------------------|
| **Parameters**     | Takes size in bytes: `malloc(size)`                  | Takes count and size: `calloc(num, size)` |
| **Initialization** | Does NOT initialize memory (contains garbage values) | Initializes memory to zero                |
| **Performance**    | Faster (no zeroing-out overhead)                     | Slower (requires writing zeros to memory) |

### Trap: Non-contiguous allocation in calloc?
If `calloc` cannot allocate the requested size as a single contiguous block of virtual memory, it **fails and returns NULL**. It never allocates fragmented memory fragments and links them.

---

## Q8: How `malloc()` Allocates Memory Internally

When a C program calls `malloc()`, it does not immediately execute a hardware request. The C standard library (`glibc`) acts as a memory manager:

```
  User Space Program
         │
         ▼ (malloc request)
  Memory Allocator (ptmalloc/glibc) ─── Maintains Free Lists (Bins)
         │
         ├─── Small allocations (< 128KB) ────► sbrk() / brk() system call
         │                                      (Moves heap boundary up)
         │
         └─── Large allocations (>= 128KB) ───► mmap() system call
                                                (Maps anonymous page from OS)
```

1. **Memory Pools (Bins):** `malloc` maintains lists of free memory blocks (bins: fastbins, unsorted bins, smallbins) to satisfy allocations quickly without invoking system calls.
2. **System Calls:**
   - **`brk` / `sbrk`:** For small allocations, `malloc` uses `sbrk()` to increment the program break, shifting the top of the heap upward.
   - **`mmap`:** For large allocations (usually >128KB), it requests an independent memory mapping from the OS.
3. **Internal vs External Fragmentation:** It manages chunk metadata headers to store the chunk size, preventing overlapping.

### Comparison: malloc vs kmalloc vs vmalloc (Linux Kernel)
- **`malloc()`:** User-space allocator. Allocates memory in virtual space. Memory is not physically contiguous.
- **`kmalloc()`:** Kernel-space allocator. Allocates physically and virtually contiguous memory. Fast, used for driver buffers and descriptors. Limited size.
- **`vmalloc()`:** Kernel-space allocator. Allocates virtually contiguous memory, but physically non-contiguous memory. Slower than `kmalloc`, used for large buffers (e.g., module loading).

---

## Q9: How to Free Memory without using `free()`

### Answer
In standard library systems, you can use `realloc` with a size of `0`:
```c
int *ptr = malloc(10 * sizeof(int));
realloc(ptr, 0); // Acts identically to free(ptr) in C89/C99 (Deprecated in C11/C17)
```
In custom memory managers or bare-metal setups, you can implement a custom stack allocator where you just reset the allocator pointer to a previous marker position.

---

## Q10: Structure Padding and Packing

### Answer
Processors read memory in word-sized chunks (e.g., 32-bit/4 bytes on 32-bit MCU, 64-bit/8 bytes on x86). To maximize access speed, compilers align structure members to memory addresses matching their sizes. This inserts unused gap bytes (padding).

```c
struct Padded {
    char a;      // 1 byte
    // 3 bytes padding
    int b;       // 4 bytes
    char c;      // 1 byte
    // 3 bytes padding
}; // Total size = 12 bytes
```

### Visual Representation of Padded Memory (32-bit boundaries):
```
Word 0: |  a (1B)  | padding | padding | padding |
Word 1: |                 b (4B)                 |
Word 2: |  c (1B)  | padding | padding | padding |
```

### Packing Structures
You can force the compiler to remove padding using `#pragma pack(1)` or `__attribute__((packed))`.

```c
struct __attribute__((packed)) Packed {
    char a;      // 1 byte
    int b;       // 4 bytes
    char c;      // 1 byte
}; // Total size = 6 bytes
```

### Visual Representation of Packed Memory:
```
Word 0: |  a (1B)  |            b (bytes 0-2)    |
Word 1: |  b (b3)  |  c (1B)  |                  |
```

> **Trap:** Accessing unaligned members in a packed struct (like reading `b` from a non-word boundary) requires the CPU to make multiple memory access cycles, degrading performance, or causing a **hardware alignment fault** on some architectures like ARM Cortex-M0.

---

## Q11: Endianness (Byte Ordering)

### Answer
Endianness refers to the sequential order in which bytes of a multi-byte word are stored in computer memory.
- **Little-Endian:** The least significant byte (LSB) is stored at the lowest memory address.
- **Big-Endian:** The most significant byte (MSB) is stored at the lowest memory address.

```
Example: uint32_t a = 0xAABBCCDD; (Base address: 0x100)

Address:       0x100      0x101      0x102      0x103
               +----------+----------+----------+----------+
Little-Endian: |   0xDD   |   0xCC   |   0xBB   |   0xAA   | (LSB first)
               +----------+----------+----------+----------+
Big-Endian:    |   0xAA   |   0xBB   |   0xCC   |   0xDD   | (MSB first)
               +----------+----------+----------+----------+
```

### Questions:
#### Q11a: How `char a[] = {0xAA, 0xBB, 0xCC, 0xDD}` is stored?
Since it is a **byte array** (individual characters), it is stored in declaration order regardless of endianness.
```
Address: 0x100 -> 0xAA, 0x101 -> 0xBB, 0x102 -> 0xCC, 0x103 -> 0xDD
```

#### Q11b: How is a string stored in memory regarding endianness?
Strings are character arrays. Each character is 1 byte. Hence, strings are stored sequentially and are **unaffected by endianness**.

#### Q11c: Does Endianness affect bits or bytes?
Endianness affects **bytes**, not the individual bits within a byte. The ordering of bits inside a single byte is fixed by hardware serializing mechanisms.

#### Q11d: C Program to detect Endianness
```c
#include <stdio.h>

int main() {
    unsigned int x = 1;
    char *c = (char*)&x;
    if (*c) {
        printf("Little-Endian\n");
    } else {
        printf("Big-Endian\n");
    }
    return 0;
}
```

#### Q11e: Convert Endianness using Union and Bitwise
```c
#include <stdio.h>
#include <stdint.h>

// 1. Bitwise Conversion
uint32_t swap_endian_bitwise(uint32_t val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

// 2. Union-Based Conversion
union EndianUnion {
    uint32_t val;
    uint8_t bytes[4];
};

uint32_t swap_endian_union(uint32_t val) {
    union EndianUnion src, dst;
    src.val = val;
    dst.bytes[0] = src.bytes[3];
    dst.bytes[1] = src.bytes[2];
    dst.bytes[2] = src.bytes[1];
    dst.bytes[3] = src.bytes[0];
    return dst.val;
}
```

---

## Q12: Bit Manipulation

### Bitwise Operators Cheat Sheet
- **Set a bit:** `num |= (1 << pos)`
- **Clear a bit:** `num &= ~(1 << pos)`
- **Toggle a bit:** `num ^= (1 << pos)`
- **Check a bit:** `(num & (1 << pos)) != 0`

---

## Q13: Set/Clear Bit 17 & 18 of a Number

```c
#include <stdio.h>
#include <stdint.h>

uint32_t set_bits_17_18(uint32_t num) {
    return num | (1 << 17) | (1 << 18);
}

uint32_t clear_bits_17_18(uint32_t num) {
    return num & ~((1 << 17) | (1 << 18));
}
```

---

## Q14: Replace Bits 2 to 4 of A with B

Given: `A = 0b11011111` (8-bit), `B = 0b101` (3-bit). We want to clear bits 2 to 4 in A and place B there.

### Step-by-Step Masking
1. **Create mask** to clear bits 2 to 4: `~(((1 << 3) - 1) << 2)` = `~(0b111 << 2)` = `~(0b00011100)` = `0b11100011`.
2. **Clear bits** in A: `A & mask`
3. **Shift B** into place: `(B & 0b111) << 2`
4. **Merge**: `(A & mask) | shifted_B`

### Code Implementation
```c
#include <stdio.h>

unsigned char replace_bits(unsigned char A, unsigned char B) {
    unsigned char mask = ~(0x7 << 2); // 0x7 = 0b111
    return (A & mask) | ((B & 0x7) << 2);
}

int main() {
    unsigned char A = 0b11011111; // 0xDF
    unsigned char B = 0b101;      // 0x05
    // Target output: Clear bits [4:2] of A (which are 0b111) and write 0b101 -> 0b11010111
    printf("Result: 0x%02X\n", replace_bits(A, B)); // Outputs 0xD7 (0b11010111)
    return 0;
}
```

---

## Q15: Map A = 11011101 to C = 01011101 using B = 010

Looking at the bit alignment:
- `A` = `1101 1101` (bit 7 is 1, bits 6:4 are `101`)
- `C` = `0101 1101` (bit 7 is 0, bits 6:4 are `101`)
- `B` = `010` (if placed at bits 7:5 or 6:4)

Let's clear bits 7:5 of A and replace them with B:
```c
unsigned char A = 0xDD; // 1101 1101
unsigned char B = 0x02; // 010
// We want to clear bits 7:5 (0xE0) and write B there:
unsigned char C = (A & 0x1F) | (B << 5);
```

---

## Q16: Count Number of Set Bits (Hamming Weight)

### 1. Iterative Method (Brian Kernighan's Algorithm)
Clears the lowest set bit on every loop step. Highly efficient (loops only as many times as there are set bits).
```c
int count_set_bits(uint32_t n) {
    int count = 0;
    while (n) {
        n &= (n - 1); // Clears the lowest set bit
        count++;
    }
    return count;
}
```

### 2. Recursive Method
```c
int count_set_bits_recurse(uint32_t n) {
    if (n == 0) return 0;
    return (n & 1) + count_set_bits_recurse(n >> 1);
}
```

---

## Q17: Reverse Bits of an Integer

### 1. Loop-based Reversal
```c
uint32_t reverse_bits(uint32_t num) {
    uint32_t NO_OF_BITS = sizeof(num) * 8;
    uint32_t reverse_num = 0;
    for (uint32_t i = 0; i < NO_OF_BITS; i++) {
        if ((num & (1 << i))) {
            reverse_num |= (1 << ((NO_OF_BITS - 1) - i));
        }
    }
    return reverse_num;
}
```

### 2. Recursive Reversal
```c
uint32_t reverse_bits_recurse(uint32_t num, int bit_pos) {
    if (bit_pos == 0) return num & 1;
    return ((num & 1) << bit_pos) | reverse_bits_recurse(num >> 1, bit_pos - 1);
}
// Usage: reverse_bits_recurse(val, 31);
```

---

## Q18: Swap Adjacent Bits of a Binary Word

Swap odd and even bits (e.g., bit 0 with bit 1, bit 2 with bit 3).
- **Mask odd bits:** `0x55555555` (0101...) -> shift left by 1.
- **Mask even bits:** `0xAAAAAAAA` (1010...) -> shift right by 1.
- **Merge:** `(even_bits >> 1) | (odd_bits << 1)`

```c
uint32_t swap_adjacent_bits(uint32_t x) {
    return ((x & 0xAAAAAAAA) >> 1) | ((x & 0x55555555) << 1);
}
```

---

## Q19: Reverse Hex number `0x87` to `0xE1` and print in binary

`0x87` = `1000 0111` in binary. Reversing all 8 bits gives `1110 0001` = `0xE1`.

```c
#include <stdio.h>

unsigned char reverse_8bit(unsigned char x) {
    x = ((x & 0xF0) >> 4) | ((x & 0x0F) << 4);
    x = ((x & 0xCC) >> 2) | ((x & 0x33) << 2);
    x = ((x & 0xAA) >> 1) | ((x & 0x55) << 1);
    return x;
}

void print_binary(unsigned char x) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (x >> i) & 1);
    }
    printf("\n");
}

int main() {
    unsigned char val = 0x87;
    unsigned char reversed = reverse_8bit(val);
    printf("Reversed Hex: 0x%02X\n", reversed); // Outputs 0xE1
    printf("Binary: ");
    print_binary(reversed);
    return 0;
}
```

---

## Q20: Reverse 32-bit Hex `0x12345678` to `0x87654321`

Note: This is a **nibble-level reversal** (digit by digit), not a bit-level reversal.
- Digit 1 (`8`) becomes Digit 8.

```c
#include <stdio.h>
#include <stdint.h>

uint32_t reverse_nibbles(uint32_t val) {
    uint32_t res = 0;
    for (int i = 0; i < 8; i++) {
        res = (res << 4) | ((val >> (i * 4)) & 0x0F);
    }
    return res;
}
```

---

## Q21: Binary Gap (Longest consecutive zeros)

Find the longest sequence of consecutive zeros surrounded by 1s. E.g., `9` = `1001` -> gap is 2. `20` = `10100` -> gap is 1.

```c
#include <stdio.h>

int binary_gap(int N) {
    int max_gap = 0;
    int current_gap = -1; // -1 means we haven't seen the first 1 yet
    
    while (N > 0) {
        if (N & 1) {
            if (current_gap > max_gap) {
                max_gap = current_gap;
            }
            current_gap = 0; // reset/start counting
        } else if (current_gap != -1) {
            current_gap++;
        }
        N >>= 1;
    }
    return max_gap;
}
```

---

## Q22: Find if a number is a Power of 2

A power of 2 in binary has exactly one set bit (e.g., `4` = `0100`, `8` = `1000`).
If we perform `n & (n - 1)`, it will clear the only set bit, resulting in `0`.

```c
#include <stdbool.h>

bool is_power_of_two(int n) {
    if (n <= 0) return false;
    return (n & (n - 1)) == 0;
}
```
