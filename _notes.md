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
|-----------|-------------------------|
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
|-----------|---------|---------------------------------------------------------|
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
|-----------|---------|
| char      | 1 byte  |
| short     | 2 bytes |
| int       | 4 bytes |
| long      | 8 bytes |
| long long | 8 bytes |
| pointer   | 8 bytes |

### Storage Classes

| Storage Class | Scope      | Lifetime  | Default Value | Memory Location |
|---------------|------------|-----------|---------------|-----------------|
| **auto**      | Local      | Automatic | Garbage       | Stack           |
| **static**    | Local/File | Program   | 0             | Data/BSS        |
| **extern**    | Global     | Program   | 0             | Data/BSS        |
| **register**  | Local      | Automatic | Garbage       | Register/Stack  |

#### Examples:


why cant we declare register variable global ?

REGISTER is only used to suggest the compiler to store the variable in register for fast access. 
1. global variables are stored in the data section which is not a CPU register
2. CPU registers are volatile in nature and are not meant for storing global variables which may be accessed by other threads or processes



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
|--------|-----|---------|
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
#define SET_BIT(REG, N)     ((REG) |= (1U << (N)))
#define CLR_BIT(REG, N)     ((REG) &= ~(1 << (N)))
#define TOGGLE_BIT(REG, N)  ((REG) ^= (1 << (N)))
#define CHECK_BIT(REG, N)   (((REG) & (1 << (N))) != 0)
#define CLEAR_AND_SET(reg, val, pos, width) ((reg) = ((reg) & ~(((1 << (width)) - 1) << (pos))) | ((val) << (pos)))


Write a macro to clear bit from 3 to 6
#define CLEAR_BITS(reg, pos, width) ((reg) &= ~(((1 << (width)) - 1) << (pos)))
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
    uint32_t num;
    uint8_t bytes[4];
};

int main() {
    union EndianCheck e;
    e.num = 0x12345678;
    
    if (e.bytes[0] == 0x78)
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
- ++*p: Increment value at address
- (*p)++: Increment value at address

int *ptr[10] // array of 10 pointers
int(*ptr)[10] // pointer to array of 10 integers
int(*ptr)(int,int) // pointer to function that takes two integers and returns an integer
int(*ptr[10])(int,int) // array of 10 pointers to functions that take two integers and return an integer


### Pointer and const

| Declaration            | Pointer Modifiable? | Data Modifiable? |
|------------------------|---------------------|------------------|
| `int *const ptr`       | ❌ No                | ✅ Yes            |
| `const int *ptr`       | ✅ Yes               | ❌ No             |
| `const int *const ptr` | ❌ No                | ❌ No             |

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
int *ptr[10] // array of 10 pointers
int(*ptr)[10] // pointer to array of 10 integers
int(*ptr)(int,int) // pointer to function that takes two integers and returns an integer
int(*ptr[10])(int,int) // array of 10 pointers to functions that take two integers and return an integer



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
|-----------------|---------------------------------|----------------------|
| **Code (Text)** | Program instructions            | Permanent            |
| **Data**        | Initialized global/static       | Permanent            |
| **BSS**         | Uninitialized global/static     | Permanent            |
| **Heap**        | Dynamic allocation              | Manual (malloc/free) |
| **Stack**       | Local variables, function calls | Automatic (LIFO)     |

### malloc vs calloc

| Feature        | malloc                     | calloc                         |
|----------------|----------------------------|--------------------------------|
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

### Reentrant Functions

A reentrant function in C is a function that can be safely interrupted in the middle of its execution and fully re-entered (called again) before its previous invocation finishes executing.

1. Function does not use any static or global variables
2. Function does not use any shared resources such as files or hardware registers
3. Function does not call any non-reentrant functions
4. Function does not modify any shared resources

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
- call stack not used

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
|---------------|---------------|---------------|
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
#define my_entry main
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
    while (y != 0) {
        int borrow = (~x) & y;
        x = x ^ y;
        y = borrow << 1;
    }
    return x;
}


int subtract(int x, int y) {
    return x + (~y) + 1;  // Two's complement
}
```

### 5. Custom `sizeof` using macro
```c
#define sizeof_custom(T) ((size_t)((T*)0 + 1))
#define SIZE_OF(NUM) ((char *)(&NUM + 1 ) - (char *)(&NUM))




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
int my_atoi(const char *s) {
    int result = 0, sign = 1;
    if (*s == '-') { sign = -1; s++; }
    while (*s >= '0' && *s <= '9')
        result = result * 10 + (*s++ - '0');
    return sign * result;
}

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
        sign = -1;
    
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
    if(str==NULL){
        return 0;
    }
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
    return *(unsigned char*)s1 - *(unsigned char*)s2; // return 0 if strings are equal, negative if s1 < s2, positive if s1 > s2
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
    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    
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
|------------------------|----------------------------|------------------------------------|
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
|-----------------|---------------|---------------------------|
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

// C code for this 0x12345678 → 0x78654321



```

### Pointer Storage Location Table

| Declaration               | Pointer Storage | Pointed Data | Scope  | Lifetime          |
|---------------------------|-----------------|--------------|--------|-------------------|
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


Key Differences

Macro	                                    Inline Function

Preprocessor feature	                    Compiler feature
Text substitution	                        Function semantics
No type checking	                        Type checked
No scope rules	                            Follows C scope rules
No return type	                            Has return type




memcpy and memmove difference is memcpy copies one block of memory from src to dest without overlaping memmove can copy overlapping memory blocks


Use memcpy when you are copying data between two entirely different variables or completely separate buffers. It is faster.

memcpy(dest, src, size);

Use memmove when you are shifting data within the same array or buffer, or anytime you are unsure if the pointers might be looking at the same memory space.

memmove(dest, src, size);




If you use memcpy:
Because it copies front-to-back incredibly fast without thinking:

 eg 

 char str[] = "ABCDEF";
 memcpy(str + 1, str, 5);
 
 // Copies 'A' to index 1. String becomes A ACDEF
 // Tries to copy index 1 to index 2. But index 1 is now 'A'! String becomes A A A DEF
 // Tries to copy index 2 to index 3. It's 'A' again! String becomes A A A A EF Result: Corrupted data ("AAAAAF").



If you use memmove:
It sees the destination address is higher than the source address, so it copies backwards from the end:

 eg

char str[] = "ABCDEF";
memmove(str + 1, str, 5);

// Copies 'D' to index 4. String becomes ABCDEF (unchanged so far)
// Copies 'C' to index 3. String becomes ABCCEF
// Copies 'B' to index 2. String becomes ABBCEF
// Copies 'A' to index 1. String becomes AABCEF Result: Safe, expected data ("AABCEF").






Pragma pack 1 and 2 difference : 

pack(1) → no padding
pack(2) → align to 2-byte boundaries




struct v1
{
    int  a : 2;
    char b : 2;
    int  c : 2;
    char d : 2;
} V1;

struct v2
{
    int  a : 2;
    char b : 2;
    int  c : 2;
    char d;
} V2;

SIZE OF V1 =4
SIZE OF V2 =8 


how ?? 

(BECAUSE regular data type is not a part of bit field Bit-fields occupy one 4-byte unit
char d takes 1 byte
Padding added to satisfy struct alignment (int alignment = 4) )








Convert lowercase → uppercase

Clear bit 5 using AND with ~(1 << 5).

char upper = c & ~(1 << 5);


Convert uppercase → lowercase

Set bit 5 using OR with (1 << 5).

char lower = c | (1 << 5);


---

## Memory Layout: Where Every Variable Lives

### Complete Memory Map (Low to High Address)

```
+---------------------------+  Low Address (0x0000...)
|       Code (Text)         |  Program instructions (read-only)
+---------------------------+
|    Initialized Data       |  Global/static with initial values
|       (.data)             |
+---------------------------+
|   Uninitialized Data      |  Global/static without initial values
|        (.bss)             |     (zero-initialized by OS)
+---------------------------+
| Heap  | Dynamic allocation (grows downward) |
|-------|-------------------------------------|
| ...   |                                     |
|       |                                     |
| Stack | Local variables (grows upward)      |
+---------------------------+  High Address (0xFFFF...)
```

### Where Each Variable Type is Stored

```c
// ========== CODE (TEXT) SEGMENT ==========
// All executable instructions live here
// String literals also live here (read-only)

