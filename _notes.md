# Complete C Programming Interview Guide

### Header Files
```c
#include <stdio.h>   // Input/Output: printf(), scanf()
#include <stdlib.h>  // Memory: malloc(), calloc(), free()
#include <string.h>  // Strings: strlen(), strcpy(), strcmp()
#include <stdbool.h> // Boolean: true, false
#include <math.h>    // Math: sqrt(), pow()
#include <assert.h>  // Debugging: assert()

```

### Format Specifiers

| Specifier | Data Type               |
| --------- | ----------------------- |
| `%d`      | int                     |
| `%f`      | float                   |
| `%c`      | char                    |
| `%s`      | string                  |
| `%lf`     | double                  |
| `%ld`     | long                    |
| `%u`      | unsigned int            |
| `%x`      | hexadecimal (lowercase) |
| `%X`      | hexadecimal (uppercase) |

### Variable Naming Rules

- Names can contain letters, digits, and underscores
- Must begin with a letter or underscore
- Case sensitive (`myVar` ≠ `myvar`)
- Cannot contain whitespace or special characters (!, #, %, etc.)
- Reserved words cannot be used


### Data Type Sizes

#### 32-bit System

| Type      | Size    | Range                                                   |
| --------- | ------- | ------------------------------------------------------- |
| char      | 1 byte  | -128 to 127                                             |
| short     | 2 bytes | -32,768 to 32,767                                       |
| int       | 4 bytes | -2,147,483,648 to 2,147,483,647                         |
| long      | 4 bytes | Same as int                                             |
| long long | 8 bytes | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 |
| float     | 4 bytes | ~7 decimal digits                                       |
| double    | 8 bytes | ~15 decimal digits                                      |
| pointer   | 4 bytes | -                                                       |

#### 64-bit System 

| Type      | Size    |
| --------- | ------- |
| char      | 1 byte  |
| short     | 2 bytes |
| int       | 4 bytes |
| long      | 8 bytes |
| long long | 8 bytes |
| pointer   | 8 bytes |

### Storage Classes

| Storage Class | Scope      | Lifetime  | Default Value | Memory Location |
| ------------- | ---------- | --------- | ------------- | --------------- |
| **auto**      | Local      | Automatic | Garbage       | Stack           |
| **static**    | Local/File | Program   | 0             | Data/BSS        |
| **extern**    | Global     | Program   | 0             | Data/BSS        |
| **register**  | Local      | Automatic | Garbage       | Register/Stack  |

#### Examples:

```c
// auto (default for local variables)
void func() {
    auto int x = 10;  // allocated on stack
}

// static (retains value between calls)
void counter() {
    static int count = 0;
    count++;
    printf("%d\n", count);
}

// extern (shared across files)
// file1.c
int globalVar = 100;

// file2.c
extern int globalVar;

// register (hint for optimization)
void process() {
    register int i;
    for(i = 0; i < 1000000; i++) {
        // fast loop counter
    }
}
```

### Structure Padding

**Rules:**
1. Each member starts at an address that's a multiple of its alignment
2. Compiler adds padding between members if required
3. Total structure size must be a multiple of the largest member's alignment

```c
struct Example1 {
    char c;      // 1 byte
    // 3 bytes padding
    int i;       // 4 bytes
    short s;     // 2 bytes
    // 2 bytes padding
};
// Total: 12 bytes

struct Example2 {
    int i;       // 4 bytes
    char c;      // 1 byte
    short s;     // 2 bytes
    // 1 byte padding
};
// Total: 8 bytes (better layout!)
```

**Key Insight:** Order members by descending size to reduce padding.

---

## Bit Manipulation

### Hexadecimal Basics

#### Conversion Table
| Binary | Hex | Decimal |
| ------ | --- | ------- |
| 0000   | 0   | 0       |
| 0001   | 1   | 1       |
| 0010   | 2   | 2       |
| 0011   | 3   | 3       |
| 0100   | 4   | 4       |
| 0101   | 5   | 5       |
| 0110   | 6   | 6       |
| 0111   | 7   | 7       |
| 1000   | 8   | 8       |
| 1001   | 9   | 9       |
| 1010   | A   | 10      |
| 1011   | B   | 11      |
| 1100   | C   | 12      |
| 1101   | D   | 13      |
| 1110   | E   | 14      |
| 1111   | F   | 15      |

**Example:** `0x1234` = `0001 0010 0011 0100` (2 bytes)

### Bitwise Operators

#### AND (&) - Both bits must be 1
```c
0x0F & 0x3C = 0x0C
  0000 1111
& 0011 1100
  ---------
  0000 1100
```
**Use Case:** Masking/extracting specific bits

#### OR (|) - Either bit is 1
```c
0x0F | 0x30 = 0x3F
  0000 1111
| 0011 0000
  ---------
  0011 1111
```
**Use Case:** Setting specific bits

#### XOR (^) - Bits are different
```c
0x0F ^ 0x55 = 0x5A
  0000 1111
^ 0101 0101
  ---------
  0101 1010
```
**Use Case:** Toggling bits, swapping values

#### NOT (~) - Flip all bits
```c
~0x0F = 0xFFFFFFF0  // (32-bit)
```

#### Left Shift (<<) - Multiply by 2^n
```c
0x03 << 2 = 0x0C
0000 0011 → 0000 1100
// 3 * 4 = 12
```

#### Right Shift (>>) - Divide by 2^n
```c
0x0C >> 2 = 0x03
0000 1100 → 0000 0011
// 12 / 4 = 3
```

### Common Bit Operations

#### Macros
```c
#define SET_BIT(REG, N)     ((REG) |= (1 << (N)))
#define CLR_BIT(REG, N)     ((REG) &= ~(1 << (N)))
#define TOGGLE_BIT(REG, N)  ((REG) ^= (1 << (N)))
#define CHECK_BIT(REG, N)   (((REG) & (1 << (N))) != 0)
```

#### Check if Number is Even/Odd
```c
int isEven(int n) {
    return (n & 1) == 0;  // Last bit 0 → even
}
```

#### Check if Power of 2
```c
bool isPowerOf2(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}
// Example: 8 = 1000, 7 = 0111
// 8 & 7 = 0000 = 0 ✓
```

#### Count Set Bits
```c
int countSetBits(int n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

// Optimized (Brian Kernighan's Algorithm)
int countSetBitsFast(int n) {
    int count = 0;
    while (n) {
        n &= (n - 1);  // Removes rightmost set bit
        count++;
    }
    return count;
}
```

#### Swap Two Numbers (No Temp Variable)
```c
void swap(int *a, int *b) {
    if (a != b) {
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}
```

#### Reverse Bits
```c
unsigned int reverseBits(unsigned int n) {
    unsigned int result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}
```
```c
// Optimized
unsigned int reverseBits(){
     for(int i=31,s=0;s<i;s++,i--){
        int L = ((var >> i) & 1);
        int R = ((var >> s) & 1)
        if(L!=R){
         var = var ^ (1 << i);
         var = var ^ (1 << s);
        }
     }
}
```
#### Find Missing Number in Array
```c
// Array contains 1 to n, one number missing
int findMissing(int arr[], int size) {
    int n = size + 1;
    int xorAll = 0, xorArr = 0;
    
    for (int i = 1; i <= n; i++)
        xorAll ^= i;
    
    for (int i = 0; i < size; i++)
        xorArr ^= arr[i];
    
    return xorAll ^ xorArr;
}
```

### Endianness

**Little Endian:** LSB at lowest address (x86, ARM)
**Big Endian:** MSB at lowest address (Network protocols)

```c
// Check Endianness - Method 1
void checkEndianness() {
    unsigned int x = 1;
    char *c = (char*)&x;
    
    if (*c == 1)
        printf("Little Endian\n");
    else
        printf("Big Endian\n");
}

// Check Endianness - Method 2 (using union)
union EndianCheck {
    unsigned int num;
    char bytes[4];
};

int main() {
    union EndianCheck e;
    e.num = 1;
    
    if (e.bytes[0] == 1)
        printf("Little Endian\n");
    else
        printf("Big Endian\n");
}
```

**Memory Layout Example:**
```
0x12345678 in memory:

Little Endian: 78 56 34 12
Big Endian:    12 34 56 78
```

---

## Pointers

### Pointer Basics

```c
int a = 10;
int *ptr = &a;   // ptr stores address of a
*ptr = 20;       // Modify value at address (a becomes 20)
```

### Pointer Arithmetic

```c
int arr[] = {10, 20, 30, 40};
int *p = arr;

printf("%d\n", *p);      // 10
printf("%d\n", *(p+1));  // 20
printf("%d\n", *++p);    // 20, p now points to arr[1]
printf("%d\n", *p++);    // 20, then p moves to arr[2]
```

**Important:**
- `*++p`: Increment pointer first, then dereference
- `*p++`: Dereference first, then increment pointer

### Pointer and const

| Declaration            | Pointer Modifiable? | Data Modifiable? |
| ---------------------- | ------------------- | ---------------- |
| `const int *ptr`       | ✅ Yes              | ❌ No           |
| `int *const ptr`       | ❌ No               | ✅ Yes          |
| `const int *const ptr` | ❌ No               | ❌ No           |

```c
// Pointer to constant data
const int *ptr1 = &a;
*ptr1 = 20;      // ❌ Error
ptr1 = &b;       // ✅ OK

// Constant pointer
int *const ptr2 = &a;
*ptr2 = 20;      // ✅ OK
ptr2 = &b;       // ❌ Error

// Constant pointer to constant data
const int *const ptr3 = &a;
*ptr3 = 20;      // ❌ Error
ptr3 = &b;       // ❌ Error
```

### Function Pointers

```c
// Declaration
float (*pf)(float);

// Example
float addTax(float price) {
    return price * 1.18;
}

int main() {
    float (*taxFunc)(float);
    taxFunc = addTax;
    
    printf("%.2f\n", (*taxFunc)(100.0));  // 118.00
    printf("%.2f\n", taxFunc(100.0));     // Also valid
}
```

#### Array of Function Pointers
```c
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int main() {
    int (*ops[3])(int, int) = {add, sub, mul};
    
    printf("5 + 3 = %d\n", ops[0](5, 3));  // 8
    printf("5 - 3 = %d\n", ops[1](5, 3));  // 2
    printf("5 * 3 = %d\n", ops[2](5, 3));  // 15
}
```

### Pointer vs Reference (Pass by Reference)

```c
// Pass by pointer (copies pointer)
void changePointer(int *p) {
    int b = 50;
    p = &b;  // Only changes local copy
}

// To modify caller's pointer, use pointer-to-pointer
void changePointerCorrect(int **p) {
    int b = 50;
    *p = &b;  // Modifies caller's pointer
}
```

---

## Memory Management

### Memory Segments

| Segment         | Contains                        | Lifetime             |
| --------------- | ------------------------------- | -------------------- |
| **Code (Text)** | Program instructions            | Permanent            |
| **Data**        | Initialized global/static       | Permanent            |
| **BSS**         | Uninitialized global/static     | Permanent            |
| **Heap**        | Dynamic allocation              | Manual (malloc/free) |
| **Stack**       | Local variables, function calls | Automatic (LIFO)     |

### malloc vs calloc

| Feature        | malloc                     | calloc                         |
| -------------- | -------------------------- | ------------------------------ |
| Syntax         | `malloc(size)`             | `calloc(n, size)`              |
| Initialization | Garbage values             | Zero-initialized               |
| Speed          | Faster                     | Slower (due to initialization) |
| Use Case       | When initializing manually | When need zeros                |

```c
// malloc example
int *arr1 = (int*)malloc(5 * sizeof(int));
if (arr1 == NULL) {
    // Handle allocation failure
}
// arr1 contains garbage

// calloc example
int *arr2 = (int*)calloc(5, sizeof(int));
// arr2 = {0, 0, 0, 0, 0}

// Always free when done
free(arr1);
free(arr2);
```

### malloc vs new (C++)

| Feature        | malloc                         | new                      |
| -------------- | ------------------------------ | ------------------------ |
| Language       | C & C++                        | C++ only                 |
| Type           | Function                       | Operator                 |
| Returns        | `void*` (requires cast in C++) | Typed pointer            |
| Initialization | No                             | Yes (calls constructors) |
| Deallocation   | `free()`                       | `delete` / `delete[]`    |
| Error          | Returns NULL                   | Throws `std::bad_alloc`  |

```c
// C: malloc
int *ptr = (int*)malloc(sizeof(int));
free(ptr);
```

```cpp
// C++: new
int *ptr = new int(10);      // Initialized to 10
delete ptr;

int *arr = new int[5];
delete[] arr;                // Use delete[] for arrays
```

### Shallow vs Deep Copy

#### Shallow Copy
```c
struct Student {
    int id;
    char *name;
};

struct Student s1 = {1, "Alice"};
struct Student s2 = s1;  // Shallow copy
// s1.name and s2.name point to SAME memory
```

#### Deep Copy
```c
void deepCopy(struct Student *dest, struct Student *src) {
    dest->id = src->id;
    dest->name = (char*)malloc(strlen(src->name) + 1);
    strcpy(dest->name, src->name);
}
```

### Memory Leaks

**What causes leaks:**
```c
void leak() {
    int *ptr = malloc(100);
    // Forgot to call free(ptr)
    return;  // Memory lost!
}
```

**Not always a leak:**
```c
void initSystem() {
    static Config *config = malloc(sizeof(Config));
    // Used throughout program lifetime - acceptable
}
```

### Common Memory Errors

#### Double Free
```c
int *ptr = malloc(sizeof(int));
free(ptr);
free(ptr);  // ❌ Undefined behavior!
```

#### Use After Free
```c
int *ptr = malloc(sizeof(int));
*ptr = 10;
free(ptr);
printf("%d", *ptr);  // ❌ Dangling pointer!
```

#### Dangling Pointer
```c
int *danglingPointer() {
    int local = 10;
    return &local;  // ❌ Returns address of stack variable
}
```

---

## Threading & Concurrency

### Creating Threads (C++)

```cpp
#include <thread>
#include <iostream>

void printNumbers(int n) {
    for (int i = 1; i <= n; i++) {
        std::cout << i << " ";
    }
}

int main() {
    std::thread t1(printNumbers, 10);
    t1.join();  // Wait for thread to complete
    return 0;
}
```

### 5 Ways to Create Threads

#### 1. Function Pointer
functor are nothing but function objects which overload the operator (). so when we call the object with () it calls the operator () function.

#### 2. Lambda Function

#### 3. Functor

#### 4. Member Function

#### 5. Static Member Function


### Thread Synchronization

#### Mutex (Mutual Exclusion)
```cpp
#include <mutex>

std::mutex mtx;
int counter = 0;

void increment() {
    for (int i = 0; i < 100000; i++) {
        mtx.lock();
        counter++;
        mtx.unlock();
    }
}
```

#### lock_guard (RAII - Safer)
```cpp
void increment() {
    for (int i = 0; i < 100000; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        counter++;
    }  // Automatically unlocks
}
```

#### Better: Local Accumulation
```cpp
void increment() {
    int local = 0;
    for (int i = 0; i < 100000; i++) {
        local++;
    }
    std::lock_guard<std::mutex> lock(mtx);
    counter += local;  // Lock once, not 100000 times!
}
```

### Deadlock

**Problem:**
```cpp
std::mutex mtx1, mtx2;

void thread1() {
    mtx1.lock();
    // ... work ...
    mtx2.lock();  // Waits for thread2
    // Deadlock!
}

void thread2() {
    mtx2.lock();
    // ... work ...
    mtx1.lock();  // Waits for thread1
    // Deadlock!
}
```

**Solution 1: Lock Multiple Together**
```cpp
void thread1() {
    std::lock(mtx1, mtx2);
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
    // Safe!
}
```

**Solution 2: try_lock with Retry**
```cpp
void thread1() {
    while (true) {
        if (mtx1.try_lock()) {
            if (mtx2.try_lock()) {
                // Got both locks
                mtx2.unlock();
                mtx1.unlock();
                break;
            }
            mtx1.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
```

### Reentrant Functions

**Non-Reentrant (Unsafe for ISR):**
```c
int counter = 0;  // Global variable

void increment() {
    counter++;  // ❌ Not safe if interrupted
}
```

**Reentrant (Safe):**
```c
void increment(int *counter) {
    (*counter)++;  // ✅ Uses parameter, not global
}
```

**Rules for Reentrant Functions:**
- No global/static variables
- No locks/mutexes
- All data passed as parameters
- No calls to non-reentrant functions

### Atomic Operations

```cpp
#include <atomic>

std::atomic<int> counter(0);

void increment() {
    counter++;  // Thread-safe without mutex
}
```

---

## Advanced Topics

### Inline Functions

```c
inline int max(int a, int b) {
    return (a > b) ? a : b;
}

// Compiler may replace calls with function body
```

**Benefits:**
- Reduces function call overhead
- Suitable for small, frequently called functions

**Drawbacks:**
- Increases code size if overused

### Loop Unrolling

```c
// Original
for (int i = 0; i < 100; i++) {
    a[i] = b[i];
}

// Unrolled (2x)
for (int i = 0; i < 100; i += 2) {
    a[i] = b[i];
    a[i+1] = b[i+1];
}
```

**Benefit:** Reduces loop overhead, fewer branch instructions

### Type Qualifiers

#### const
```c
const int MAX = 100;  // Cannot modify
// MAX = 200;  // ❌ Error
```

#### volatile
```c
volatile int sensorValue;
// Tells compiler value can change unexpectedly
// (e.g., by hardware, ISR)
```

#### restrict (C99)
```c
void copy(int *restrict dest, const int *restrict src, size_t n) {
    // Hints that dest and src don't overlap
    // Allows compiler optimizations
}
```

### Macros vs Functions

```c
// Function
int square(int x) {
    return x * x;
}

// Macro
#define SQUARE(x) ((x) * (x))
```

| Feature       | Function      | Macro         |
| ------------- | ------------- | ------------- |
| Type checking | ✅ Yes         | ❌ No          |
| Overhead      | Function call | None (inline) |
| Side effects  | Safe          | Dangerous     |
| Debugging     | Easy          | Difficult     |

**Macro Pitfall:**
```c
int a = 5;
int result = SQUARE(a++);  // ((a++) * (a++)) - Wrong!
```

### Assertions

```c
#include <assert.h>

void process(int *ptr) {
    assert(ptr != NULL);  // Program exits if false (in debug)
    // ... use ptr ...
}
```

**Usage:**
- Check preconditions
- Verify invariants
- Disabled in release builds (`-DNDEBUG`)

### Enum

```c
enum Status {
    SUCCESS = 0,
    ERROR = -1,
    PENDING = 1
};

enum Status checkConnection() {
    return SUCCESS;
}
```

### Typedef

```c
typedef unsigned char uint8_t;
typedef struct {
    int x, y;
} Point;

// Usage
uint8_t byte = 255;
Point p = {10, 20};
```

---

## Interview Questions

### 1. What's the value of `5["abxdef"]`?
```c
printf("%c", 5["abxdef"]);  // Output: 'f'
```
**Explanation:** `a[i]` is equivalent to `*(a + i)`, so `5["abxdef"]` is `*("abxdef" + 5)` = `'f'`

### 2. Can we compile without `main()`?
**Yes**, using custom entry point:
```c
#define main my_entry
int my_entry() {
    printf("Hello\n");
    return 0;
}
```

### 3. Add two numbers without `+`
```c
// Method 1
int add(int x, int y) {
    while (y != 0) {
        int carry = x & y;
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}

// Method 2
int add(int x, int y) {
    return x - (~y) -1;
}

```

### 4. Subtract without `-`
```c
int subtract(int x, int y) {
    return x + (~y) + 1;  // Two's complement
}
```

### 5. Custom `sizeof` using macro
```c
#define sizeof_custom(type) \
    ((char*)(&type + 1) - (char*)(&type))

int var = 10;
printf("%d", sizeof_custom(var));  // 4
```

### 6. String declaration differences
```c
char s1[] = "string";   // ✅ OK - array with size 7
char s2[10];
s2 = "string";          // ❌ Error - can't assign to array
strcpy(s2, "string");   // ✅ OK - copy contents

char *s3 = "string";    // ✅ OK - pointer to string literal
```

### 7. What does `printf` return?
```c
int n = printf("Hello");
printf("%d", n);  // Output: Hello5
```
**Returns:** Number of characters printed

### 8. Infinite loop without `while/for`
```c
// Using goto
LOOP:
    printf("Loop\n");
    goto LOOP;

// Using recursion
void loop() {
    printf("Loop\n");
    loop();
}
```

### 9. Swap nibbles of a byte
```c
unsigned char swapNibbles(unsigned char x) {
    return ((x & 0xF0) >> 4) | ((x & 0x0F) << 4);
}
// 0x12 → 0x21
```

### 10. Delete a specific bit
```c
unsigned int deleteBit(unsigned int num, int pos) {
    unsigned int left = num >> (pos + 1);
    unsigned int right = num & ((1 << pos) - 1);
    return (left << pos) | right;
}
```

---

## Common Functions Implementation

### 1. `atoi` (String to Integer)
```c
int my_atoi(const char *str) {
    int result = 0;
    int sign = 1;
    
    // Skip whitespace
    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;
    
    // Handle sign
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    
    // Convert digits
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return sign * result;
}
```

### 2. `itoa` (Integer to String)
```c
void my_itoa(int num, char *str) {
    int i = 0;
    int sign = num;
    
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    if (num < 0)
        num = -num;
    
    // Extract digits
    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    if (sign < 0)
        str[i++] = '-';
    
    str[i] = '\0';
    
    // Reverse string
    for (int start = 0, end = i - 1; start < end; start++, end--) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
    }
}
```

### 3. `strlen`
```c
size_t my_strlen(const char *str) {
    size_t len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}
```

### 4. `strcpy`
```c
char *my_strcpy(char *dest, const char *src) {
    char *original = dest;
    while ((*dest++ = *src++) != '\0');
    return original;
}
```

### 5. `strcmp`
```c
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}
```

### 6. `strcat`
```c
char *my_strcat(char *dest, const char *src) {
    char *original = dest;
    
    // Move to end of dest
    while (*dest)
        dest++;
    
    // Copy src
    while ((*dest++ = *src++) != '\0');
    
    return original;
}
```

### 7. `memset`
```c
void *my_memset(void *ptr, int value, size_t num) {
    unsigned char *p = (unsigned char*)ptr;
    
    while (num--) {
        *p++ = (unsigned char)value;
    }
    
    return ptr;
}
```

### 8. `memcpy`
```c
void *my_memcpy(void *dest, const void *src, size_t num) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    
    while (num--) {
        *d++ = *s++;
    }
    
    return dest;
}
```

### 9. Print Binary Representation
```c
void printBinary(unsigned int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}
```

---

## Quick Reference

### ASCII Values
- `'a'` to `'z'`: 97 to 122
- `'A'` to `'Z'`: 65 to 90
- `'0'` to `'9'`: 48 to 57

### Convert Case
```c
// Lowercase to uppercase
char c = 'a';
c = c - 32;  // 'A'

// Uppercase to lowercase
char c = 'A';
c = c + 32;  // 'a'
```

### Common Sizes
- 1 byte = 8 bits
- Hexadecimal: Each digit = 4 bits
- `0x1234` = 2 bytes (4 hex digits)

### Compilation Steps
1. **Preprocessor** (.c → .i): Handle `#include`, `#define`, etc.
2. **Compiler** (.i → .s): Generate assembly code
3. **Assembler** (.s → .o): Convert to object code
4. **Linker** (.o → executable): Link libraries, resolve symbols


Source Code (.c)
        |
    Preprocessor
        |
Preprocessed File (.i)
        |
     Compiler
        |
  Assembly Code (.s)
        |
    Assembler
        |
  Object File (.o)
        |
     Linker
        |
   Executable (a.out)


The preprocessor handles all preprocessor directives (lines starting with #), such as #include, #define, and #ifdef. It produces an intermediate file called the preprocessed file.

Key Tasks:
Expand macros defined using #define.
Include the contents of header files (e.g., #include <stdio.h>).
Remove comments from the source code.
Process conditional compilation (#ifdef, #endif, etc.).


The compiler takes the preprocessed file and translates it into assembly code. Assembly code is a low-level, human-readable representation of machine instructions.

Key Tasks:
Syntax analysis: Ensures the code follows the rules of C.
Semantic analysis: Checks data types, scope rules, and consistency.
Optimization: Improves code efficiency without changing functionality.


The assembler converts the assembly code into machine code (binary instructions that the CPU can execute). This process generates an object file.

Key Tasks:
Translate assembly instructions into binary instructions.
Generate relocation information for symbols (e.g., function or variable references).


The linker combines the object file(s) and required libraries into a single executable file.

Key Tasks:
Resolve external symbols: Functions or variables used in one file but defined in another.
Combine multiple object files if the program is split across multiple .c files.
Link standard libraries (e.g., libc for printf, scanf, etc.).



### Pointer Types (Complete Reference)

| Pointer Type           | Description                | Example                            |
| ---------------------- | -------------------------- | ---------------------------------- |
| **Null Pointer**       | Points to nothing (NULL)   | `int *ptr = NULL;`                 |
| **Void Pointer**       | Generic pointer (any type) | `void *ptr;`                       |
| **Wild Pointer**       | Uninitialized pointer      | `int *ptr;` (dangerous!)           |
| **Dangling Pointer**   | Points to freed memory     | After `free(ptr)`, ptr is dangling |
| **Function Pointer**   | Points to function         | `int (*ptr)(int, int);`            |
| **Array Pointer**      | Points to array            | `int (*ptr)[10];`                  |
| **Pointer to Pointer** | Points to another pointer  | `int **ptr;`                       |

#### Null Pointer
```c
int *ptr = NULL;
if (ptr == NULL) {
    printf("Pointer is null\n");
}
```

#### Void Pointer
```c
void *ptr;
int a = 10;
ptr = &a;  // Can point to any type
int *intPtr = (int*)ptr;  // Must cast before dereferencing
```

#### Wild Pointer (Dangerous!)
```c
int *ptr;  // Uninitialized - points to random memory
*ptr = 10;  // ❌ Undefined behavior!
```

#### Dangling Pointer
```c
int *ptr = (int*)malloc(sizeof(int));
*ptr = 100;
free(ptr);  // Memory freed
// ptr is now dangling - points to invalid memory
*ptr = 200;  // ❌ Undefined behavior!
```


### Type Conversion

```c
int main() {
    // Implicit conversion
    int a = 10;
    float b = a;  // int → float
    
    // Explicit conversion (type casting)
    float x = 3.14;
    int y = (int)x;  // y = 3
    
    // Type promotion in expressions
    float sum = (float)2 + 0.3;  // = 2.3
    
    printf("Type conversion: %f\n", sum);
    return 0;
}
```

### Ternary Operator
```c
int time = 20;
(time < 18) ? printf("Good day.") : printf("Good evening.");

// More common usage
int max = (a > b) ? a : b;
```

### Array Reversal with Swap
```c
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int array[] = {1, 2, 3, 4, 5};
    int size = sizeof(array) / sizeof(array[0]);
    
    for (int i = 0; i < size / 2; i++) {
        swap(&array[i], &array[size - i - 1]);
    }
    
    // array is now {5, 4, 3, 2, 1}
    return 0;
}
```

### strlen vs sizeof
```c
int main() {
    char array[] = "Hello world";
    
    printf("%lu\n", strlen(array));  // 11 (excludes '\0')
    printf("%lu\n", sizeof(array));  // 12 (includes '\0')
    
    return 0;
}
```

**Key Difference:**
- `strlen()`: Counts characters until '\0' (runtime)
- `sizeof()`: Total memory allocated (compile-time)

### Jagged Array (Variable Length Columns)
```c
int main() {
    int **jaggedArray;
    int rows = 3;
    
    // Allocate array of pointers
    jaggedArray = (int**)malloc(rows * sizeof(int*));
    
    // Allocate each row with different sizes
    for (int i = 0; i < rows; i++) {
        jaggedArray[i] = (int*)malloc((i + 1) * sizeof(int));
    }
    
    // Row 0: 1 element
    // Row 1: 2 elements
    // Row 2: 3 elements
    
    // Don't forget to free!
    for (int i = 0; i < rows; i++) {
        free(jaggedArray[i]);
    }
    free(jaggedArray);
    
    return 0;
}
```

### Range Overflow Example
```c
int main() {
    char ch = 127;  // Maximum value for signed char
    ch++;
    printf("%d", ch);  // Output: -128 (wraps around)
    
    unsigned int x = UINT_MAX;
    x++;
    printf("%u", x);  // Output: 0 (wraps to minimum)
    
    return 0;
}
```

### Signed vs Unsigned Comparison (Tricky!)
```c
int main() {
    unsigned int a = 1;
    int b = -1;
    
    if (a > b)
        printf("True");
    else
        printf("False");  // Prints False!
    
    return 0;
}
```

**Explanation:** When comparing signed with unsigned, signed is converted to unsigned. `-1` becomes a very large positive number (`0xFFFFFFFF`).

### Tokens in C

**6 Types of Tokens:**
1. **Keywords:** `int`, `if`, `while`, `return`, etc.
2. **Identifiers:** Variable names, function names
3. **Constants:** `42`, `3.14`, `'A'`
4. **Operators:** `+`, `-`, `*`, `/`, `==`, etc.
5. **Special Characters:** `{}`, `[]`, `()`, `;`, `,`
6. **Strings:** `"Hello World"`

### Static Keyword Restrictions

```c
// ❌ Cannot use static for function parameters
void func(static int x) {  // Compilation error!
    // ...
}

// ✅ Can use static inside function
void func(int x) {
    static int count = 0;  // OK
    count++;
}
```

### Virtual Functions & V-Table (C++)

```cpp
class Base {
public:
    virtual void display() {
        cout << "Base\n";
    }
};

class Derived : public Base {
public:
    void display() override {
        cout << "Derived\n";
    }
};
```

**Memory Layout:**
- **V-Table:** Stored in Data Segment (read-only)
- **V-Ptr:** 8 bytes inside each object (points to V-Table)
- **Function Code:** Stored in Code Segment

**Cost:** Each object with virtual functions has +8 bytes overhead (V-Ptr).

### Union Examples

#### Structure Inside Union
```c
typedef struct {
    unsigned char byte0;
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
} Bytes;

union Register {
    unsigned int value;
    Bytes parts;
};

int main() {
    union Register reg;
    reg.value = 0x12345678;
    
    printf("Byte 0: %02X\n", reg.parts.byte0);  // 78 (little-endian)
    printf("Byte 1: %02X\n", reg.parts.byte1);  // 56
    
    return 0;
}
```

#### Union Inside Structure
```c
typedef struct {
    unsigned char type;
    union {
        int number;
        float decimal;
        char text[20];
    } data;
} Packet;

int main() {
    Packet pkt;
    pkt.type = 1;
    pkt.data.number = 2025;
    
    printf("Type: %d, Number: %d\n", pkt.type, pkt.data.number);
    
    return 0;
}
```

### Bit-Fields Warning

```c
// ❌ DON'T USE for network protocols or file headers
typedef struct {
    uint16_t device_id : 12;  // Endian-dependent!
    uint16_t command   : 4;
} Header;  // Order depends on Little/Big Endian

// ✅ DO USE masks and shifts instead
#define DEVICE_ID_MASK  0x0FFF
#define COMMAND_MASK    0xF000
#define GET_DEVICE_ID(x) ((x) & DEVICE_ID_MASK)
#define GET_COMMAND(x)   (((x) & COMMAND_MASK) >> 12)
```

**Warning:** Bit-field ordering is compiler and endian-dependent, making them non-portable for protocols.

### Array Decay

```c
int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    
    // Array "decays" to pointer when passed to function
    void process(int *ptr) {
        // ptr is just a pointer, not an array
        // sizeof(ptr) = 8 bytes (pointer size)
    }
    
    process(arr);  // arr decays to &arr[0]
    
    return 0;
}
```

**Think of it like:** Referring to a book gives you the address of the first page, not a copy of the whole book.

### Float Defaults to Double

```c
float num = 3.14;   // Actually 3.14 is double by default
float num2 = 3.14f; // Use 'f' suffix for float literal

// In printf
printf("%f", num);  // Works because of promotion
```

### Atomic Functions

```c
#include <stdatomic.h>

atomic_int counter = 0;

void increment() {
    counter++;  // Thread-safe without mutex
    // Operation cannot be interrupted
    // Either fully done or not done at all
}
```

**Atomic vs Reentrant:**
- **Atomic:** Operation completes without interruption
- **Reentrant:** Function safe to call from interrupt, uses no globals

### sleep() vs wait()

```c
// sleep() - Does NOT release mutex
mutex.lock();
sleep(1000);  // Holds mutex while sleeping - BAD!
mutex.unlock();

// wait() - Releases mutex while waiting
mutex.lock();
condition.wait(mutex);  // Releases mutex, reacquires when notified
// ... critical section ...
mutex.unlock();
```

| Feature         | sleep()       | wait()                    |
| --------------- | ------------- | ------------------------- |
| Releases mutex? | ❌ No          | ✅ Yes                     |
| Wakes up        | After timeout | When notified             |
| Use case        | Simple delay  | Condition synchronization |

### Semaphore Example

```c
#include <semaphore.h>

sem_t semaphore;

void *printJob(void *arg) {
    sem_wait(&semaphore);  // P operation (decrement)
    
    // Critical section - only N threads can enter
    printf("Thread %ld printing\n", (long)arg);
    sleep(1);
    
    sem_post(&semaphore);  // V operation (increment)
    return NULL;
}

int main() {
    sem_init(&semaphore, 0, 2);  // Allow 2 threads max
    
    pthread_t threads[5];
    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, printJob, (void*)i);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&semaphore);
    return 0;
}
```

### Binary String Addition

```c
int main() {
    char s1[] = "11101";
    char s2[] = "01011";
    char result[100];
    int carry = 0;
    int idx = 0;
    
    int i = strlen(s1) - 1;
    int j = strlen(s2) - 1;
    
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += s1[i--] - '0';
        if (j >= 0) sum += s2[j--] - '0';
        
        result[idx++] = (sum % 2) + '0';
        carry = sum / 2;
    }
    result[idx] = '\0';
    
    // Reverse result
    for (int x = 0, y = idx - 1; x < y; x++, y--) {
        char temp = result[x];
        result[x] = result[y];
        result[y] = temp;
    }
    
    printf("Sum: %s\n", result);  // 101000
    return 0;
}
```

### Debounce Function

```c
// Debounce: Function called only after certain time has passed
// since last invocation

#include <time.h>

#define DEBOUNCE_DELAY 500  // milliseconds

void debounced_action() {
    static clock_t last_call = 0;
    clock_t now = clock();
    
    if ((now - last_call) * 1000 / CLOCKS_PER_SEC > DEBOUNCE_DELAY) {
        printf("Action executed!\n");
        last_call = now;
    } else {
        printf("Action ignored (too soon)\n");
    }
}
```

**Use Case:** Button presses, keyboard input, search-as-you-type

### String Input Methods

```c
int main() {
    char str1[100];
    char str2[100];
    
    // scanf - stops at whitespace
    printf("Enter name: ");
    scanf("%s", str1);  // Reads only "Hello" from "Hello World"
    
    // fgets - reads entire line including spaces
    printf("Enter full name: ");
    fgets(str2, sizeof(str2), stdin);  // Reads "Hello World\n"
    
    // Remove newline from fgets
    str2[strcspn(str2, "\n")] = '\0';
    
    return 0;
}
```

### Pragma Startup (Compiler-Specific)

```c
#include <stdio.h>

void before_main() {
    printf("This runs before main!\n");
}

#pragma startup before_main

int main() {
    printf("Inside main\n");
    return 0;
}
```

**Note:** `#pragma startup` is **not supported by GCC**. It's compiler-dependent (works with Turbo C, some others).

### NULL Macro

```c
// NULL is defined in multiple headers
#include <stdio.h>   // Defines NULL
#include <stddef.h>  // Also defines NULL

// NULL is typically defined as
#define NULL ((void*)0)  // In C
#define NULL 0           // In C++
```

### Specific Bit Manipulation Examples

#### Delete Specific Bit from Number
```c
unsigned int deleteBit(unsigned int num, int pos) {
    unsigned int left = num >> (pos + 1);
    unsigned int right = num & ((1 << pos) - 1);
    return (left << pos) | right;
}

// Example: deleteBit(0b10110, 2) = 0b1010
```

#### Swap 5th and 7th Nibble
```c
int main() {
    unsigned int var = 0x11223344;
    
    unsigned int left = (var & 0x000F0000) << 8;   // 5th nibble
    unsigned int right = (var & 0x0F000000) >> 8;  // 7th nibble
    unsigned int rest = var & 0xF0F0FFFF;          // Other nibbles
    
    unsigned int result = left | right | rest;
    
    printf("0x%08X\n", result);  // 0x31223344
    return 0;
}
```

#### Reverse Bytes (Not Just Bits)
```c
// Reverse entire bytes in 32-bit integer
unsigned int reverseBytes(unsigned int num) {
    return ((num & 0xFF000000) >> 24) |
           ((num & 0x00FF0000) >> 8)  |
           ((num & 0x0000FF00) << 8)  |
           ((num & 0x000000FF) << 24);
}

// 0x12345678 → 0x78563412
```

### Pointer Storage Location Table

| Declaration               | Pointer Storage | Pointed Data | Scope  | Lifetime          |
| ------------------------- | --------------- | ------------ | ------ | ----------------- |
| `int *ptr;` (inside func) | Stack           | -            | Local  | Auto              |
| `static int *ptr;`        | Data/BSS        | -            | Local  | Program           |
| `int *ptr;` (global)      | Data/BSS        | -            | Global | Program           |
| `int *ptr = malloc()`     | Stack           | Heap         | Local  | Heap until free() |

### Complete sizeof Comparison

```c
int arr[10];
int *ptr = arr;

sizeof(arr);  // 40 bytes (10 * 4)
sizeof(ptr);  // 8 bytes (pointer size on 64-bit)

sizeof(arr) / sizeof(arr[0]);  // 10 (array length)
```

### char Type Implementation Dependent

```c
// char can be signed or unsigned (implementation-dependent)

char c = 200;  // What happens?

// If char is signed (most systems):
//   200 is out of range (-128 to 127)
//   Undefined behavior or wraps to negative

// If char is unsigned:
//   200 is valid (0 to 255)

// Always be explicit:
signed char sc = -50;    // -128 to 127
unsigned char uc = 200;  // 0 to 255
```



## Static and DYNAMIC linking

Static linking involves linking libraries at compile-time, resulting in a larger 
executable but with no external dependencies. 

Dynamic linking links libraries at runtime, 
resulting in smaller executables but requiring external library files to run.




The MSB is used as the sign bit:
0 → Positive number
1 → Negative number





log2n n + 1  gives no of integer in a number
1234 - >  4



##  Difference in C and CPP structure

| Feature				        | C Struct		        |  C++ Struct
| Functions inside struct		| ❌ Not allowed		   |  ✅ Allowed
| Constructors/Destructors	    | ❌ Not allowed		   |  ✅ Allowed
| Access specifiers		        | ❌ Only public		   | ✅ Public/Private/Protected
| Typename usage			    | Must write struct      | struct not needed on variable
| Inheritance			        | ❌ Not possible	   | ✅ Possible

Note : The concept of padding is also there for classes and objects in cpp 
       In struct, everything is public unless you specify otherwise.
       In class, everything is private unless you specify otherwise.



Register Keyword with global variable : 

No, you cannot use the register keyword with global variables in C.

🔴 Global variables must:

✔ Have a fixed memory address
✔ Be stored in global memory (data or BSS segment)
✔ Be accessible from other files (linker level)





Can we write C program without main function :

yes but we need to compile src file differently as compiler by default consider main as starting point of program    <<<cc -nostartfiles test.c>>>


Can we write C program without any function : No


Compile time erro happens mainly due to syntax or structure of program.
Run time error happens due to seg fault , bus err within our code etc;

32 bit processor means it can read or write 4 byte of data simultaneouly


memset fills memory with a repeated single byte, not a multi-byte value.
The int argument is converted to unsigned char, and only the lowest 8 bits are used.

memset(buf, 0x12345678, 4) does NOT write
78 56 34 12 to buf. Instead, it writes
78 78 78 78 because only the least significant byte (0x78) is used to fill the memory.

memset fills memory byte-by-byte using a single byte value.
memset(arr, 1, sizeof(int) * n);  -- wrong behaviour as it fills all integer with 01 01 01 01  01 01 01 01  ...



Fragmentation occur only in heap memory not in stack memory because stack memory is managed by compiler itself and it allocate and deallocate memory in LIFO manner so there is no scope of fragmentation.

void func() { char *d = malloc(10); } -> Pointer d is on Stack, 10 bytes are on Heap.


111010100101010 -> 010101001010111
To reverse the bits of the hexadecimal number  without using bitwise operators, you can use a loop to extract each bit and construct the reversed number. Here's a simple C++ program to achieve that:

#include <iostream>
using namespace std;
int reverseBits(unsigned int n) {
    unsigned int reversed = 0;
    for (int i = 0; i < 32; i++) {
        reversed <<= 1;          // Shift reversed to the left
        reversed |= (n & 1);    // Add the least significant bit of n to reversed
        n >>= 1;                // Shift n to the right
    }
    return reversed;
}

int main() {
    unsigned int num = 0x12345678;
    unsigned int reversedNum = reverseBits(num);
    cout << "Original: 0x" << hex << num << endl;
    cout << "Reversed: 0x" << hex << reversedNum << endl;
    return 0;
}
*/


without bitwise operator 

#include <iostream>
using namespace std;
int reverseBits(unsigned int n) {
    unsigned int reversed = 0;
    for (int i = 0; i < 32; i++) {
        reversed = reversed * 2 + (n % 2); // Extract least significant bit and add to reversed
        n = n / 2;                         // Divide n by 2 to shift right
    }
    return reversed;
}
int main() {
    unsigned int num = 0x12345678;
    unsigned int reversedNum = reverseBits(num);
    cout << "Original: 0x" << hex << num << endl;
    cout << "Reversed: 0x" << hex << reversedNum << endl;
    return 0;
}

In C, typedef and static cannot be combined because:

typedef belongs to type definition

static belongs to object storage / linkage


Q: "Can I use printf or malloc in an ISR? Why not?"

Answer: NO. Never.
Reason 1: They are Blocking.

printf writes to the UART console. It might wait for the UART buffer to be empty. An ISR must never wait/sleep. If the ISR sleeps, the kernel panics because there is no process to wake it up.

Reason 2: They use Mutexes (Deadlock Risk).

malloc() {
    lock(heap_mutex);
    find_free_block();
    update_heap_metadata();
    unlock(heap_mutex);
}

The heap is shared global memory.
malloc manages the Heap. To prevent corruption, it uses a Lock.
Scenario:
Main Program calls malloc(). It acquires the Heap Lock.
Interrupt fires! CPU jumps to ISR.
ISR calls malloc().
ISR tries to acquire the Heap Lock... but Main Program already has it.
ISR waits for Main to release it.
BUT Main cannot run until ISR finishes.
Result: DEADLOCK. System hangs.

Reason 3: Non-Deterministic Timing.

malloc searches for a free block. This might take 10ns or 10ms. Real-time systems require predictable timing.


What is the role of assertions in embedded C programming, and how should they be used?

Assertions are statements used to check for conditions that should always be true. In embedded C programming:

assert(pointer != NULL)


Short Integer in C is 32 bit and it can be printed using %hd or %hu.


unsigned with typedef

unsigned is a type specifier, not a general modifier.
It can modify built-in integer types (int, char, long), not typedef names.

typedef int myint;
unsigned myint x;   // ❌ invalid


Why?
👉 myint is a complete type alias, so the compiler can’t “re-modify” it.

typedef unsigned int myint;
myint x;            // ✅ valid


Macro works because:

#define UINT int
unsigned UINT x;    // expands to: unsigned int x




scanf("%s", buf) does not limit input size → buffer overflow risk.
fgets() always knows the buffer size → prevents overflow.



auto in C is storage class while in C++ it is type inference nothing to do with storage location


static function & function pointers

static function → internal linkage (not visible outside the file).
But if its address is passed out, it can still be called via a function pointer.

static void foo(void) { }

void get_fp(void (**fp)(void)) {
    *fp = foo;   // address escapes
}

👉 static restricts linking visibility,
❌ not runtime access or execution.

A single pointer can modify the data it points to, but only a pointer-to-pointer can modify the pointer variable itself.

eg 

void func(int *p) {
    *p = 1;
}

void func2(int **pp) {
    *pp = NULL; // changes p
}

int main() {
    int x = 0;
    int *p = &x;
    func(p);    // changes x
    func2(&p);  // changes p 
    return 0;
}




Static variable and all the global variables are stored in data segment of memory.


If you define static int a; in a header file and include it in two different .c files, the compiler creates two completely separate variables named a.

file.h 
static int a;

file1.c 
#include "file.h"

file2.c 
#include "file.h"

Changes to a in File 1 will not be reflected in File 2. Each file gets its own private copy.


A device tree is a hardware description data structure passed to the Linux kernel at boot time, allowing the kernel to support multiple hardware platforms without board-specific kernel code.
DTS -> dtb (compiled)


Top half and bottom half are two parts of interrupt handling in the Linux kernel.

Top half does the urgent, minimal work when an interrupt occurs.
Bottom half does the deferred, non-urgent work later.

What does process context contain?

1️⃣ CPU register state
Program Counter (PC / IP)
Stack Pointer (SP)
General-purpose registers
Flags / status registers

2️⃣ Memory context
Virtual address space
Page tables
Stack
Heap
Code (text segment)
Data & BSS

3️⃣ Kernel context
Kernel stack
System call state
Open file table references
Signal handlers
Scheduling information

4️⃣ Process control info (PCB / task_struct)
Stored in kernel data structures:
PID
Process state (Running, Ready, Blocked)
Priority
Scheduling policy
CPU affinity




Key Differences

Macro	                                    Inline Function

Preprocessor feature	                    Compiler feature
Text substitution	                        Function semantics
No type checking	                        Type checked
No scope rules	                            Follows C scope rules
No return type	                            Has return type




memcpy and memmove difference is memcpy copies one block of memory from src to dest without overlaping memmove can copy overlapping memory blocks