char *str = "Hello";  // "Hello" is in Code/Text segment (read-only)
                       // str (pointer) is on Stack (if local) or Data (if global)


// ========== DATA SEGMENT (.data) ==========
// Initialized global and static variables

int global_init = 42;              // Data segment
static int static_global_init = 10; // Data segment
void func() {
    static int static_local_init = 5; // Data segment (NOT stack!)
}


// ========== BSS SEGMENT (.bss) ==========
// Uninitialized global and static variables (zero-filled)

int global_uninit;                  // BSS segment (value = 0)
static int static_global_uninit;    // BSS segment (value = 0)
void func() {
    static int static_local_uninit; // BSS segment (value = 0)
}


// ========== STACK SEGMENT ==========
// Local variables, function parameters, return addresses

void func(int param) {             // param on Stack
    int local_var = 10;             // Stack
    int arr[5];                     // Stack (20 bytes)
    char buffer[100];               // Stack (100 bytes)
    int *ptr;                       // Stack (pointer itself, 4/8 bytes)
    const int x = 5;                // Stack (const doesn't change location)
    register int i;                 // Register (hint) or Stack
}


// ========== HEAP SEGMENT ==========
// Dynamically allocated memory

void func() {
    int *p = malloc(sizeof(int));   // p on Stack, *p on Heap
    int *arr = calloc(10, sizeof(int)); // arr on Stack, arr[0..9] on Heap
    char *str = strdup("hello");    // str on Stack, actual string on Heap
}
```

### Tricky Memory Location Questions

```c
// Q1: Where is each part stored?
char *str = "Hello World";
// Answer:
// - str (the pointer variable) on Stack (if local) / Data (if global)
// - "Hello World" (the string literal) in Code/Text segment (read-only)
// - Trying str[0] = 'h' causes Segfault! (modifying read-only memory)


// Q2: What about this?
char str[] = "Hello World";
// Answer:
// - str (the array) on Stack (if local) / Data (if global)
// - "Hello World" is COPIED into the array
// - str[0] = 'h' is VALID! (it is a local copy on stack)


// Q3: Where does each part of a struct live?
struct Node {
    int data;
    struct Node *next;
};

struct Node n1;                  // Entire struct on Stack
struct Node *n2 = malloc(sizeof(struct Node));
// n2 (pointer) on Stack
// *n2 (struct contents: data + next) on Heap


// Q4: What about arrays of pointers?
int *arr[5];                     // Array of 5 pointers on Stack
for(int i = 0; i < 5; i++)
    arr[i] = malloc(sizeof(int)); // Each int on Heap


// Q5: const variables
const int a = 10;                // Stack (local) or Data segment (global)
// NOTE: const does NOT mean it goes to ROM/Flash in hosted systems
// In embedded bare-metal, const globals typically go to Flash/ROM


// Q6: String literal vs character array
char *p = "abc";    // p on Stack, "abc" in Text(read-only)
char a[] = "abc";   // a on Stack, 4 bytes copied: a,b,c,\0
```

### Interview Question: Identify Memory Segments

```c
#include <stdio.h>
#include <stdlib.h>

int g_init = 100;          // Data segment
int g_uninit;              // BSS segment
static int s_init = 200;   // Data segment
static int s_uninit;       // BSS segment
const int g_const = 50;    // Data segment (read-only section)

void foo(int param) {       // param on Stack
    int local = 10;         // Stack
    static int s_local = 5; // Data segment
    int *heap_ptr = malloc(sizeof(int) * 10);  // heap_ptr on Stack, memory on Heap
    char *lit = "hello";    // lit on Stack, "hello" in Text segment
    char arr[] = "hello";   // arr on Stack (6 bytes copied)

    free(heap_ptr);
}

int main() {               // return address on Stack
    foo(42);
    return 0;
}
```

### Stack Frame Layout

```
+---------------------------+  High Address
|   Command-line args       |
|   Environment variables   |
+---------------------------+
|   Return address          |  Pushed by CALL instruction
|   Saved frame pointer     |  Old EBP/RBP
|   Local variable 1        |
|   Local variable 2        |
|   ...                     |
|   Saved registers         |
+---------------------------+  ESP/RSP (Stack Pointer)
```

```c
// Q: What happens to local variables after function returns?
int *bad_function() {
    int x = 42;
    return &x;     // Dangling pointer! x destroyed when function returns
}

// Stack frame is deallocated, x memory is reclaimed
// The pointer returned points to invalid/reused memory
```

---

## Multiline Macros and Advanced Macro Concepts

### Multiline Macros using Backslash

```c
// Simple multiline macro with backslash continuation
#define SWAP(a, b)  \
    do {            \
        int temp = (a); \
        (a) = (b);      \
        (b) = temp;     \
    } while(0)

// Usage:
int x = 5, y = 10;
SWAP(x, y);  // x=10, y=5
```

### Why do-while(0) Pattern?

```c
// WITHOUT do-while(0) -- BROKEN with if-else
#define SWAP_BAD(a, b) \
    { int temp = (a); (a) = (b); (b) = temp; }

if (condition)
    SWAP_BAD(x, y);   // Expands with extra semicolon
else                   // Compiler error! "else without matching if"
    printf("no swap");

// Expands to:
// if (condition)
//     { int temp = (x); (x) = (y); (y) = temp; };  <-- stray semicolon
// else  <-- orphaned else!


// WITH do-while(0) -- CORRECT
#define SWAP_GOOD(a, b) \
    do { int temp = (a); (a) = (b); (b) = temp; } while(0)

if (condition)
    SWAP_GOOD(x, y);  // Works perfectly
else
    printf("no swap");

// Expands to:
// if (condition)
//     do { int temp = (x); (x) = (y); (y) = temp; } while(0);
// else
//     printf("no swap");
```

### Macro with Return Value (GCC Statement Expression)

```c
// GCC statement expression: ({ ... }) returns last expression value
#define MAX(a, b) ({    \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a > _b ? _a : _b;  \
})

int result = MAX(3 + 1, 2 * 2);  // result = 4
// Avoids double evaluation of arguments
```

### Stringification Operator (#)

```c
// # converts macro argument to a string literal
#define TO_STRING(x) #x
#define PRINT_VAR(var) printf(#var " = %d\n", var)

int count = 42;
PRINT_VAR(count);  // printf("count" " = %d\n", count);
                   // Output: count = 42

printf("%s\n", TO_STRING(Hello World));  // Output: Hello World
```

### Token Pasting Operator (##)

```c
// ## concatenates two tokens into one
#define CONCAT(a, b) a##b
#define MAKE_VAR(name, num) name##num

int MAKE_VAR(var, 1) = 10;   // int var1 = 10;
int MAKE_VAR(var, 2) = 20;   // int var2 = 20;

// Practical: Create register access macros
#define REG(name) (*(volatile uint32_t *)(name##_BASE_ADDR))
#define GPIO_BASE_ADDR 0x40020000

REG(GPIO) = 0xFF;  // Expands to: (*(volatile uint32_t *)(GPIO_BASE_ADDR)) = 0xFF;
```

### Variadic Macros (__VA_ARGS__)

```c
// Macros that accept variable number of arguments
#define DEBUG_PRINT(fmt, ...) \
    printf("[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// ## before __VA_ARGS__ removes trailing comma if no args
#define LOG(fmt, ...) \
    fprintf(stderr, fmt, ##__VA_ARGS__)

DEBUG_PRINT("value = %d", x);
// Output: [DEBUG] main.c:25: value = 42

DEBUG_PRINT("checkpoint reached");
// Output: [DEBUG] main.c:26: checkpoint reached
// (## removes the comma before empty __VA_ARGS__)
```

### Predefined Macros

```c
printf("File: %s\n", __FILE__);       // Current filename
printf("Line: %d\n", __LINE__);       // Current line number
printf("Function: %s\n", __func__);   // Current function name (C99)
printf("Date: %s\n", __DATE__);       // Compilation date "Jul 11 2026"
printf("Time: %s\n", __TIME__);       // Compilation time "14:30:00"

// Check C standard version
#if __STDC_VERSION__ >= 201112L
    printf("C11 or later\n");
#elif __STDC_VERSION__ >= 199901L
    printf("C99\n");
#else
    printf("C89/C90\n");
#endif
```

### Conditional Compilation

```c
// Include guards (prevent double inclusion)
#ifndef MY_HEADER_H
#define MY_HEADER_H
// header content
#endif

// Alternative (non-standard but widely supported)
#pragma once

// Feature flags
#define DEBUG_MODE 1

#if DEBUG_MODE
    #define LOG(msg) printf("[LOG] %s\n", msg)
#else
    #define LOG(msg)   // Compiles to nothing
#endif

// Check if macro is defined (regardless of value)
#ifdef FEATURE_X
    // Feature X code
#endif

#ifndef FEATURE_Y
    // Default code when FEATURE_Y not defined
#endif

// #undef - remove a macro definition
#define BUFFER_SIZE 256
// ... use BUFFER_SIZE ...
#undef BUFFER_SIZE
#define BUFFER_SIZE 512  // Redefine with new value
```

### Macro Pitfalls and Interview Questions

```c
// Q1: What is wrong with this macro?
#define SQUARE(x) x * x

int result = SQUARE(3 + 1);
// Expands to: 3 + 1 * 3 + 1 = 3 + 3 + 1 = 7  (NOT 16!)
// Fix: #define SQUARE(x) ((x) * (x))


// Q2: Double evaluation problem
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int x = 5;
int result = MAX(x++, 3);
// Expands to: ((x++) > (3) ? (x++) : (3))
// x gets incremented TWICE if x > 3!


// Q3: Can a macro call itself? (Recursive macro)
#define FOO FOO
int x = FOO;
// Preprocessor detects recursion and stops. Expands to: int x = FOO;
// FOO is NOT expanded again. It stays as the token "FOO"


// Q4: Empty macro
#define NOTHING
int NOTHING x NOTHING = NOTHING 5 NOTHING;
// Expands to: int x = 5; (all NOTHING removed)


// Q5: Macro vs Enum for constants
#define STATUS_OK    0    // No type safety, no scope
#define STATUS_ERR   1

enum Status {             // Type safe, scoped, debuggable
    STATUS_OK_E = 0,
    STATUS_ERR_E = 1
};
```

### X-Macro Pattern (Advanced)

```c
// Define list once, use multiple times
#define ERROR_LIST \
    X(ERR_NONE,    "No error")      \
    X(ERR_TIMEOUT, "Timeout")       \
    X(ERR_OVERFLOW,"Buffer overflow")\
    X(ERR_NULL,    "Null pointer")

// Generate enum
#define X(code, msg) code,
typedef enum {
    ERROR_LIST
    ERR_COUNT  // Automatically counts entries
} ErrorCode;
#undef X

// Generate string array
#define X(code, msg) msg,
const char *error_messages[] = {
    ERROR_LIST
};
#undef X

// Usage:
printf("Error: %s\n", error_messages[ERR_TIMEOUT]);
// Output: Error: Timeout
```

---

## Typedef vs #define Deep Dive

### Key Differences

| Feature       | typedef               | #define                            |
|---------------|-----------------------|------------------------------------|
| Processed by  | Compiler              | Preprocessor                       |
| Scope         | Follows C scope rules | Active until #undef or end of file |
| Type checking | Yes                   | No                                 |
| Pointer types | Handles correctly     | Can be tricky                      |
| Debuggable    | Shows in debugger     | Replaced before compilation        |

### The Pointer Trap

```c
// #define with pointers -- DANGEROUS
#define INT_PTR int*

INT_PTR a, b;
// Expands to: int* a, b;
// a is int*, but b is just int!


// typedef with pointers -- SAFE
typedef int* INT_PTR_T;

INT_PTR_T a, b;
// Both a and b are int*


// Another example
#define CHAR_PTR char*
typedef char* CharPtr;

CHAR_PTR s1, s2;   // char *s1, s2;  s2 is char, NOT char*!
CharPtr s3, s4;     // Both s3 and s4 are char*
```

### typedef with Function Pointers

```c
// Without typedef (hard to read)
void (*signal(int sig, void (*handler)(int)))(int);

// With typedef (much clearer)
typedef void (*SignalHandler)(int);
SignalHandler signal(int sig, SignalHandler handler);

// typedef for callback pattern
typedef int (*Comparator)(const void*, const void*);

int sort(int *arr, int size, Comparator cmp) {
    // Use cmp to compare elements
    return 0;
}
```

### typedef with Structs

```c
// In C, without typedef you must use struct keyword
struct Point {
    int x, y;
};
struct Point p1;   // Must write struct

// With typedef
typedef struct {
    int x, y;
} PointT;
PointT p2;          // No struct keyword needed

// Self-referential struct (linked list node)
typedef struct Node {
    int data;
    struct Node *next;  // Must use struct Node here, not just Node
} Node;
```

### typedef with Arrays

```c
typedef int IntArray5[5];
IntArray5 arr;        // int arr[5];
IntArray5 *ptr;       // int (*ptr)[5]; pointer to array of 5 ints

typedef char String[100];
String name;          // char name[100];
```

---

## C Programming Fundamentals: Frequently Asked Questions

### Operator Precedence Tricky Questions

```c
// Q1: What is the output?
int a = 1, b = 1, c = 1;
int result = a++ + ++b + c++;
// a++ = 1 (post-increment, a becomes 2)
// ++b = 2 (pre-increment)
// c++ = 1 (post-increment, c becomes 2)
// result = 1 + 2 + 1 = 4


// Q2: What is the output?
int x = 5;
int y = x-- + x--;
// UNDEFINED BEHAVIOR! Modifying x twice without sequence point.
// Different compilers may give different results.


// Q3: What is the output?
printf("%d %d %d\n", 10 & 5, 10 | 5, 10 ^ 5);
// 10 = 1010, 5 = 0101
// AND: 0000 = 0, OR: 1111 = 15, XOR: 1111 = 15
// Output: 0 15 15


// Q4: Comma operator
int x = (5, 10, 15);
// Comma operator evaluates left to right, returns rightmost value
// x = 15


// Q5: sizeof with expressions
int x = 5;
printf("%zu\n", sizeof(x++));
// sizeof is compile-time (except VLAs), x++ is NOT evaluated
// Output: 4 (size of int), x is still 5!


// Q6: Short-circuit evaluation
int a = 0, b = 0;
if (a++ && b++) { }
printf("a=%d b=%d\n", a, b);
// a++ evaluates to 0 (false), so b++ is NEVER evaluated (short-circuit)
// Output: a=1 b=0
```

### Sequence Points and Undefined Behavior

```c
// Sequence points: points where all side effects are guaranteed complete
// Sequence points exist at:
// 1. End of full expression (semicolon)
// 2. && || ?: , operators (left operand evaluated first)
// 3. Function call (after arguments evaluated, before function executes)

// UNDEFINED BEHAVIOR (no sequence point between modifications)
int i = 5;
i = i++ + ++i;    // UB: modifying i more than once between sequence points

// DEFINED BEHAVIOR
int i = 5;
int a = i++;       // a=5, i=6 (sequence point at ;)
int b = ++i;       // b=7, i=7 (sequence point at ;)
```

### Storage Class Interview Questions

```c
// Q1: What happens here?
void func() {
    static int x;     // Initialized to 0 (BSS), retains value between calls
    x++;
    printf("%d ", x);
}
// Calling func() 3 times prints: 1 2 3


// Q2: extern with initialization
extern int x = 10;  // Warning! extern with initialization is same as defining
                     // Generally, extern is for declaration, not definition


// Q3: Can you take address of register variable?
register int x = 10;
int *p = &x;  // Error! Cannot take address of register variable


// Q4: What is the scope of static function?
static void helper() { }   // Only visible within this translation unit (.c file)
// Other .c files CANNOT call helper() directly
// But can be called via function pointer if address is shared


// Q5: What is the default storage class for local variables?
void func() {
    int x;          // Default is auto stored on stack
    auto int y;     // Explicitly auto same as above
}


// Q6: extern vs static for global variables
// file1.c
int shared_var = 10;         // External linkage (visible to other files)
static int private_var = 20; // Internal linkage (only this file)

// file2.c
extern int shared_var;       // Can access shared_var
// extern int private_var;   // Linker error! private_var is static
```

### Type Promotion Rules

```c
// Integer promotion: types smaller than int are promoted to int
char a = 10;
char b = 20;
// a + b: both promoted to int, result is int

// Usual arithmetic conversions (when operands differ):
// int + long then long
// int + float then float
// float + double then double
// signed + unsigned (same rank) then unsigned

// Q: What does this print?
unsigned int a = 1;
signed int b = -1;
if (a > b)
    printf("a > b");
else
    printf("a <= b");  // This prints! b promoted to unsigned = 0xFFFFFFFF


// Q: What does this print?
printf("%d\n", sizeof('a'));
// In C: sizeof('a') = sizeof(int) = 4 (char literal is promoted to int)
// In C++: sizeof('a') = sizeof(char) = 1
```

### Comma Operator vs Comma Separator

```c
// Comma OPERATOR: evaluates both, returns right
int x = (1, 2, 3);      // x = 3

// Comma SEPARATOR: in function args, declarations
printf("%d %d", a, b);  // Separator, not operator
int x = 1, y = 2;       // Separator in declaration

// Tricky:
int result = (printf("Hello"), printf("World"), 42);
// Prints "HelloWorld", result = 42
```

### Scope and Lifetime Questions

```c
// Q: What is the output?
int x = 10;
void func() {
    int x = 20;           // Shadows global x
    {
        int x = 30;       // Shadows local x
        printf("%d ", x); // 30
    }
    printf("%d ", x);     // 20
}
// After func(): global x is still 10


// Q: What is the difference?
char *getString1() {
    char *str = "Hello";       // String literal has static duration
    return str;                // Safe because "Hello" lives in Text segment
}

char *getString2() {
    char str[] = "Hello";      // Local array on stack
    return str;                // Dangerous! stack memory will be reclaimed!
}
```

---

## String Operations: Interview Questions

### String Reversal (Multiple Methods)

```c
// Method 1: Two-pointer swap
void reverseString(char *str) {
    if (str == NULL) return;
    int left = 0;
    int right = strlen(str) - 1;
    while (left < right) {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}

// Method 2: Recursive
void reverseRecursive(char *str, int start, int end) {
    if (start >= end) return;
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverseRecursive(str, start + 1, end - 1);
}

// Method 3: Using XOR (no temp variable)
void reverseXOR(char *str) {
    int left = 0, right = strlen(str) - 1;
    while (left < right) {
        str[left] ^= str[right];
        str[right] ^= str[left];
        str[left] ^= str[right];
        left++;
        right--;
    }
}
```

### Reverse Words in a String

```c
// "Hello World" becomes "World Hello"
void reverseWords(char *str) {
    int len = strlen(str);

    // Step 1: Reverse entire string -> "dlroW olleH"
    int left = 0, right = len - 1;
    while (left < right) {
        char t = str[left]; str[left] = str[right]; str[right] = t;
        left++; right--;
    }

    // Step 2: Reverse each word
    int start = 0;
    for (int i = 0; i <= len; i++) {
        if (str[i] == ' ' || str[i] == '\0') {
            int l = start, r = i - 1;
            while (l < r) {
                char t = str[l]; str[l] = str[r]; str[r] = t;
                l++; r--;
            }
            start = i + 1;
        }
    }
}
```

### Check Palindrome

```c
int isPalindrome(const char *str) {
    int left = 0, right = strlen(str) - 1;
    while (left < right) {
        if (str[left] != str[right])
            return 0;  // Not palindrome
        left++;
        right--;
    }
    return 1;  // Palindrome
}
```

### Find First Non-Repeating Character

```c
char firstNonRepeating(const char *str) {
    int freq[256] = {0};

    // Count frequencies
    for (int i = 0; str[i]; i++)
        freq[(unsigned char)str[i]]++;

    // Find first with freq == 1
    for (int i = 0; str[i]; i++) {
        if (freq[(unsigned char)str[i]] == 1)
            return str[i];
    }

    return '\0';  // All characters repeat
}
// firstNonRepeating("aabbcde") returns 'c'
```

### Remove Duplicates from String

```c
void removeDuplicates(char *str) {
    int seen[256] = {0};
    int write = 0;

    for (int read = 0; str[read]; read++) {
        if (!seen[(unsigned char)str[read]]) {
            seen[(unsigned char)str[read]] = 1;
            str[write++] = str[read];
        }
    }
    str[write] = '\0';
}
// "programming" becomes "progamin"
```

### Check if Two Strings are Anagrams

```c
int areAnagrams(const char *s1, const char *s2) {
    int count[256] = {0};

    if (strlen(s1) != strlen(s2))
        return 0;

    for (int i = 0; s1[i]; i++) {
        count[(unsigned char)s1[i]]++;
        count[(unsigned char)s2[i]]--;
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] != 0)
            return 0;
    }
    return 1;
}
// areAnagrams("listen", "silent") returns 1
```

### Implement strstr (Find Substring)

```c
char *my_strstr(const char *haystack, const char *needle) {
    if (*needle == '\0') return (char *)haystack;

    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0')
            return (char *)haystack;  // Found
    }

    return NULL;  // Not found
}
```

### Implement strtok

```c
char *my_strtok(char *str, const char *delim) {
    static char *saved = NULL;

    if (str != NULL)
        saved = str;

    if (saved == NULL || *saved == '\0')
        return NULL;

    // Skip leading delimiters
    while (*saved && strchr(delim, *saved))
        saved++;

    if (*saved == '\0')
        return NULL;

    char *token_start = saved;

    // Find end of token
    while (*saved && !strchr(delim, *saved))
        saved++;

    if (*saved) {
        *saved = '\0';
        saved++;
    }

    return token_start;
}

// Note: strtok is NOT reentrant (uses static variable)
// Use strtok_r for thread-safe version
```

### Count Words in a String

```c
int countWords(const char *str) {
    int count = 0;
    int in_word = 0;

    while (*str) {
        if (*str == ' ' || *str == '\t' || *str == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}
// countWords("  Hello   World  ") returns 2
```

### String Rotation Check

```c
// Is s2 a rotation of s1?
// "waterbottle" is rotation of "erbottlewat"
int isRotation(const char *s1, const char *s2) {
    int len = strlen(s1);
    if (len != (int)strlen(s2) || len == 0)
        return 0;

    // Concatenate s1 with itself and search for s2
    char *doubled = (char *)malloc(2 * len + 1);
    strcpy(doubled, s1);
    strcat(doubled, s1);

    int result = (strstr(doubled, s2) != NULL);
    free(doubled);
    return result;
}
```

---

## Memory Operations: Interview Questions

### Implement memmove (Handles Overlapping)

```c
void *my_memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d < s) {
        // Copy forward (no overlap risk)
        while (n--)
            *d++ = *s++;
    } else if (d > s) {
        // Copy backward (handles overlap)
        d += n;
        s += n;
        while (n--)
            *(--d) = *(--s);
    }
    return dest;
}
```

### Implement realloc

```c
void *my_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (ptr == NULL)
        return malloc(new_size);

    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    void *new_ptr = malloc(new_size);
    if (new_ptr == NULL)
        return NULL;

    size_t copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);

    return new_ptr;
}

// Interview note: real realloc may extend in-place if space available
// realloc(NULL, size) behaves like malloc(size)
// realloc(ptr, 0) behaves like free(ptr) -- implementation defined in C11+
```

### Memory Alignment

```c
// Q: Why does memory alignment matter?
// A: CPU reads memory in word-sized chunks (4 bytes on 32-bit, 8 on 64-bit)
// Misaligned access = 2 reads + shift OR hardware exception on some architectures

// Check if address is aligned
#define IS_ALIGNED(ptr, alignment) (((uintptr_t)(ptr) & ((alignment) - 1)) == 0)

// Align up to next boundary
#define ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))

// Example: ALIGN_UP(13, 4) = 16
// 13 + 3 = 16, 16 & ~3 = 16 & 0xFFFFFFFC = 16
```

### Memory Pool (Embedded Pattern)

```c
// Fixed-size memory pool: no fragmentation, O(1) alloc/free
#define POOL_SIZE 10
#define BLOCK_SIZE 64

typedef struct {
    uint8_t pool[POOL_SIZE][BLOCK_SIZE];
    uint8_t used[POOL_SIZE];
} MemPool;

void pool_init(MemPool *mp) {
    memset(mp->used, 0, sizeof(mp->used));
}

void *pool_alloc(MemPool *mp) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!mp->used[i]) {
            mp->used[i] = 1;
            return mp->pool[i];
        }
    }
    return NULL;  // Pool exhausted
}

void pool_free(MemPool *mp, void *ptr) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (mp->pool[i] == ptr) {
            mp->used[i] = 0;
            return;
        }
    }
}
```

### Stack vs Heap Comparison

| Feature          | Stack                        | Heap                          |
|------------------|------------------------------|-------------------------------|
| Allocation speed | Very fast (move SP)          | Slower (find free block)      |
| Deallocation     | Automatic (function return)  | Manual (free/delete)          |
| Size             | Limited (1-8 MB typical)     | Large (limited by RAM/OS)     |
| Fragmentation    | No                           | Yes                           |
| Thread safety    | Each thread has own stack    | Shared, needs synchronization |
| Access pattern   | LIFO only                    | Random access                 |
| Overflow         | Stack overflow then crash    | Returns NULL                  |
| Direction        | Grows downward (high to low) | Grows upward (low to high)    |

### Stack Overflow Detection

```c
// Q: How to detect stack overflow in embedded systems?
// Method 1: Canary/Sentinel value
#define STACK_CANARY 0xDEADBEEF

void check_stack_overflow() {
    // Place canary at bottom of stack during init
    // Periodically check if canary is corrupted
    extern uint32_t _stack_bottom;
    if (*(volatile uint32_t*)&_stack_bottom != STACK_CANARY) {
        // Stack overflow detected!
    }
}

// Method 2: MPU (Memory Protection Unit) on Cortex-M
// Configure MPU to make guard region at stack bottom as no-access
// Any stack overflow triggers MemManage fault
```

---

## Pointer Operations: Interview Juggling Questions

### Pointer Arithmetic Deep Dive

```c
// Q1: What does this print?
int arr[] = {10, 20, 30, 40, 50};
int *p = arr;
printf("%d\n", *(p + 3));      // 40 (p + 3*sizeof(int) bytes)
printf("%d\n", *(arr + 3));    // 40 (same thing)
printf("%d\n", 3[arr]);        // 40 (arr[3] == *(arr+3) == *(3+arr) == 3[arr])

// Q2: Pointer subtraction
int *p1 = &arr[1];
int *p2 = &arr[4];
printf("%td\n", p2 - p1);     // 3 (NOT 12 bytes, but 3 elements)


// Q3: What does this print?
int arr[] = {1, 2, 3, 4, 5};
int *p = arr;
printf("%d ", *p++);    // 1 (dereference, THEN increment pointer)
printf("%d ", *++p);    // 3 (increment pointer, THEN dereference; p skips 2)
printf("%d ", ++*p);    // 4 (increment the VALUE at p, which was 3)


// Q4: Array name vs pointer
int arr[5] = {1, 2, 3, 4, 5};
int *ptr = arr;

sizeof(arr);    // 20 (total array size)
sizeof(ptr);    // 4 or 8 (pointer size)
// arr is NOT a pointer! It is an array that decays to pointer in most contexts

// You CANNOT do:
// arr++;      arr is not a modifiable lvalue
// arr = ptr;  cannot assign to array

// You CAN do:
ptr++;         // ptr is a modifiable lvalue
```

### Pointer to Array vs Array of Pointers

```c
// Array of pointers
int *arr_of_ptrs[5];    // 5 pointers, each pointing to an int

// Pointer to array
int (*ptr_to_arr)[5];   // ONE pointer, pointing to an array of 5 ints
int nums[5] = {1, 2, 3, 4, 5};
ptr_to_arr = &nums;
printf("%d\n", (*ptr_to_arr)[2]);  // 3

// Key difference:
// int *p[5]   array of 5 pointers (size = 5 * sizeof(int*))
// int (*p)[5] pointer to array of 5 ints (size = sizeof(int*))


// Q: What is the type of &arr vs arr?
int arr[5];
// arr      type is int* (decayed pointer to first element)
// &arr     type is int(*)[5] (pointer to entire array)
// arr + 1  moves by sizeof(int) bytes = 4 bytes
// &arr + 1 moves by sizeof(int[5]) bytes = 20 bytes!

printf("%p\n", (void*)arr);        // 0x1000 (example)
printf("%p\n", (void*)(arr + 1));  // 0x1004
printf("%p\n", (void*)&arr);       // 0x1000 (same address!)
printf("%p\n", (void*)(&arr + 1)); // 0x1014 (moves by 20 bytes!)
```

### Double Pointer (Pointer to Pointer) Patterns

```c
// Q: Why use double pointers?
// A1: To modify a pointer from a function
void allocate(int **pp) {
    *pp = (int *)malloc(sizeof(int) * 10);
}

int main() {
    int *arr = NULL;
    allocate(&arr);  // arr now points to heap memory
    free(arr);
    return 0;
}


// A2: Dynamic 2D array
int **create2D(int rows, int cols) {
    int **arr = (int **)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
        arr[i] = (int *)malloc(cols * sizeof(int));
    return arr;
}

void free2D(int **arr, int rows) {
    for (int i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);
}


// A3: Linked list insertion at head
void insertAtHead(struct Node **head, int data) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;  // Modifies the caller's head pointer
}
```

### Void Pointer Interview Questions

```c
// Q1: Can you do arithmetic on void*?
void *vp = malloc(10);
// vp++;       Invalid in standard C (size of void is unknown)
// vp + 1;     Invalid in standard C
// GCC extension: treats sizeof(void) as 1, so vp+1 moves by 1 byte


// Q2: Can you dereference void*?
int some_int = 42;
void *vp = &some_int;
// *vp;        Cannot dereference void*
// Must cast first:
int val = *(int*)vp;  // Valid


// Q3: Why is void* useful?
// Generic programming in C:
// - malloc/calloc return void*
// - qsort/bsearch use void* for generic data
// - Callback mechanisms

void generic_swap(void *a, void *b, size_t size) {
    unsigned char temp[64]; // Use fixed size or alloca
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

int x = 5, y = 10;
generic_swap(&x, &y, sizeof(int));  // Works for any type!
```

### Complex Pointer Declarations (Spiral Rule)

```c
// Read declarations using the "right-left" or "spiral" rule:
// Start at identifier, go RIGHT, then LEFT, repeat

int *p;                  // p is a pointer to int
int **p;                 // p is a pointer to pointer to int
int *p[10];              // p is an array of 10 pointers to int
int (*p)[10];            // p is a pointer to an array of 10 ints
int (*p)(int);           // p is a pointer to function taking int returning int
int (*p[5])(int);        // p is an array of 5 pointers to functions
                         //   taking int, returning int
int *(*p)(int*, int*);   // p is a pointer to function taking two int*
                         //   returning int*

const int *p;            // p is a pointer to const int (cannot modify *p)
int const *p;            // Same as above
int *const p;            // p is a const pointer to int (cannot modify p)
const int *const p;      // p is a const pointer to const int
```

### Function Pointer Callback Pattern

```c
// Event-driven architecture (common in embedded)
typedef void (*EventHandler)(int event_id, void *data);

typedef struct {
    int event_id;
    EventHandler handler;
} EventEntry;

#define MAX_EVENTS 10
static EventEntry event_table[MAX_EVENTS];
static int event_count = 0;

void register_handler(int event_id, EventHandler handler) {
    if (event_count < MAX_EVENTS) {
        event_table[event_count].event_id = event_id;
        event_table[event_count].handler = handler;
        event_count++;
    }
}

void dispatch_event(int event_id, void *data) {
    for (int i = 0; i < event_count; i++) {
        if (event_table[i].event_id == event_id) {
            event_table[i].handler(event_id, data);
        }
    }
}
```

---

## Bit Manipulation: More Interview Questions

### Extract a Bit Field

```c
// Extract bits from position pos with width bits
#define EXTRACT_BITS(val, pos, width) \
    (((val) >> (pos)) & ((1U << (width)) - 1))

// Example: Extract bits 4-7 from 0xAB (10101011)
uint8_t val = 0xAB;
uint8_t result = EXTRACT_BITS(val, 4, 4);  // 0x0A = 10

// Insert a value into a bit field
#define INSERT_BITS(reg, val, pos, width) \
    ((reg) = ((reg) & ~(((1U << (width)) - 1) << (pos))) | (((val) & ((1U << (width)) - 1)) << (pos)))
```

### Find Position of Only Set Bit

```c
// If number has exactly one set bit, find its position
int findPosition(unsigned int n) {
    if (n == 0 || (n & (n - 1)) != 0)
        return -1;  // Not a power of 2

    int pos = 0;
    while (n > 1) {
        n >>= 1;
        pos++;
    }
    return pos;
}
// findPosition(16) returns 4 (10000)
// findPosition(1) returns 0 (00001)
```

### Find Two Non-Repeating Elements

```c
// All elements appear twice except two. Find those two.
void findTwoUnique(int arr[], int n, int *x, int *y) {
    int xor_all = 0;
    for (int i = 0; i < n; i++)
        xor_all ^= arr[i];

    // xor_all = x ^ y (XOR of the two unique numbers)
    // Find rightmost set bit (differentiating bit)
    int set_bit = xor_all & (-xor_all);

    *x = 0;
    *y = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] & set_bit)
            *x ^= arr[i];
        else
            *y ^= arr[i];
    }
}
```

### Rotate Bits

```c
// Left rotate
uint32_t rotateLeft(uint32_t n, int d) {
    d = d % 32;
    return (n << d) | (n >> (32 - d));
}

// Right rotate
uint32_t rotateRight(uint32_t n, int d) {
    d = d % 32;
    return (n >> d) | (n << (32 - d));
}
// rotateLeft(0x12345678, 8) returns 0x34567812
```

### Turn Off Rightmost Set Bit

```c
// Brian Kernighan trick
unsigned int turnOffRightmost(unsigned int n) {
    return n & (n - 1);
}
// 12 = 1100 becomes 8 = 1000 (turned off bit at position 2)
// 10 = 1010 becomes 8 = 1000 (turned off bit at position 1)
```

### Isolate Rightmost Set Bit

```c
unsigned int isolateRightmost(unsigned int n) {
    return n & (-n);  // Uses two's complement
}
// 12 = 1100 returns 4 = 0100 (isolated bit at position 2)
// 10 = 1010 returns 2 = 0010 (isolated bit at position 1)
```

### Check if Two Integers Have Opposite Signs

```c
int haveOppositeSigns(int x, int y) {
    return (x ^ y) < 0;  // XOR: MSB will be 1 if signs differ
}
```

### Multiply/Divide by Power of 2

```c
int multiplyBy8(int n) { return n << 3; }   // n * 8
int divideBy4(int n)   { return n >> 2; }   // n / 4

// Modulo by power of 2
int mod8(int n)  { return n & 7; }   // n % 8  (7 = 0b111)
int mod16(int n) { return n & 15; }  // n % 16 (15 = 0b1111)
```

### Find Next Power of 2

```c
unsigned int nextPowerOf2(unsigned int n) {
    if (n == 0) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}
// nextPowerOf2(5) returns 8
// nextPowerOf2(13) returns 16
```

### Count Trailing Zeros

```c
int countTrailingZeros(unsigned int n) {
    if (n == 0) return 32;
    int count = 0;
    while ((n & 1) == 0) {
        count++;
        n >>= 1;
    }
    return count;
}
// countTrailingZeros(12) returns 2 (1100 has 2 trailing zeros)
```

### Absolute Value Without Branching

```c
int abs_no_branch(int n) {
    int mask = n >> 31;        // All 1s if negative, all 0s if positive
    return (n ^ mask) - mask;  // If neg: (~n + 1) = -n; If pos: (n - 0) = n
}
```

### Set/Clear/Toggle Multiple Bits at Once

```c
// Set bits 3 through 6 (4 bits starting from position 3)
#define SET_FIELD(reg, pos, width) \
    ((reg) |= (((1U << (width)) - 1) << (pos)))

// Clear bits 3 through 6
#define CLR_FIELD(reg, pos, width) \
    ((reg) &= ~(((1U << (width)) - 1) << (pos)))

// Toggle bits 3 through 6
#define TOGGLE_FIELD(reg, pos, width) \
    ((reg) ^= (((1U << (width)) - 1) << (pos)))

// Example:
uint32_t reg = 0x00;
SET_FIELD(reg, 3, 4);     // reg = 0x78 = 0111 1000
CLR_FIELD(reg, 5, 2);     // Clear bits 5-6
TOGGLE_FIELD(reg, 3, 4);  // Toggle bits 3-6
```

---

## More Frequently Asked Embedded Interview Questions

### volatile Keyword Deep Dive

```c
// Q: When to use volatile?
// 1. Memory-mapped hardware registers
volatile uint32_t *GPIO_PORT = (volatile uint32_t *)0x40020014;
*GPIO_PORT = 0xFF;  // Compiler MUST perform this write

// 2. Variables modified by ISR
volatile int flag = 0;

void ISR_Handler(void) {
    flag = 1;  // Modified in interrupt
}

int main() {
    while (!flag) {
        // Without volatile, compiler may optimize this to while(1)
        // because it does not see flag changing in this function
    }
    printf("Flag set!\n");
    return 0;
}

// 3. Variables shared between threads (though volatile alone is NOT enough
//    for thread safety. You still need atomic or mutex)

// Q: Can a variable be both const and volatile?
const volatile uint32_t *STATUS_REG = (const volatile uint32_t *)0x40020000;
// const: software cannot modify it
// volatile: hardware can change it at any time
// Read-only status register is the perfect use case!

// Q: Does volatile prevent optimization?
// Only for accesses to that variable. Other optimizations still happen.
volatile int x = 10;
int y = x;    // Must read x from memory
int z = x;    // Must read x from memory again (cannot reuse y's value)
```

### Struct Padding Interview Questions

```c
// Q1: What is the size of this struct?
struct A {
    char a;     // 1 byte + 3 padding
    int b;      // 4 bytes
    char c;     // 1 byte + 3 padding
};
// Size = 12 bytes (not 6!)


// Q2: Reorder to minimize padding
struct A_optimized {
    int b;      // 4 bytes
    char a;     // 1 byte
    char c;     // 1 byte + 2 padding
};
// Size = 8 bytes


// Q3: Pack to remove all padding
#pragma pack(push, 1)
struct A_packed {
    char a;     // 1 byte
    int b;      // 4 bytes (misaligned!)
    char c;     // 1 byte
};
#pragma pack(pop)
// Size = 6 bytes, but may have performance penalty (misaligned access)


// Q4: Nested struct padding
struct Inner {
    char x;     // 1 + 3 padding
    int y;      // 4
};  // Size = 8, alignment = 4

struct Outer {
    char a;             // 1 + 3 padding
    struct Inner inner; // 8 bytes (already aligned)
    char b;             // 1 + 3 padding
};
// Size = 16


// Q5: offsetof macro
#include <stddef.h>
printf("Offset of b in struct A: %zu\n", offsetof(struct A, b));  // 4

// Implement offsetof yourself
#define MY_OFFSETOF(type, member) ((size_t)&(((type *)0)->member))
```

### Linked List Operations (Very Common)

```c
// Reverse a linked list
struct Node *reverse(struct Node *head) {
    struct Node *prev = NULL;
    struct Node *curr = head;
    struct Node *next = NULL;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// Detect cycle in linked list (Floyd's algorithm)
int hasCycle(struct Node *head) {
    struct Node *slow = head;
    struct Node *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return 1;  // Cycle detected
    }
    return 0;  // No cycle
}

// Find middle of linked list
struct Node *findMiddle(struct Node *head) {
    struct Node *slow = head;
    struct Node *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;  // Middle node
}
```

### Circular Buffer / Ring Buffer (Embedded Essential)

```c
#define CBUF_SIZE 8  // Must be power of 2 for efficient modulo

typedef struct {
    uint8_t buffer[CBUF_SIZE];
    volatile int head;  // Write index
    volatile int tail;  // Read index
} CircularBuffer;

void cb_init(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
}

int cb_is_full(CircularBuffer *cb) {
    return ((cb->head + 1) & (CBUF_SIZE - 1)) == cb->tail;
}

int cb_is_empty(CircularBuffer *cb) {
    return cb->head == cb->tail;
}

int cb_put(CircularBuffer *cb, uint8_t data) {
    if (cb_is_full(cb)) return -1;
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) & (CBUF_SIZE - 1);  // Efficient modulo
    return 0;
}

int cb_get(CircularBuffer *cb, uint8_t *data) {
    if (cb_is_empty(cb)) return -1;
    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) & (CBUF_SIZE - 1);
    return 0;
}
```

### State Machine Pattern (Embedded)

```c
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event event);

State handle_idle(Event event) {
    switch (event) {
        case EVENT_START: return STATE_RUNNING;
        case EVENT_ERROR: return STATE_ERROR;
        default: return STATE_IDLE;
    }
}

State handle_running(Event event) {
    switch (event) {
        case EVENT_STOP:  return STATE_IDLE;
        case EVENT_ERROR: return STATE_ERROR;
        default: return STATE_RUNNING;
    }
}

State handle_error(Event event) {
    if (event == EVENT_RESET) return STATE_IDLE;
    return STATE_ERROR;
}

// Function pointer table for state handlers
StateHandler state_table[STATE_COUNT] = {
    handle_idle,
    handle_running,
    handle_error,
};

// State machine engine
State current_state = STATE_IDLE;

void process_event(Event event) {
    current_state = state_table[current_state](event);
}
```

### Tricky Output Questions

```c
// Q1: What does this print?
char *ptr = "Hello";
printf("%c\n", *ptr++);    // 'H' (dereference then increment pointer)
printf("%c\n", *ptr);      // 'e' (ptr now points to 'e')


// Q2: What does this print?
int arr[] = {0, 1, 2, 3, 4};
int *p = arr + 4;
printf("%d\n", *p);        // 4
printf("%d\n", p[-2]);     // 2  (same as *(p - 2))


// Q3: What does this print?
int x = 1;
int y = (x++, ++x, x + 10);
// x++ then x=2 (post-increment)
// ++x then x=3 (pre-increment)
// x + 10 = 13
printf("x=%d y=%d\n", x, y);  // x=3 y=13


// Q4: What does this print?
printf("%d\n", printf("Hello"));
// Inner printf prints "Hello" and returns 5 (chars printed)
// Outer printf prints 5
// Output: Hello5


// Q5: sizeof vs strlen for arrays and pointers
char arr[] = "Hello";
char *ptr = "Hello";

printf("%zu %zu\n", sizeof(arr), strlen(arr));  // 6 5
printf("%zu %zu\n", sizeof(ptr), strlen(ptr));  // 4or8 5
// sizeof(arr) = 6 (includes '\0')
// sizeof(ptr) = 4 or 8 (size of pointer, NOT string)


// Q6:
void foo() {
    static int x = 0;
    printf("%d ", ++x);
}
// Calling foo() three times prints: 1 2 3 (static retains value)


// Q7:
int main() {
    int a = 10;
    int *p = &a;
    int **pp = &p;
    printf("%d %d %d\n", a, *p, **pp);
    // Output: 10 10 10
    return 0;
}
```

### Designated Initializers (C99)

```c
// Array designated initializer
int arr[10] = {[0] = 1, [5] = 50, [9] = 99};
// arr = {1, 0, 0, 0, 0, 50, 0, 0, 0, 99}

// Struct designated initializer
struct Point {
    int x, y, z;
};
struct Point p = {.y = 20, .x = 10};
// p = {10, 20, 0}
```

### Flexible Array Member (C99)

```c
// Last member of struct can be incomplete array
typedef struct {
    int length;
    char data[];  // Flexible array member (FAM)
} Packet;

// sizeof(Packet) = sizeof(int), data[] has zero size

// Allocate with variable-length data
Packet *create_packet(int len) {
    Packet *pkt = (Packet *)malloc(sizeof(Packet) + len * sizeof(char));
    pkt->length = len;
    memset(pkt->data, 0, len);
    return pkt;
}

// Access: pkt->data[0] through pkt->data[pkt->length - 1]
```

### Compound Literals (C99)

```c
// Create unnamed objects inline
struct PointCL { int x, y; };

// Pass struct to function without declaring a variable
void print_point(struct PointCL p) {
    printf("(%d, %d)\n", p.x, p.y);
}
print_point((struct PointCL){10, 20});

// Use as pointer
int *p = (int[]){1, 2, 3, 4, 5};
printf("%d\n", p[2]);  // 3
```

### Restrict Keyword Deep Dive

```c
// restrict tells compiler that pointer is the ONLY way to access that memory
// Enables aggressive optimizations

// Without restrict: compiler assumes a and b might overlap
void add_arrays(int *a, int *b, int *result, int n) {
    for (int i = 0; i < n; i++)
        result[i] = a[i] + b[i];
    // Compiler must re-read a[i] and b[i] each time
    // because result might overlap with a or b
}

// With restrict: compiler knows no overlap
void add_arrays_fast(int *restrict a, int *restrict b,
                     int *restrict result, int n) {
    for (int i = 0; i < n; i++)
        result[i] = a[i] + b[i];
    // Compiler can vectorize, reorder, cache values freely
}

// memcpy uses restrict (no overlap allowed)
// void *memcpy(void *restrict dest, const void *restrict src, size_t n);

// memmove does NOT use restrict (overlap is allowed)
// void *memmove(void *dest, const void *src, size_t n);
```

### Inline vs Macro: When to Use What

```c
// Use inline function when:
// - Type safety is needed
// - Debugging is needed (shows in call stack)
// - Complex logic with multiple statements
// - You want scope rules and namespace

// Use macro when:
// - Need stringification (#) or token pasting (##)
// - Conditional compilation (#ifdef, #if)
// - Constants that must work with switch-case
// - Type-generic operations (before C11 _Generic)
// - File/line information (__FILE__, __LINE__)

// Example: Macro is better here (needs __FILE__, __LINE__)
#define ASSERT(cond) \
    do { \
        if (!(cond)) { \
            printf("ASSERT FAILED: %s at %s:%d\n", \
                   #cond, __FILE__, __LINE__); \
        } \
    } while(0)

// Example: Inline is better here (type safety, no double eval)
static inline int min_val(int a, int b) {
    return (a < b) ? a : b;
}
```