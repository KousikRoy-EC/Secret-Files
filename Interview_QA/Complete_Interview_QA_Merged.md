# Embedded Systems & C Programming — Complete Interview Q&A

# 1. C Code Output & Behavior Questions

---

## Q1.1: Pointer Arithmetic with Post-Increment

```c
#include <stdio.h>
int main() {
    int arr[] = {1, 2, 3};
    int *p = arr;
    printf("%d ", *p);
    printf("%d ", *p++);
    printf("%d", *p);
    return 0;
}
```

### Step-by-Step Execution

```
Memory Layout:
arr:  [ 1 ][ 2 ][ 3 ]
       ^
       p (initially points to arr[0])
```

1. **`printf("%d ", *p);`** → `*p` dereferences pointer at `arr[0]`, prints **1**. `p` still points to `arr[0]`.

2. **`printf("%d ", *p++);`** → Post-increment on pointer: `*p++` is parsed as `*(p++)`.
   - The **current value** of `*p` (which is `arr[0]` = 1) is used first.
   - Then `p` is incremented to point to `arr[1]`.
   - Prints **1**.

3. **`printf("%d", *p);`** → `p` now points to `arr[1]`, so `*p` = **2**.

```
After *p++:
arr:  [ 1 ][ 2 ][ 3 ]
            ^
            p (now points to arr[1])
```

### Output
```
1 1 2
```

> **Interview Trap:** `*p++` does NOT increment the value pointed to by `p`. It increments the pointer `p` itself. The `*` and `++` have the same precedence but `++` (postfix) associates left-to-right, so `*p++` ≡ `*(p++)`.

> **Key Precedence Table:**
> | Expression | Meaning | Effect |
> |-----------|---------|--------|
> | `*p++`    | `*(p++)` | Use `*p`, then increment `p` |
> | `(*p)++`  | Increment value at `*p` | Increment the pointed-to value |
> | `++*p`    | `++(*p)` | Pre-increment the pointed-to value |
> | `*++p`    | `*(++p)` | Increment `p` first, then dereference |

---

## Q1.2: Undefined Behavior with `i++ + ++i`

```c
#include <stdio.h>
int main() {
    int i = 3;
    do {
        i = i++ + ++i;
    } while (i < 20);
    printf("%d", i);
    return 0;
}
```

### Answer: **UNDEFINED BEHAVIOR**

This code invokes **undefined behavior** per the C standard (C99/C11 §6.5/2):

> "Between two sequence points, an object shall not have its stored value modified more than once."

In the expression `i = i++ + ++i`:
- `i++` modifies `i` (post-increment)
- `++i` modifies `i` (pre-increment)
- `i =` modifies `i` (assignment)

All three modifications happen **between the same pair of sequence points** (the `;` at the end of the statement), violating the rule.

### Why Different Compilers Give Different Results

| Compiler            | Possible Output           |
|---------------------|---------------------------|
| GCC (some versions) | `20` or `21`              |
| MSVC                | Different value           |
| Clang               | May warn, different value |

> **Interview Trap:** If asked "what is the output?", the correct answer is: **"This is undefined behavior. The output is unpredictable and depends on the compiler."** Any specific numeric answer is technically wrong.

> **Key Takeaway:** Never modify a variable more than once between sequence points. Expressions like `i++ + ++i`, `i = i++`, `a[i] = i++` are all UB.

---

## Q1.3: do-while with Post/Pre Increment

```c
#include <stdio.h>
int main() {
    int i = 0;
    do {
        i++;    // i becomes 1
        ++i;    // i becomes 2
    } while (i++ > 25);  // condition: 2 > 25 = false, then i becomes 3
    printf("%d", i);
    return 0;
}
```

### Step-by-Step Execution

| Step | Operation  | Value of `i` before | Value of `i` after | Notes                                                                             |
|------|------------|---------------------|--------------------|-----------------------------------------------------------------------------------|
| 1    | `i++`      | 0                   | 1                  | Pre/post both give same result when standalone                                    |
| 2    | `++i`      | 1                   | 2                  |                                                                                   |
| 3    | `i++ > 25` | 2                   | 3                  | **Post-increment**: `i` is compared FIRST (2 > 25 = false), THEN incremented to 3 |

Since `2 > 25` is **false**, the do-while loop exits after the first iteration.

### Output
```
3
```

> **Interview Trap:** The `do-while` loop always executes **at least once**. The `i++` in the condition uses the current value (2) for comparison, then increments to 3. The loop exits because the condition is false, but `i` is already 3.

---

## Q1.4: Returning Address of Local Variable (Dangling Pointer)

```c
#include <stdio.h>
int *fun() {
    int a = 10;
    return &a;  // WARNING: returning address of local variable!
}
int main() {
    int *ptr = fun();
    printf("%d", *ptr);
    return 0;
}
```

### Answer: **UNDEFINED BEHAVIOR**

```
Stack during fun():           Stack after fun() returns:
+------------------+          +------------------+
| a = 10           |  ←ptr    | ??? (freed)      |  ←ptr (DANGLING!)
+------------------+          +------------------+
| return address   |          | ...              |
+------------------+          +------------------+
```

**Why it's UB:**
- `a` is a **local variable** stored on the stack frame of `fun()`.
- When `fun()` returns, its stack frame is **deallocated**.
- `ptr` now points to deallocated memory → **dangling pointer**.
- Dereferencing `*ptr` accesses freed stack memory.

**Possible outcomes:**
- May print `10` (if stack hasn't been overwritten yet)
- May print garbage value 
- May cause segmentation fault 
- Behavior changes with optimization levels

> **Compiler Warning:** Most compilers will warn: `"warning: function returns address of local variable"`

> **How to Fix:**
> ```c
> // Method 1: Use static
> int *fun() { static int a = 10; return &a; }
> 
> // Method 2: Use dynamic allocation
> int *fun() { int *a = malloc(sizeof(int)); *a = 10; return a; }
> 
> // Method 3: Pass pointer from caller
> void fun(int *a) { *a = 10; }
> ```

---

## Q1.5: Double Pointer Dereference

```c
#include <stdio.h>
int main() {
    int a = 6;
    int *p = &a;
    int **q = &p;
    (**q)++;
    printf("%d", a);
    return 0;
}
```

### Step-by-Step with Pointer Chain Diagram

```
   q          p          a
+------+   +------+   +------+
| &p   |-->| &a   |-->|  6   |
+------+   +------+   +------+
  int**      int*        int

  *q  = p    (pointer to a)
  **q = *p = a (the integer value 6)
```

1. `**q` is equivalent to `*(*q)` = `*(p)` = `a` = **6**
2. `(**q)++` increments the value of `a`: `a` becomes **7**
3. `printf("%d", a)` prints **7**

### Output
```
7
```

> **Key Takeaway:** `**q` ultimately accesses the same memory location as `a`. The double pointer lets you modify the original variable through two levels of indirection.

---

## Q1.6: Pointer Assignment and Arithmetic

```c
#include <stdio.h>
int main() {
    int a = 1, b = 2;
    int *p = &a;
    int *q = &b;
    p = q;          // p now points to b (not a!)
    *q = *p + *q;   // *q = *p + *q = b + b = 2 + 2 = 4
    printf("%d %d", a, b);
    return 0;
}
```

### Step-by-Step with Diagrams

```
Initially:
  p → a(1)      q → b(2)

After p = q:
  p → b(2)      q → b(2)      a(1) (no pointer to a anymore)

After *q = *p + *q:
  Both p and q point to b.
  *p = 2, *q = 2
  *q = 2 + 2 = 4
  b is now 4, a is unchanged (still 1)
```

### Output
```
1 4
```

> **Interview Trap:** After `p = q`, **both** `p` and `q` point to `b`. So `*p + *q` = `b + b` = `2 + 2` = `4`. The variable `a` is never modified — it stays `1`.

---

## Q1.7: Negative Array Indexing

```c
#include <stdio.h>
int main() {
    int arr[] = {1, 2, 3, 4};
    int *p = arr + 1;
    printf("%d", p[-1]);
    return 0;
}
```

### Explanation

```
arr:  [ 1 ][ 2 ][ 3 ][ 4 ]
       [0]  [1]  [2]  [3]
             ^
             p = arr + 1 (points to arr[1])

p[-1] = *(p + (-1)) = *(arr + 1 - 1) = *(arr) = arr[0] = 1
```

`p[-1]` is perfectly valid C. Array subscript `p[n]` is equivalent to `*(p + n)`. When `n` is negative, the pointer moves backward.

### Output
```
1
```

> **Key Takeaway:** Negative indexing is legal in C as long as the resulting pointer stays within the bounds of the allocated array. `p[-1]` ≡ `*(p - 1)`.

---

## Q1.8: Static Variable in Function with Multiple Calls

```c
#include <stdio.h>
int fun() {
    static int x = 5;
    x++;
    return x;
}
int main() {
    printf("%d %d %d", fun(), fun(), fun());
    return 0;
}
```

### Key Concept: Unspecified Evaluation Order

The **static variable** `x` is initialized only once (to 5) and retains its value between calls.

Each call to `fun()` increments `x`:
- 1st call: `x` = 5 → 6, returns 6
- 2nd call: `x` = 6 → 7, returns 7
- 3rd call: `x` = 7 → 8, returns 8

**BUT:** The order of evaluation of function arguments is **unspecified** in C. The compiler can evaluate `fun(), fun(), fun()` in any order.

### Possible Outputs

| Evaluation Order | Output  |
|------------------|---------|
| Left to Right    | `6 7 8` |
| Right to Left    | `8 7 6` |

Most compilers (GCC, MSVC) evaluate right-to-left, giving: **`8 7 6`**

> **Interview Trap:** This is NOT undefined behavior — it's **unspecified behavior**. The calls will execute in some order, and `x` will be 8 after all three calls. But the mapping of return values to format specifiers depends on evaluation order.

---

## Q1.9: The `a+++a` Puzzle (Maximal Munch Rule)

```c
#include <stdio.h>
int main() {
    int a = 3;
    int b = a+++a;
    printf("%d %d", a, b);
    return 0;
}
```

### Tokenization: Maximal Munch Rule

The C lexer uses the **maximal munch** rule: it takes the longest possible valid token.

`a+++a` is tokenized as: `a` `++` `+` `a`

Which means: `b = (a++) + a`

### Execution (C11 behavior)

This expression has **potential for undefined behavior** in older standards because `a` is both modified (`a++`) and read (`+a`) between sequence points. However, many compilers handle it predictably:

**Most common output (GCC):**
- `a++` gives 3, then `a` becomes 4
- `b = 3 + 4 = 7` (using new value)

### Output (GCC typical)
```
4 7
```

> **Interview Trap:** Know the maximal munch rule. `a+++a` → `(a++) + a`, NOT `a + (++a)`.

---

## Q1.10: `sizeof('A')` in C

```c
#include <stdio.h>
int main() {
    printf("%d", sizeof('A'));
    return 0;
}
```

### Answer

In **C**, character constants like `'A'` are of type **`int`**, not `char`.

Therefore: `sizeof('A')` = `sizeof(int)` = **4** (on 32/64-bit systems)

### C vs C++ Difference

| Language | Type of `'A'` | `sizeof('A')` |
|----------|---------------|---------------|
| **C**    | `int`         | 4             |
| **C++**  | `char`        | 1             |

### Output
```
4
```

> **Interview Trap:** This is a classic C vs C++ difference question. In C, character constants are promoted to `int`. In C++, they remain `char`.

---

## Q1.11: Modifying a String Literal

```c
#include <stdio.h>
int main() {
    char *p = "Hello";
    p[0] = 'h';      // Attempting to modify string literal
    printf("%s", p);
    return 0;
}
```

### Answer: **UNDEFINED BEHAVIOR (likely Segmentation Fault)**

```
Memory Layout:
+------------------+
|   .rodata        |  ← Read-only data segment
|  "Hello\0"       |  ← String literal stored here
+------------------+
|   Stack          |
|  p → "Hello"     |  ← p points to .rodata
+------------------+
```

String literals in C are stored in **read-only memory** (`.rodata` segment). Attempting to modify them causes:
- **Segmentation fault** on most systems (Linux, macOS)
- **Undefined behavior** per the C standard

### How to Fix

```c
// Use a character array instead — this copies the string to stack
char p[] = "Hello";    // Array on stack, modifiable
p[0] = 'h';           // OK!
printf("%s", p);       // Output: "hello"
```

> **Key Difference:**
> | Declaration | Storage | Modifiable? |
> |-------------|---------|-------------|
> | `char *p = "Hello"` | `.rodata` (read-only) | ❌ No |
> | `char p[] = "Hello"` | Stack (local copy) | ✅ Yes |

---

## Q1.12: `sizeof(1.5)`

```c
#include <stdio.h>
int main() {
    printf("%d", sizeof(1.5));
    return 0;
}
```

### Answer

In C, floating-point literals like `1.5` default to type **`double`** (not `float`).

- `sizeof(double)` = **8** bytes on most platforms
- If you want float: use `1.5f`

### Output
```
8
```

> **Key Takeaway:**
> | Literal | Type | sizeof |
> |---------|------|--------|
> | `1.5`   | `double` | 8 |
> | `1.5f`  | `float`  | 4 |
> | `1.5L`  | `long double` | 8, 12, or 16 (platform-dependent) |

---

## Q1.13: Post-Increment in printf

```c
#include <stdio.h>
int main() {
    int a = 10;
    printf("%d", a++);
    return 0;
}
```

### Answer

`a++` is **post-increment**: the current value of `a` (10) is used in the expression, then `a` is incremented to 11.

So `printf` receives the value **10**.

### Output
```
10
```

> After this line, `a` is `11`, but that value is never used. If there were another `printf("%d", a);` it would print `11`.

---

## Q1.14: Assignment in `if` Condition (= vs ==)

```c
#include <stdio.h>
int main() {
    int x = 10;
    if (x = 5)        // Assignment, NOT comparison!
        printf("Yes");
    else
        printf("No");
    return 0;
}
```

### Step-by-Step

1. `x = 5` is an **assignment**, not a comparison.
2. The assignment expression `x = 5` assigns 5 to `x` and **evaluates to 5**.
3. In C, any non-zero value is **truthy**.
4. Since 5 ≠ 0, the `if` condition is **true**.

### Output
```
Yes
```

> **Interview Trap:** This is a classic bug. `=` is assignment, `==` is comparison. If the code had `x = 0`, the condition would be false and print "No".
> 
> **Best Practice:** Write constants on the left: `if (5 == x)` — this way, `if (5 = x)` would cause a compiler error, catching the mistake.

---

## Q1.15: Casting `char` Array to `int*`

```c
char arr[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
printf("%x", *((int*)arr));
```

### Answer: Depends on Endianness

We cast the address of `arr` to `int*` and dereference it, reading 4 bytes as a single integer.

```
Memory (byte addresses):
Address:  [0]    [1]    [2]    [3]    [4]
Value:    0xAA   0xBB   0xCC   0xDD   0xEE
```

**Little-Endian (x86, ARM):**
- Byte at lowest address = least significant byte
- Integer = `0xEEDDCCBBAA`

**Big-Endian (PowerPC, SPARC):**
- Byte at lowest address = most significant byte
- Integer = `0xAABBCCDDEE`

### Output
| System        | Output       |
|---------------|--------------|
| Little-Endian | `eeddccbbaa` |
| Big-Endian    | `aabbccddee` |

---

## Q1.16: How `char arr[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE}` is Stored

### Key Insight: Byte Arrays are NOT Affected by Endianness

Endianness only affects how **multi-byte values** (int, short, long) are stored. A char array is a sequence of individual bytes, and they are **always stored in the same order** regardless of endianness.

```
Both Little-Endian and Big-Endian:
Address:  0x100   0x101   0x102   0x103   0x104
Value:    0xAA    0xBB    0xCC    0xDD    0xEE
```

> **Interview Trap:** Many candidates confuse this. Endianness only matters when you interpret multiple bytes as a single larger type (like casting `char[]` to `int*` as in Q1.15). The raw byte order in memory is identical.

---

## Q1.17: Array Pointer Operations (`a++` is Illegal)

```c
int a[7] = {6, 5, 1, 2, 3, 4};
// What is the value of a++?
int *b = a;
b++;
b++;
// What is the value of *b?
```

### Answer

**`a++` is ILLEGAL and won't compile.**

`a` is an **array name**, which decays to a constant pointer to the first element. You cannot increment an array name because it's not a modifiable lvalue.

```c
a++;   // ERROR: lvalue required as increment operand
```

For the pointer part:
```
a[]:  [ 6 ][ 5 ][ 1 ][ 2 ][ 3 ][ 4 ][ ? ]
       [0]  [1]  [2]  [3]  [4]  [5]  [6]
       ^
       b = a (initially)

After b++:  b points to a[1] (value 5)
After b++:  b points to a[2] (value 1)

*b = 1
```

### Answers
- `a++` → **Compilation Error**
- `*b` after two increments → **1** (value at `a[2]`)

> **Key Takeaway:** Array names are NOT pointers — they decay to pointers in most contexts, but they are not modifiable lvalues. You cannot do `a++`, `a--`, or `a = something`.

---

## Q1.18: `*p++` vs `++*p`

Given:
```c
int arr[] = {2, 3, 4, 5};
int *p = arr;
```

### `*p++` (Use, then advance pointer)
```c
int val = *p++;
// Equivalent to: val = *(p++);
// Step 1: val = *p = 2 (current value at p)
// Step 2: p++ (p advances to arr[1])
// Result: val = 2, p now points to arr[1]
```

### `++*p` (Increment value at pointer)
```c
int val = ++*p;
// Equivalent to: val = ++(*p);
// Step 1: *p = arr[0] = 2
// Step 2: ++(*p) increments arr[0] from 2 to 3
// Result: val = 3, arr[0] is now 3, p unchanged
```

### Operator Precedence Summary

| Expression | Parsed As | Action                                            |
|------------|-----------|---------------------------------------------------|
| `*p++`     | `*(p++)`  | Return `*p`, then increment **pointer**           |
| `*++p`     | `*(++p)`  | Increment **pointer** first, then dereference     |
| `++*p`     | `++(*p)`  | Increment **value** at pointer                    |
| `(*p)++`   | —         | Return **value** at pointer, then increment value |

---

## Q1.19: What do `printf` and `scanf` Return?

### `printf` Return Value
Returns the **number of characters successfully printed** (excluding the null terminator).

```c
int n = printf("Hello");    // Prints "Hello", n = 5
int m = printf("%d", 123);  // Prints "123", m = 3
int k = printf("");          // Prints nothing, k = 0
```

Returns a **negative value** on error.

### `scanf` Return Value
Returns the **number of input items successfully matched and assigned**.

```c
int a, b;
int n = scanf("%d %d", &a, &b);
// If user enters "10 20": n = 2
// If user enters "10 abc": n = 1 (b not assigned)
// If user enters "abc": n = 0
// If EOF: n = EOF (-1)
```

### Practical Use

```c
// Common pattern to check scanf success:
if (scanf("%d", &x) != 1) {
    printf("Invalid input!\n");
}

// Using printf return for logging:
int chars = printf("Result: %d\n", result);
fprintf(logfile, "Wrote %d chars\n", chars);
```

> **Interview Trap:** `scanf` returns `EOF` (typically -1) on end-of-file, NOT 0. And `printf` returns the count of characters, NOT the number of format specifiers.

---
---

# 2. C Language Concepts

---

## Q2.1: What is `volatile`? What is the use of `volatile`? Any practical applications?

### Answer
The `volatile` keyword in C is a qualifier that tells the compiler that a variable's value can be changed by something outside the control of the program's code itself at any time. Consequently, the compiler **must not optimize** any reads or writes to this variable. It forces the compiler to read the value directly from the memory address every time it is referenced, rather than caching it in a CPU register.

### Compiler Optimization without volatile
If a variable is not marked `volatile`, the compiler performs optimization. For example:
```c
int flag = 0;
while (flag == 0) {
    // Do nothing
}
```
The compiler might optimize this by loading `flag` into a CPU register once and looping indefinitely because it sees no code inside the loop modifying `flag`:
```assembly
; Optimized Assembly Representation
LDR R0, [flag_address]
LOOP:
    CMP R0, #0
    BEQ LOOP      ; If R0 was 0, it loops forever without re-reading from memory!
```

### With volatile
```c
volatile int flag = 0;
while (flag == 0) {
    // Do nothing
}
```
The compiler generates code to reload `flag` from memory on every iteration:
```assembly
; Volatile Assembly Representation
LOOP:
    LDR R0, [flag_address] ; Re-reads from memory every time!
    CMP R0, #0
    BEQ LOOP
```

### Practical Applications in Embedded Systems
1. **Memory-Mapped I/O (MMIO) Registers:** Peripheral registers (e.g., UART status register) change state based on hardware events (e.g., byte received).
2. **Interrupt Service Routines (ISRs):** Flags shared between an ISR and the main loop.
3. **Multi-threaded applications:** Variables shared between threads (though mutexes/semaphores are also needed for synchronization, `volatile` ensures memory visibility).

### Working Code Example
```c
#include <stdio.h>
#include <stdbool.h>

// Shared flag updated by an interrupt or signal handler
volatile bool data_ready = false;

void pseudo_isr_handler() {
    // Simulated hardware interrupt setting flag
    data_ready = true;
}

int main() {
    printf("Waiting for data...\n");
    while (!data_ready) {
        // Without volatile, the compiler could optimize this loop into an infinite loop 
        // by assuming data_ready never changes because it doesn't see it change in this thread.
    }
    printf("Data received!\n");
    return 0;
}
```

> **Interview Trap:** `volatile` is NOT a synchronization mechanism. It does NOT make operations atomic. It only guarantees memory visibility. For thread safety, mutexes or atomic operations are still required.

---

## Q2.2: Use of the `static` keyword?
The `static` keyword has three distinct meanings in C depending on where it is used.

### 1. Static Local Variables (Inside a Function)
A static local variable is initialized only once and retains its value between function calls. Its lifetime is the duration of the program, but its scope is limited to the function.

```c
void counter() {
    static int count = 0; // Initialized once, stored in .data segment
    count++;
    printf("%d ", count);
}
```

```
Memory Allocation:
+-------------------+
|  .data segment    |  <-- static int count is stored here (retains value)
+-------------------+
|  Stack Frame      |  <-- Regular local variables (destroyed after call)
+-------------------+
```

### 2. Static Global Variables (File Scope)
A static global variable is visible only within the file (translation unit) it is declared. It cannot be accessed from other files using the `extern` keyword. This provides encapsulation.

### 3. Static Functions
A static function is visible only within its translation unit. It prevents namespace pollution and name clashes in large projects.

### Questions:
#### Q2.2a: What happens during compilation if I use a static function?
During compilation, the compiler generates the code for the static function normally but limits its symbol visibility to local scope inside the object file (`.o`). The linker will not resolve any references to this function symbol from other object files, preventing linking conflicts.

#### Q2.2b: Static variable defined in a header file, included in 2 different files?
If you define a static variable in a header file (e.g., `static int x = 10;`) and include it in `file1.c` and `file2.c`, **each C file gets its own independent copy of the variable**. Modifying `x` in `file1.c` will not affect `x` in `file2.c`. This is usually a bug if you intended to share the variable.
- **Trap:** It increases memory usage since multiple copies exist. To share, declare it as `extern int x;` in the header and define it as `int x = 10;` in one C file.

#### Q2.2c: How to indirectly modify a static variable from another file?
Since a static global variable cannot be accessed directly via `extern`, you must provide public getter and setter functions in the file where the static variable is defined:

```c
// file1.c
static int private_var = 100;

void set_private_var(int val) {
    private_var = val;
}

int get_private_var(void) {
    return private_var;
}
```

---

## Q2.3: Use of `extern` keyword? (Internal vs External Linkage)

### Answer
The `extern` keyword extends the visibility of a variable or function to other translation units (files). It tells the compiler: *"This symbol is declared here, but its definition and memory allocation exist in another file. The linker will resolve this reference."*

### Declaration vs Definition
- **Declaration:** Informs the compiler about the type and name of a variable/function. Allocates no memory.
- **Definition:** Allocates memory for the variable/function and optionally initializes it.

```c
extern int global_var; // Declaration (does not allocate memory)
int global_var = 42;   // Definition (allocates memory)
```

### Internal vs External Linkage
- **External Linkage:** Symbols (variables/functions) can be accessed across files. By default, global variables and normal functions have external linkage.
- **Internal Linkage:** Symbols are restricted to the file they are defined in. Achieved using the `static` keyword.

```
Linkage Visual:
               [ file1.c ]                      [ file2.c ]
      ---------------------------      ---------------------------
      int shared_val = 5; (Ext)   <=== extern int shared_val;
      static int local_val = 10;       (Can access shared_val)
      (Int, hidden from file2)         (Cannot access local_val)
```

---

## Q2.4: Storage Classes in C

C provides 4 storage classes that determine the scope, lifetime, and storage location of variables.

| Storage Class | Storage          | Default Value | Scope         | Lifetime               |
|---------------|------------------|---------------|---------------|------------------------|
| `auto`        | Stack            | Garbage       | Local (Block) | Temporary (Block exit) |
| `register`    | CPU Register     | Garbage       | Local (Block) | Temporary (Block exit) |
| `static`      | RAM (.data/.bss) | Zero (0)      | Local / File  | Program duration       |
| `extern`      | RAM (.data/.bss) | Zero (0)      | Global        | Program duration       |

### Details
1. **`auto`**: The default storage class for local variables.
2. **`register`**: Suggests that the compiler store the variable in a CPU register for faster access.
   - **Trap:** You cannot take the address of a register variable (`&reg_var`) because registers do not have memory addresses.
3. **`static`**: Preserves value, zero-initialized by default.
4. **`extern`**: Shares global variables across files.

---

## Q2.5: Steps of Compiling a C Program

The compilation process converts human-readable C code into machine-executable binary. It consists of 4 main phases:

```
[ Source Code (.c) ]
       │
       ▼  (1) Preprocessor (gcc -E) -> Expands macros, includes headers, removes comments
[ Preprocessed Code (.i) ]
       │
       ▼  (2) Compiler (gcc -S) -> Translates C to assembly instructions
[ Assembly Code (.s) ]
       │
       ▼  (3) Assembler (gcc -c) -> Translates assembly to machine code (Object file)
[ Object Code (.o) ]
       │
       ▼  (4) Linker (gcc) -> Merges object files and library binaries
[ Executable Binary (.out / .exe) ]
```

### Detailed Steps:
1. **Preprocessing:** Handles directives starting with `#`. Outputs a `.i` file.
   - Expands macros (`#define`).
   - Inserts header files (`#include`).
   - Conditional compilation (`#if`, `#ifdef`).
   - Removes comments.
2. **Compilation:** Translates preprocessed code into assembly language for the target architecture. Outputs a `.s` file.
3. **Assembly:** Converts assembly instructions into machine code (relocatable object code). Outputs a `.o` (or `.obj`) file.
4. **Linking:** Resolves external references, combines multiple object files, links standard library functions (like `printf`), and arranges section layouts. Outputs the final executable.

---

## Q2.6: Weak Functions (`__attribute__((weak))`)

### Answer
A weak function is a function declared with the `weak` attribute. It tells the linker that if another function with the same name is defined elsewhere in the program (a "strong" definition), that strong definition should override the weak one. If no strong definition is found, the linker uses the weak one.

### Practical Application in Embedded Systems
Used heavily in Hardware Abstraction Layers (HAL) and startup codes (e.g., STM32 HAL). The vector table contains weak definitions for all interrupt handlers pointing to a default handler (usually an infinite loop). If the developer defines a custom handler for an interrupt, it overrides the weak one automatically without throwing a "multiple definition" linker error.

### Code Example
```c
#include <stdio.h>

// Weak implementation (default behavior)
void __attribute__((weak)) USART1_IRQHandler(void) {
    // Default: Clear interrupt flag or ignore
    printf("Default UART Handler (Weak)\n");
}

int main() {
    USART1_IRQHandler(); // Calls strong definition if it exists, else weak.
    return 0;
}
```

If the developer writes this in another file:
```c
// Strong implementation (overrides default)
void USART1_IRQHandler(void) {
    printf("Custom developer UART Handler (Strong)\n");
}
```
The program will output `Custom developer UART Handler (Strong)`.

---

## Q2.7: Macro Function vs Inline Function

### Comparison Table

| Feature               | Macro Function (`#define`)                | Inline Function (`inline`)             |
|-----------------------|-------------------------------------------|----------------------------------------|
| **Compilation Phase** | Preprocessor (text substitution)          | Compiler (inserts code if appropriate) |
| **Type Checking**     | None (leads to dangerous bugs)            | Strict type checking performed         |
| **Debugging**         | Hard (doesn't appear in stack traces)     | Easy (supports breakpoints)            |
| **Side Effects**      | Susceptible to expression evaluation bugs | None                                   |
| **Return Value**      | Cannot return a value cleanly             | Can return values with proper types    |

### Visual Representation of Expansion
```
Macro: SQUARE(x++) -> (x++) * (x++) -> Increments x twice! (Double evaluation trap)
Inline: inline int square(int x) { return x * x; } -> Evaluates x once, then squares.
```

### Side Effect Trap Code
```c
#define SQUARE(x) ((x) * (x))
inline int sq_func(int x) { return x * x; }

int main() {
    int a = 5;
    int b = SQUARE(a++); // Expands to: ((a++) * (a++)) -> UB or double increment (a becomes 7!)
    
    int c = 5;
    int d = sq_func(c++); // c is passed as 5, incremented once to 6. d = 25. Safe!
    return 0;
}
```

---

## Q2.8: Inline Functions — Why, When, and Compiler Behavior

### What is an Inline Function?
An `inline` function is a request to the compiler to insert the function's body directly at the call site, eliminating the overhead of a function call (pushing arguments to stack, jumping to function address, returning).

```c
inline int square(int x) {
    return x * x;
}

int main() {
    int val = square(5); // Compiler may replace this with: int val = 5 * 5;
    return 0;
}
```

### Why Use Inline?
1. **Eliminates function call overhead:** No stack frame push/pop, no jump/return instructions.
2. **Enables further optimizations:** Once inlined, the compiler can optimize the combined code (constant folding, dead code elimination).
3. **Ideal for small, frequently called functions:** Getters, setters, simple math operations.

### When Does the Compiler IGNORE the `inline` Keyword?
The `inline` keyword is a **suggestion**, not a command. The compiler may refuse to inline a function when:

| Condition                                              | Reason                                                             |
|--------------------------------------------------------|--------------------------------------------------------------------|
| Function body is too large                             | Code bloat (binary size explosion) outweighs call overhead savings |
| Function contains loops (`for`, `while`, `do-while`)   | Loop bodies are already expensive; inlining adds no benefit        |
| Function is recursive                                  | Cannot expand recursion at compile time                            |
| Function uses `static` local variables                 | Static variables require a fixed memory address                    |
| Function address is taken (`&func`)                    | Must exist as a callable entity in memory                          |
| Function contains `switch`/`goto`                      | Complex control flow is hard to inline                             |
| Compiler optimization level is `-O0` (no optimization) | Inlining is an optimization; disabled at `-O0`                     |

### Inline vs Macro vs Regular Function Summary

| Feature             | `#define` Macro                  | `inline` Function         | Regular Function         |
|---------------------|----------------------------------|---------------------------|--------------------------|
| **Expansion Phase** | Preprocessor (text substitution) | Compiler (code insertion) | Linker (call resolution) |
| **Type Checking**   | ❌ None                           | ✅ Full type checking      | ✅ Full type checking     |
| **Debugging**       | ❌ Hard (no symbol)               | ✅ Debuggable              | ✅ Debuggable             |
| **Side Effects**    | ⚠️ Double evaluation trap         | ✅ Safe                    | ✅ Safe                   |
| **Call Overhead**   | ❌ None (text replaced)           | ❌ None (if inlined)       | ✅ Yes (stack frame)      |
| **Code Size**       | Grows (each use = copy)          | Grows (if inlined)        | Compact (single copy)    |

> **Interview Trap:** GCC uses `__attribute__((always_inline))` to force inlining. The standard `inline` keyword alone never guarantees inlining.

---

## Q2.9: Structure, Union, and Enum differences

### 1. Structure (`struct`)
A structure allocates separate memory locations for all of its members. All members can be accessed simultaneously.

```c
struct MyStruct {
    char a;    // 1 byte
    int b;     // 4 bytes
}; // Size = 8 bytes (due to padding/alignment)
```

### 2. Union (`union`)
A union allocates a single memory location shared by all its members. The size of the union is the size of its largest member. Only one member can be used at a time.

```c
union MyUnion {
    char a;    // 1 byte
    int b;     // 4 bytes
}; // Size = 4 bytes (shared memory)
```

```
Memory Layout Comparison:
Structure:
+-----+-----+-----+-----+-----+-----+-----+-----+
|  a  | padding   |          b                  |
+-----+-----+-----+-----+-----+-----+-----+-----+
0     1           4                             8

Union:
+-----+-----+-----+-----+
|  a  |                 |  <-- Both 'a' and 'b' share this start address
+-----+-----+-----+-----+
|           b           |
+-----------------------+
0                       4
```

### 3. Enum (`enum`)
An enumeration is a user-defined type consisting of integer constants. It increases code readability and safety.

```c
enum State { IDLE, RUNNING, ERROR }; // IDLE=0, RUNNING=1, ERROR=2
```

---

## Q2.10: Storage of static members inside structures

### Answer
In standard C, **structures cannot contain static members**. 
In C++, structures can have static members. A static member in a C++ struct is not part of the individual struct objects. It is shared across all instances of the struct and is stored in the `.data` or `.bss` segment of the global memory, not on the stack or heap inside the struct instance.

```cpp
struct Test {
    int x;             // Stored inside each instance
    static float y;    // Stored globally in .data/.bss
};
```

---

## Q2.11: The `const` Keyword — Usage, Pointer Variants, Function Parameters

### Basic Usage
`const` makes a variable read-only after initialization. Any attempt to modify it results in a compilation error.

```c
const int MAX_SIZE = 100;
MAX_SIZE = 200; // ERROR: assignment of read-only variable
```

### The 4 Pointer-`const` Combinations

| Declaration          | Read As                            | Pointer Modifiable? | Data Modifiable? |
|----------------------|------------------------------------|---------------------|------------------|
| `int *p`             | Pointer to int                     | ✅ Yes               | ✅ Yes            |
| `const int *p`       | Pointer to **const** int           | ✅ Yes               | ❌ No             |
| `int *const p`       | **const** pointer to int           | ❌ No                | ✅ Yes            |
| `const int *const p` | **const** pointer to **const** int | ❌ No                | ❌ No             |

### Code Examples
```c
int a = 10, b = 20;

// 1. Pointer to const int (data is read-only)
const int *p1 = &a;
// *p1 = 30;    // ERROR: cannot modify data through p1
p1 = &b;        // OK: can change where p1 points

// 2. Const pointer to int (pointer is fixed)
int *const p2 = &a;
*p2 = 30;        // OK: can modify data through p2
// p2 = &b;     // ERROR: cannot change where p2 points

// 3. Const pointer to const int (both fixed)
const int *const p3 = &a;
// *p3 = 30;    // ERROR
// p3 = &b;     // ERROR
```

### `const` in Function Parameters
Using `const` in function parameters communicates intent and prevents accidental modification:

```c
// Promise: this function will NOT modify the string pointed to by str
size_t my_strlen(const char *str) {
    const char *s = str;
    while (*s) s++;
    return s - str;
}
```

> **Key Takeaway:** `const` is a contract. In embedded systems, it is used to place data in Flash/ROM (read-only memory). For example, `const uint32_t lookup_table[] = {0, 1, 4, 9};` is stored in `.rodata` (Flash), saving precious SRAM.

---

## Q2.12: Void Pointer (`void*`)

### Answer
A void pointer is a generic pointer that has no associated data type. It can point to any data type (int, float, char, struct, etc.).

### Rules
1. You **cannot dereference** a void pointer directly without typecasting it first, because the compiler doesn't know the size of the underlying type.
2. You **cannot perform pointer arithmetic** on a void pointer in standard C without casting it (though some compilers like GCC treat `sizeof(void)` as 1 as an extension).

### Code Example
```c
#include <stdio.h>

int main() {
    int a = 10;
    float b = 5.5;
    void *ptr = &a; // Points to int
    
    // Dereferencing require explicit cast
    printf("Integer: %d\n", *(int*)ptr);
    
    ptr = &b; // Points to float
    printf("Float: %.1f\n", *(float*)ptr);
    
    return 0;
}
```

---

## Q2.13: Function Pointer with Code Example

### Answer
A function pointer is a pointer that stores the address of a function in memory (code segment) instead of a data address. It is used to implement callbacks, dynamic dispatch, and state machines.

### Syntax
```c
return_type (*pointer_name)(parameter_types);
```

### Code Example
```c
#include <stdio.h>

void greet(const char *name) {
    printf("Hello, %s!\n", name);
}

int main() {
    // Declare function pointer
    void (*func_ptr)(const char*) = greet;
    
    // Invoke function through pointer
    func_ptr("Embedded System Engineer");
    
    return 0;
}
```

---

## Q2.14: Callback Function

### Answer
A callback function is a function that is passed as an argument to another function, to be called when a certain event occurs or a task completes.

### Why use Callback instead of Normal Function?
Callbacks enable **loose coupling** and **modularity**. For example, a generic timer driver doesn't need to know what specific action (blink LED, send packet) needs to run on timer expiry. It simply accepts a function pointer (callback) from the application layer and executes it when the timer fires.

```
+------------------+                   +--------------------+
| Application Layer |               | Hardware Driver |
|-------------------|---------------|-----------------|
| void blink()      | ──Register──► |                 |
|                   |               | On Interrupt:   |
|                   | ◄──Execute──  | callback()      |
+------------------+                   +--------------------+
```

### Code Example
```c
#include <stdio.h>

// Driver function that takes a callback pointer
void perform_operation(int a, int b, int (*callback)(int, int)) {
    printf("Performing operation...\n");
    int result = callback(a, b);
    printf("Result is: %d\n", result);
}

int add(int x, int y) { return x + y; }
int multiply(int x, int y) { return x * y; }

int main() {
    perform_operation(5, 4, add);      // Outputs 9
    perform_operation(5, 4, multiply); // Outputs 20
    return 0;
}
```

---

## Q2.15: Function Declaration vs Definition (e.g. `printf`, `scanf`)

### Answer
- **Declaration (Prototype):** Specifies the function name, return type, and parameters. It allows the compiler to perform type checking. No code is generated.
- **Definition:** Contains the actual implementation (code body) of the function.

For library functions like `printf` and `scanf`:
- **Declaration** is in header file `<stdio.h>`:
  ```c
  int printf(const char *format, ...);
  int scanf(const char *format, ...);
  ```
- **Definition** is inside the pre-compiled C standard library binary (`libc.a` or `libc.so`), which the linker merges with your code during the linking phase.

---

## Q2.16: Signed vs Unsigned Integers

### Answer
- **Signed integers:** Can represent both positive and negative values. Uses Two's Complement representation for negative numbers, reserving the MSB (Most Significant Bit) as the sign bit (0 = positive, 1 = negative).
- **Unsigned integers:** Can represent only non-negative values. The MSB is used as part of the value.

### Trap: Comparison between Signed and Unsigned
When comparing signed and unsigned integers, C performs **implicit promotion** to unsigned. This causes unexpected behavior:

```c
#include <stdio.h>

int main() {
    int a = -10;
    unsigned int b = 5;
    if (a > b) {
        // This prints because -10 is converted to a very large unsigned int (e.g., 4294967286)
        printf("-10 is greater than 5!\n");
    } else {
        printf("-10 is less than 5\n");
    }
    return 0;
}
```

---

## Q2.17: Typecasting vs Type Conversion

### Answer
- **Type Conversion (Implicit / Coercion):** Performed automatically by the compiler to convert a value of one data type to another to prevent data loss or match operations (e.g., `float f = 5;` -> integer 5 promoted to float 5.0).
- **Typecasting (Explicit):** Performed manually by the programmer using the cast operator (e.g., `(int)3.14`).

### Code Example
```c
int a = 10;
int b = 3;
double val1 = a / b;        // Implicit: division is int/int = 3, then converted to 3.0
double val2 = (double)a / b; // Explicit Typecast: float division = 3.333
```

---

## Q2.18: `typedef`

### Answer
`typedef` is used to create an alias (alternative name) for an existing data type. It makes code more readable and portable.

```c
typedef unsigned int uint32_t;
typedef struct {
    int x;
    int y;
} Point;
```

---

## Q2.19: Static vs Shared Libraries

### Answer
- **Static Library (`.a` / `.lib`):** The code of the library is physically copied and linked into the application executable during build time.
  - **Pros:** Fast execution, independent executable.
  - **Cons:** Larger executable size, requires rebuilding application if library updates.
- **Shared / Dynamic Library (`.so` / `.dll`):** The library code is loaded into memory only when the application starts or runs. The executable contains only references.
  - **Pros:** Smaller executable size, multiple apps can share a single copy in memory.
  - **Cons:** Startup overhead, runtime dependency ("DLL hell").

---

## Q2.20: Implement custom `sizeof` using Macro

### Answer
```c
#define sizeof_custom(type) ((size_t)((type*)0 + 1))
// Or for variables:
#define var_sizeof(var) ((char *)(&var + 1) - (char *)&var)
```

### Explanation of `my_sizeof`
1. `(type*)0` casts the address `0` to a pointer of the target type.
2. `((type*)0 + 1)` increments the pointer by `sizeof(type)` bytes, effectively pointing to the memory location immediately after the first element.
3. Since the array started at `0`, the address of the element at index 1 is exactly equal to `sizeof(type)`.

---

## Q2.21: Pointers to Strings & Storage Location

Given the following declarations:
```c
const char *p1 = "hello";
char *p2 = "hello";
char s[] = "hello";
```

### Memory Storage and Differences

```
Memory Layout:
+-----------------------------------+
|  .rodata (Read-Only Data Segment) | <-- "hello" (string literals)
|   Address: 0x1000                 |     p1 and p2 point here
+-----------------------------------+
|  Stack Segment                    | <-- s[] = {'h','e','l','l','o','\0'}
|   Address: 0x8000                 |     (Local copy of the string)
+-----------------------------------+
```

1. **`const char *p1 = "hello"`:** `p1` is a pointer stored on the stack pointing to the read-only data segment (`.rodata`). Trying to modify it (`p1[0] = 'H'`) is prevented at compile time because of the `const` keyword.
2. **`char *p2 = "hello"`:** `p2` is a modifiable pointer, but it also points to `.rodata`. Trying to modify the string (`p2[0] = 'H'`) compiles but causes a **Segmentation Fault** at runtime because you are writing to read-only memory.
3. **`char s[] = "hello"`:** `s` is a local array stored on the stack. The string literal is copied from flash/`.rodata` to stack during initialization. You can modify it safely (`s[0] = 'h'`).

---

## Q2.22: IEEE 754 Floating Point Representation

### Format (32-bit Single Precision `float`)

A 32-bit float is stored as:

```
  Bit 31       Bits 30-23          Bits 22-0
 +------+------------------+---------------------------+
 | Sign |    Exponent (8)  |     Mantissa/Fraction (23) |
 +------+------------------+---------------------------+
    1 bit      8 bits                 23 bits
```

- **Sign (1 bit):** `0` = positive, `1` = negative.
- **Exponent (8 bits):** Stored with a **bias of 127**. Actual exponent = stored value − 127.
- **Mantissa (23 bits):** Stores the fractional part of the normalized number. An implicit leading `1.` is assumed (normalized form: `1.mantissa × 2^exponent`).

### Example: Represent `6.75` in IEEE 754
Fracton entity is resolved by multiplying by 2 until fractin become zero
1. **Convert to binary:** `6.75` = `110.11` in binary.
2. **Normalize:** `1.1011 × 2^2` (moved decimal point 2 places left).
3. **Sign bit:** `0` (positive).
4. **Exponent:** `2 + 127 (bias)` = `129` = `10000001` in binary.
5. **Mantissa:** `10110000000000000000000` (23 bits, drop the leading `1.`).

```
  0  10000001  10110000000000000000000
  S  Exponent        Mantissa
```

In hex: `0x40D80000`

### Special Values

| Value       | Sign | Exponent | Mantissa                |
|-------------|------|----------|-------------------------|
| `+0.0`      | 0    | 00000000 | 00000000000000000000000 |
| `-0.0`      | 1    | 00000000 | 00000000000000000000000 |
| `+Infinity` | 0    | 11111111 | 00000000000000000000000 |
| `-Infinity` | 1    | 11111111 | 00000000000000000000000 |
| `NaN`       | X    | 11111111 | non-zero                |

### 64-bit Double Precision
- Sign: 1 bit, Exponent: 11 bits (bias = 1023), Mantissa: 52 bits.

> **Interview Trap:** `sizeof('A')` is `4` in C (char literal is promoted to `int`), but `sizeof(1.5)` is `8` (floating point literals are `double` by default, not `float`). Use `1.5f` for a 4-byte float literal.

---

## Q2.23: How `char*` and `int*` Access Memory Differently

Both are pointers containing memory addresses, but their **type** determines how many bytes the CPU reads/writes when the pointer is dereferenced.

```c
char *cp;  // Dereferences 1 byte at the pointed address
int *ip;   // Dereferences 4 bytes (on 32-bit) at the pointed address
```

```
Memory:  0x100  0x101  0x102  0x103  0x104
         [0xAA] [0xBB] [0xCC] [0xDD] [0xEE]

char *cp = (char*)0x100;
*cp → reads 1 byte → 0xAA

int *ip = (int*)0x100;
*ip → reads 4 bytes → 0xDDCCBBAA (little-endian)
```

Pointer arithmetic is also affected:
- `cp + 1` advances by `sizeof(char)` = **1 byte** → address `0x101`.
- `ip + 1` advances by `sizeof(int)` = **4 bytes** → address `0x104`.

---

## Q2.24: `allocateMemory(&myPtr)` — Pointer-to-Pointer for Dynamic Allocation

### The Problem
A common interview challenge: Implement `allocateMemory()` that allocates heap memory inside the function and sets it to value 10, such that the caller's pointer is updated.

### Why pointer-to-pointer is needed
If you pass a plain pointer (`int *ptr`) to a function, the function receives a **copy** of the pointer. Modifying the copy inside the function does not affect the original pointer in the caller. To modify the caller's pointer itself, you must pass a **pointer to the pointer** (`int **ptr`).

```
Passing int *ptr (WRONG):              Passing int **ptr (CORRECT):
┌─────────┐    ┌──────────┐           ┌─────────┐    ┌──────────┐
│ main()  │    │ func()   │           │ main()  │    │ func()   │
│ ptr ────┼──► │ ptr_copy │           │ ptr ────┼──► │ *pptr    │
│ (NULL)  │    │ = malloc │           │ (NULL)  │    │ = &ptr   │
│         │    │ (lost!)  │           │ (Updated│◄── │ *pptr =  │
└─────────┘    └──────────┘           │ to heap)│    │ malloc   │
                                      └─────────┘    └──────────┘
```

### Code Implementation
```c
#include <stdio.h>
#include <stdlib.h>

void allocateMemory(int **pptr) {
    // Allocate memory on the heap
    *pptr = (int *)malloc(sizeof(int));
    if (*pptr == NULL) {
        printf("Allocation failed!\n");
        return;
    }
    // Set value to 10
    **pptr = 10;
}

int main() {
    int *myPtr = NULL;
    
    allocateMemory(&myPtr);
    
    // myPtr now points to allocated memory with value 10
    printf("Value: %d\n", *myPtr); // Output: 10
    
    free(myPtr);
    myPtr = NULL; // Prevent dangling pointer
    return 0;
}
```

---

## Q2.25: Implement `atoi` (ASCII to Integer Conversion)

```c
#include <stdio.h>

int my_atoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;
    
    // Skip leading whitespace
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }
    
    // Handle sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    
    // Process digits
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return sign * result;
}

int main() {
    printf("%d\n", my_atoi("  -12345abc")); // Output: -12345
    printf("%d\n", my_atoi("42"));           // Output: 42
    printf("%d\n", my_atoi("   +100"));      // Output: 100
    return 0;
}
```

> **Interview Trap:** Always handle: leading whitespace, optional sign character, non-digit trailing characters, and integer overflow conditions.

---

## Q2.26: Implement Decimal to Binary Conversion

```c
#include <stdio.h>

void decimal_to_binary(int n) {
    if (n == 0) {
        printf("0");
        return;
    }
    
    // Store bits in array (max 32 bits for int)
    int bits[32];
    int i = 0;
    int num = (n < 0) ? -n : n; // Handle negative using absolute value
    
    while (num > 0) {
        bits[i++] = num % 2;
        num /= 2;
    }
    
    if (n < 0) printf("-");
    
    // Print in reverse order (MSB first)
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", bits[j]);
    }
    printf("\n");
}

int main() {
    decimal_to_binary(42);  // Output: 101010
    decimal_to_binary(255); // Output: 11111111
    return 0;
}
```
--

# 3. Memory Layout, Management & Pointers

---

## Q3.1: C Program Memory Layout Details

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

## Q3.2: Static Memory vs Dynamic Memory Difference

| Feature             | Static Memory Allocation                       | Dynamic Memory Allocation                  |
|---------------------|------------------------------------------------|--------------------------------------------|
| **Allocation Time** | Compile / Load time                            | Runtime (execution time)                   |
| **Storage Area**    | `.data`, `.bss` (or Stack for local variables) | Heap segment                               |
| **Size**            | Fixed size defined at compile-time             | Variable size adjusted at runtime          |
| **Lifetime**        | Duration of block (stack) or program (global)  | Controlled by programmer (`malloc`/`free`) |
| **Access Speed**    | Very fast (direct memory access)               | Slower (requires pointer dereferencing)    |

---

## Q3.3: Stack vs Heap Memory

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

## Q3.4: Stack Overflow vs Segmentation Fault

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

## Q3.5: Dangling Pointer, NULL Pointer, and Void Pointer

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

## Q3.6: Memory Leaks

### Answer
A memory leak occurs when a program allocates memory on the heap but fails to release it back to the operating system after it is no longer needed. Over time, memory leaks consume RAM, eventually causing the system to run out of memory (OOM crash).

### Does heap memory get freed automatically?
- **Within execution:** No. Heap memory remains allocated until the programmer explicitly calls `free()`.
- **On Program Exit:** Yes, the OS automatically reclaims the entire heap space allocated to the process when it terminates. (However, in long-running servers or bare-metal embedded systems that run indefinitely, memory leaks are catastrophic).

### How to avoid:
- Match every `malloc`/`calloc` with a corresponding `free`.
- Use static analysis tools (like Coverity) or dynamic analysis tools (like Valgrind).

---

## Q3.7: `malloc()` vs `calloc()`

| Feature            | `malloc()`                                           | `calloc()`                                |
|--------------------|------------------------------------------------------|-------------------------------------------|
| **Parameters**     | Takes size in bytes: `malloc(size)`                  | Takes count and size: `calloc(num, size)` |
| **Initialization** | Does NOT initialize memory (contains garbage values) | Initializes memory to zero                |
| **Performance**    | Faster (no zeroing-out overhead)                     | Slower (requires writing zeros to memory) |

### Trap: Non-contiguous allocation in calloc?
If `calloc` cannot allocate the requested size as a single contiguous block of virtual memory, it **fails and returns NULL**. It never allocates fragmented memory fragments and links them.

---

## Q3.8: How `malloc()` Allocates Memory Internally

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

## Q3.9: How to Free Memory without using `free()`

### Answer
In standard library systems, you can use `realloc` with a size of `0`:
```c
int *ptr = malloc(10 * sizeof(int));
realloc(ptr, 0); // Acts identically to free(ptr) in C89/C99 (Deprecated in C11/C17)
```
In custom memory managers or bare-metal setups, you can implement a custom stack allocator where you just reset the allocator pointer to a previous marker position.

---

## Q3.10: Structure Padding and Packing

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

struct pointer{
    char a;
    int *ptr;
}

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

> **Trap:** Accessing unaligned members in a packed struct (like reading `b` from a non-word boundary) requires the CPU to make multiple memory access cycles, degrading performance, or causing a **hardware alignment fault** on some architectures like ARM Cortex-M0.

---

## Q3.11: Endianness (Byte Ordering)

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

### Q3.11a: How `char a[] = {0xAA, 0xBB, 0xCC, 0xDD}` is stored?
Since it is a **byte array** (individual characters), it is stored in declaration order regardless of endianness.
```
Address: 0x100 -> 0xAA, 0x101 -> 0xBB, 0x102 -> 0xCC, 0x103 -> 0xDD
```

### Q3.11b: How is a string stored in memory regarding endianness?
Strings are character arrays. Each character is 1 byte. Hence, strings are stored sequentially and are **unaffected by endianness**.

### Q3.11c: Does Endianness affect bits or bytes?
Endianness affects **bytes**, not the individual bits within a byte. The ordering of bits inside a single byte is fixed by hardware serializing mechanisms.

### Q3.11d: C Program to detect Endianness
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

### Q3.11e: Convert Endianness using Union and Bitwise
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

## Q3.12: Memory Corruption — Types, Detection, and Prevention

Memory corruption occurs when a program writes data to a memory location that it does not own or beyond the boundary of an allocated buffer. It is one of the most dangerous and hardest-to-debug classes of bugs in embedded systems and C programming.

### Types of Memory Corruption

#### 1. Buffer Overflow (Stack Smashing)
Writing past the end of a stack-allocated buffer, overwriting the return address or other local variables.
```c
void vulnerable() {
    char buf[8];
    strcpy(buf, "This string is way too long!"); // Overwrites stack frame!
}
```

```
Stack Layout during overflow:
+---------------------+ High Address
| Return Address      | ← OVERWRITTEN! (Can redirect to attacker code)
+---------------------+
| Saved Frame Pointer | ← OVERWRITTEN!
+---------------------+
| buf[7]..buf[0]      | ← Legitimate buffer
+---------------------+ Low Address
     ▲
     │ Overflow writes beyond buf[7] upward into return address
```

#### 2. Heap Buffer Overflow
Writing past the end of a heap-allocated buffer, corrupting heap metadata or adjacent heap chunks.
```c
int *arr = malloc(5 * sizeof(int));
arr[10] = 42; // Writing beyond allocated 5 elements!
```

#### 3. Use-After-Free
Accessing memory that has already been freed.
```c
int *p = malloc(sizeof(int));
*p = 42;
free(p);
printf("%d\n", *p); // UB: Use-After-Free! p is now a dangling pointer.
```

#### 4. Double-Free
Calling `free()` on the same pointer twice. This corrupts the heap's internal free-list metadata, potentially leading to arbitrary code execution.
```c
int *p = malloc(sizeof(int));
free(p);
free(p); // CRASH or heap corruption!
```

#### 5. Uninitialized Memory Read
Reading from a variable before it has been assigned a value.
```c
int x; // Uninitialized local variable (garbage value on stack)
if (x == 0) { /* May or may not execute — UB */ }
```

### Detection Tools
| Tool                        | Type                                            | What It Detects                                                   |
|-----------------------------|-------------------------------------------------|-------------------------------------------------------------------|
| **Valgrind (memcheck)**     | Dynamic (Runtime)                               | Memory leaks, use-after-free, uninitialized reads, invalid writes |
| **AddressSanitizer (ASan)** | Compiler instrumentation (`-fsanitize=address`) | Stack/heap buffer overflow, use-after-free, double-free           |
| **Stack Canary / Guard**    | Compiler (`-fstack-protector`)                  | Stack smashing / buffer overflow                                  |
| **Coverity / Cppcheck**     | Static analysis                                 | Potential null dereferences, buffer overflows, dead code          |

---

## Q3.13: Stack Corruption — Detection and Prevention

### What is Stack Corruption?
Stack corruption occurs when data is written beyond the boundaries of stack-allocated buffers, overwriting critical control data like the saved return address, frame pointer, or other local variables.

### How to Detect Stack Corruption
1. **Stack Canaries (GCC `-fstack-protector`):** The compiler inserts a known "canary" value between local variables and the saved return address. Before the function returns, it checks if the canary value has been modified. If so, the program aborts immediately.

```
Stack with Canary:
+---------------------+
| Return Address      |
+---------------------+
| 🐤 CANARY VALUE 🐤  | ← Checked before return. If modified → abort!
+---------------------+
| Local Variables     |
+---------------------+
```

2. **Stack Painting:** At startup, fill the entire stack memory with a known pattern (e.g., `0xDEADBEEF`). Periodically scan the stack from the bottom to find the high-water mark where the pattern is still intact. This reveals maximum stack usage.

3. **Hardware MPU (Memory Protection Unit):** Configure the MPU to mark a guard region below the stack as "no access." Any stack overflow into the guard region triggers a hardware fault.

### How to Prevent Stack Corruption
- Use `strncpy` / `snprintf` instead of `strcpy` / `sprintf`.
- Avoid variable-length arrays (VLAs) in safety-critical code.
- Limit recursion depth.
- Use `-fstack-protector-strong` compiler flag.
- Perform static analysis to calculate worst-case stack depth.

---

## Q3.14: Coredump — What, How to Generate, and Analysis

### What is a Coredump?
A coredump (core file) is a snapshot of a process's memory at the moment it crashed. It contains the stack, heap, register values, and memory mappings, allowing post-mortem debugging.

### How to Generate a Coredump (Linux)
```bash
# 1. Enable core dumps (set unlimited core file size)
ulimit -c unlimited

# 2. Compile with debug symbols
gcc -g -o myprogram myprogram.c

# 3. Run the program (it will crash and produce 'core' file)
./myprogram
# Output: Segmentation fault (core dumped)
```

### How to Analyze with GDB
```bash
gdb ./myprogram core

# Inside GDB:
(gdb) bt          # Print backtrace (call stack at crash)
(gdb) frame 0     # Select the crashing frame
(gdb) info locals  # Print local variables in the frame
(gdb) print *ptr   # Inspect specific variables
(gdb) info registers # View CPU register values at crash
```

---

## Q3.15: Internal vs External Fragmentation

- **Internal Fragmentation:** Wasted memory **inside** an allocated block. Occurs when the allocator gives more memory than requested (e.g., requesting 12 bytes but allocator gives a 16-byte aligned block — 4 bytes are wasted internally).
- **External Fragmentation:** Free memory exists but is split into small non-contiguous chunks that cannot satisfy a large allocation request.

```
External Fragmentation:
[Used][Free 2KB][Used][Free 1KB][Used][Free 3KB]
  ← Total free = 6KB, but cannot allocate a contiguous 5KB block! →
```

---
---

# 4. Bit Manipulation

---

## Q4.1: Bitwise Operators Cheat Sheet

- **Set a bit:** `num |= (1 << pos)`
- **Clear a bit:** `num &= ~(1 << pos)`
- **Toggle a bit:** `num ^= (1 << pos)`
- **Check a bit:** `(num & (1 << pos)) != 0`

---

## Q4.2: Set/Clear Bit 17 & 18 of a Number

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

## Q4.3: Replace Bits 2 to 4 of A with B

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
    printf("Result: 0x%02X\n", replace_bits(A, B)); // Outputs 0xD7 (0b11010111)
    return 0;
}
```

---

## Q4.4: Map A = 11011101 to C = 01011101 using B = 010

```c
unsigned char A = 0xDD; // 1101 1101
unsigned char B = 0x02; // 010
// We want to clear bits 7:5 (0xE0) and write B there:
unsigned char C = (A & 0x1F) | (B << 5);
```

---

## Q4.5: Count Number of Set Bits (Hamming Weight)

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

## Q4.6: Reverse Bits of an Integer

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

## Q4.7: Swap Adjacent Bits of a Binary Word

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

## Q4.8: Reverse Hex number `0x87` to `0xE1` and print in binary

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

## Q4.9: Reverse 32-bit Hex `0x12345678` to `0x87654321`

Note: This is a **nibble-level reversal** (digit by digit), not a bit-level reversal.

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

## Q4.10: Binary Gap (Longest consecutive zeros)

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

## Q4.11: Find if a number is a Power of 2

A power of 2 in binary has exactly one set bit (e.g., `4` = `0100`, `8` = `1000`).
If we perform `n & (n - 1)`, it will clear the only set bit, resulting in `0`.

```c
#include <stdbool.h>

bool is_power_of_two(int n) {
    if (n <= 0) return false;
    return (n & (n - 1)) == 0;
}
```

---

## Q4.12: Parity Corrected Byte from User Input

Parity is an error-detection method. A parity bit is set so the total number of `1` bits in the byte (including the parity bit) is even (even parity) or odd (odd parity).

```c
#include <stdio.h>
#include <stdint.h>

// Calculate even parity bit for 7-bit data
uint8_t add_even_parity(uint8_t data_7bit) {
    int count = 0;
    uint8_t temp = data_7bit;
    while (temp) {
        count += (temp & 1);
        temp >>= 1;
    }
    if (count % 2 != 0) {
        data_7bit |= (1 << 7); // Set parity bit
    }
    return data_7bit;
}

// Check even parity of 8-bit byte
int check_even_parity(uint8_t byte) {
    int count = 0;
    while (byte) {
        count += (byte & 1);
        byte >>= 1;
    }
    return (count % 2 == 0); // Returns 1 if parity is correct
}

int main() {
    uint8_t data = 0b0110101; // 7-bit data
    uint8_t with_parity = add_even_parity(data);
    printf("With parity: 0x%02X\n", with_parity);
    printf("Parity valid: %d\n", check_even_parity(with_parity));
    return 0;
}
```

---
---

# 5. Data Structures & Algorithms

---

## Q5.1: Reverse a String in C

### 1. In-place Iterative Reversal
```c
#include <stdio.h>
#include <string.h>

void reverse_string_inplace(char *str) {
    if (!str) return;
    int len = strlen(str);
    int i = 0, j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
```

### 2. Recursive Reversal
```c
void reverse_string_recursive(char *str, int start, int end) {
    if (start >= end) return;
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverse_string_recursive(str, start + 1, end - 1);
}
```

---

## Q5.2: String Operations Implemented from Scratch

```c
#include <stdio.h>
#include <stddef.h>

// 1. strlen (length of string)
size_t my_strlen(const char *str) {
    const char *s = str;
    while (*s) s++;
    return s - str;
}

// 2. strcmp (compare strings)
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// 3. strcpy (copy string)
char* my_strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

// 4. strcat (concatenate string)
char* my_strcat(char *dest, const char *src) {
    char *d = dest;
    while (*d) d++;
    while ((*d++ = *src++));
    return dest;
}

// 5. strstr (find substring)
char* my_strstr(const char *haystack, const char *needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; haystack++) {
        if (*haystack == *needle) {
            const char *h = haystack;
            const char *n = needle;
            while (*h && *n && *h == *n) {
                h++;
                n++;
            }
            if (!*n) return (char*)haystack;
        }
    }
    return NULL;
}

// 6. strncpy (n-bounded copy)
char* my_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// 7. strncat (n-bounded concatenation)
char* my_strncat(char *dest, const char *src, size_t n) {
    char *d = dest;
    while (*d) d++;
    size_t i = 0;
    while (i < n && *src) {
        *d++ = *src++;
        i++;
    }
    *d = '\0';
    return dest;
}
```

---

## Q5.3: Implement `strcpy` in a Single Line

```c
char* my_strcpy_oneline(char *dest, const char *src) {
    while ((*dest++ = *src++));
    return dest;
}
```

---

## Q5.4: Substring Copy Function and Test Cases

```c
#include <stdio.h>
#include <string.h>

void copy_substring(const char *source, char *dest, int start, int end) {
    if (!source || !dest) {
        if (dest) dest[0] = '\0';
        return;
    }
    int len = strlen(source);
    
    if (start < 0 || start >= len || end < start) {
        dest[0] = '\0';
        return;
    }
    
    if (end >= len) {
        end = len - 1;
    }
    
    int i, j = 0;
    for (i = start; i <= end; i++) {
        dest[j++] = source[i];
    }
    dest[j] = '\0';
}
```

### Test Cases Matrix
1. **Normal Case:** `source = "Embedded"`, `start = 2`, `end = 5` -> `dest = "bedd"`.
2. **Invalid Indices:** `start = -1` or `start > end`.
3. **End index out of bounds:** `end` larger than string size.
4. **Empty strings / NULL pointers:** Check robustness.

---

## Q5.5: Most Frequent Character in "Hello world"

```c
#include <stdio.h>

char most_frequent_char(const char *str) {
    if (!str || *str == '\0') return '\0';
    
    int freq[256] = {0};
    
    for (int i = 0; str[i] != '\0'; i++) {
        freq[(unsigned char)str[i]]++;
    }
    
    char max_char = str[0];
    int max_count = freq[(unsigned char)str[0]];
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (freq[(unsigned char)str[i]] > max_count) {
            max_count = freq[(unsigned char)str[i]];
            max_char = str[i];
        }
    }
    return max_char;
}

int main() {
    char res = most_frequent_char("Hello world");
    printf("Most frequent: '%c'\n", res); // Output: 'l'
    return 0;
}
```

---

## Q5.6: Count Occurrences of Substring "to"

```c
#include <stdio.h>
#include <string.h>

int count_substring(const char *str, const char *sub) {
    int count = 0;
    int sub_len = strlen(sub);
    const char *temp = str;
    
    while ((temp = strstr(temp, sub)) != NULL) {
        count++;
        temp += sub_len;
    }
    return count;
}

int main() {
    char str[] = "Welcome to gdb Welcome to gdb Welcome to gdb";
    printf("Count: %d\n", count_substring(str, "to")); // Output: 3
    return 0;
}
```

---

## Q5.7: Singly Linked List Implementation

```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

void insert_at_head(struct Node **head, int val) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = val;
    new_node->next = *head;
    *head = new_node;
}

void insert_at_tail(struct Node **head, int val) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = val;
    new_node->next = NULL;
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    struct Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
}

void delete_by_value(struct Node **head, int val) {
    if (*head == NULL) return;
    struct Node *temp = *head;
    struct Node *prev = NULL;
    
    if (temp != NULL && temp->data == val) {
        *head = temp->next;
        free(temp);
        return;
    }
    
    while (temp != NULL && temp->data != val) {
        prev = temp;
        temp = temp->next;
    }
    
    if (temp == NULL) return;
    
    prev->next = temp->next;
    free(temp);
}

void print_list(struct Node *head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}
```

---

## Q5.8: Doubly Linked List Implementation

```c
#include <stdio.h>
#include <stdlib.h>

struct DLLNode {
    int data;
    struct DLLNode *next;
    struct DLLNode *prev;
};

void insert_front(struct DLLNode **head, int val) {
    struct DLLNode *new_node = (struct DLLNode*)malloc(sizeof(struct DLLNode));
    new_node->data = val;
    new_node->next = *head;
    new_node->prev = NULL;
    
    if (*head != NULL) {
        (*head)->prev = new_node;
    }
    *head = new_node;
}

void print_bidirectional(struct DLLNode *head) {
    struct DLLNode *last = NULL;
    printf("Forward: ");
    while (head != NULL) {
        printf("%d <-> ", head->data);
        last = head;
        head = head->next;
    }
    printf("NULL\nBackward: ");
    while (last != NULL) {
        printf("%d <-> ", last->data);
        last = last->prev;
    }
    printf("NULL\n");
}
```

---

## Q5.9: Reverse a Linked List

### 1. Iterative Reverse (O(N) Time, O(1) Space)

```c
struct Node* reverse_iterative(struct Node *head) {
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
```

### 2. Recursive Reverse
```c
struct Node* reverse_recursive(struct Node *head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    struct Node *rest = reverse_recursive(head->next);
    head->next->next = head;
    head->next = NULL;
    return rest;
}
```

---

## Q5.10: Print Linked List in Reverse using Recursion

```c
void print_reverse(struct Node *head) {
    if (head == NULL) return;
    print_reverse(head->next);
    printf("%d ", head->data);
}
```

---

## Q5.11: Find Middle Node of Linked List (Slow/Fast Pointer)

```c
struct Node* find_middle(struct Node *head) {
    if (head == NULL) return NULL;
    struct Node *slow = head;
    struct Node *fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
```

---

## Q5.12: Detect Loop in Linked List (Floyd's Cycle-Finding)

```c
#include <stdbool.h>

bool detect_loop(struct Node *head) {
    struct Node *slow = head;
    struct Node *fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
    return false;
}
```

---

## Q5.13: Find Nth Node from End

```c
struct Node* find_nth_from_end(struct Node *head, int n) {
    struct Node *main_ptr = head;
    struct Node *ref_ptr = head;
    
    for (int i = 0; i < n; i++) {
        if (ref_ptr == NULL) return NULL;
        ref_ptr = ref_ptr->next;
    }
    
    while (ref_ptr != NULL) {
        main_ptr = main_ptr->next;
        ref_ptr = ref_ptr->next;
    }
    return main_ptr;
}
```

---

## Q5.14: Replace Nth Node Value

```c
void replace_nth_node_value(struct Node *head, int n, int new_val) {
    struct Node *temp = head;
    for (int i = 1; temp != NULL && i < n; i++) {
        temp = temp->next;
    }
    if (temp != NULL) {
        temp->data = new_val;
    }
}
```

---

## Q5.15: Detect Intersection of Two Linked Lists

```c
struct Node* get_intersection_node(struct Node *headA, struct Node *headB) {
    if (!headA || !headB) return NULL;
    struct Node *ptrA = headA;
    struct Node *ptrB = headB;
    
    while (ptrA != ptrB) {
        ptrA = (ptrA == NULL) ? headB : ptrA->next;
        ptrB = (ptrB == NULL) ? headA : ptrB->next;
    }
    return ptrA;
}
```

---

## Q5.16: Use Cases of Circular Linked List in Embedded Systems

1. **Task Scheduler (Round Robin):** In operating systems (RTOS), active tasks are kept in a circular queue. The scheduler cycles through the list indefinitely giving CPU time to each task.
2. **Circular Buffers / Ring Buffers:** Used for serial interface queueing (UART/DMA packet buffers).

---

## Q5.17: Insert and Delete at Middle of Linked List

### Insert at Middle
```c
void insert_at_middle(struct Node **head, int val) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = val;
    
    if (*head == NULL) {
        new_node->next = NULL;
        *head = new_node;
        return;
    }
    
    struct Node *slow = *head, *fast = *head;
    struct Node *prev = NULL;
    
    while (fast != NULL && fast->next != NULL) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    
    new_node->next = slow;
    if (prev != NULL) {
        prev->next = new_node;
    } else {
        *head = new_node;
    }
}
```

### Delete at Middle
```c
void delete_middle(struct Node **head) {
    if (*head == NULL || (*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }
    
    struct Node *slow = *head, *fast = *head;
    struct Node *prev = NULL;
    
    while (fast != NULL && fast->next != NULL) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    
    prev->next = slow->next;
    free(slow);
}
```

---

## Q5.18: Print Nth Node from Start

```c
void print_nth_node(struct Node *head, int n) {
    struct Node *temp = head;
    int count = 1;
    
    while (temp != NULL) {
        if (count == n) {
            printf("Node %d: %d\n", n, temp->data);
            return;
        }
        count++;
        temp = temp->next;
    }
    printf("List has fewer than %d nodes\n", n);
}
```

---

## Q5.19: Stack Implementation using Structures

```c
#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100

struct Stack {
    int arr[MAX_SIZE];
    int top;
};

void init_stack(struct Stack *s) {
    s->top = -1;
}

bool isFull(struct Stack *s) {
    return s->top == MAX_SIZE - 1;
}

bool isEmpty(struct Stack *s) {
    return s->top == -1;
}

void push(struct Stack *s, int val) {
    if (isFull(s)) {
        printf("Stack Overflow\n");
        return;
    }
    s->arr[++(s->top)] = val;
}

int pop(struct Stack *s) {
    if (isEmpty(s)) {
        printf("Stack Underflow\n");
        return -1;
    }
    return s->arr[(s->top)--];
}

int peek(struct Stack *s) {
    if (isEmpty(s)) return -1;
    return s->arr[s->top];
}
```

---

## Q5.20: Sorting Techniques

### Comparison Table

| Algorithm          | Best Time  | Average Time | Worst Time | Space Complexity | Stable? | When to Use                     |
|--------------------|------------|--------------|------------|------------------|---------|---------------------------------|
| **Bubble Sort**    | O(N)       | O(N²)        | O(N²)      | O(1)             | Yes     | Almost-sorted small arrays      |
| **Selection Sort** | O(N²)      | O(N²)        | O(N²)      | O(1)             | No      | Minimize memory write cycles    |
| **Insertion Sort** | O(N)       | O(N²)        | O(N²)      | O(1)             | Yes     | Real-time streaming data        |
| **Merge Sort**     | O(N log N) | O(N log N)   | O(N log N) | O(N)             | Yes     | Stable sorting of linked lists  |
| **Quick Sort**     | O(N log N) | O(N log N)   | O(N²)      | O(log N)         | No      | Default general purpose sorting |

### Selection Sort Implementation
```c
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}
```

---

## Q5.21: Matrix Operations in C

```c
#include <stdio.h>

#define N 3

void add_matrices(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void multiply_matrices(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
```

---

## Q5.22: Linear vs Non-Linear Data Structures

| Feature       | Linear Data Structure                     | Non-Linear Data Structure                            |
|---------------|-------------------------------------------|------------------------------------------------------|
| **Structure** | Elements arranged in sequential order     | Elements arranged in a hierarchical / net-like order |
| **Traversal** | Single run traversal is possible          | Requires multiple runs or complex recursion          |
| **Levels**    | Single level                              | Multi-level                                          |
| **Examples**  | Array, Linked List, Stack, Queue          | Binary Tree, BST, Heap, Graph                        |
| **Memory**    | Typically contiguous or simple node links | Highly fragmented/dynamic nodes pointers             |

---

## Q5.23: Second Highest Element in an Array

```c
#include <stdio.h>
#include <limits.h>

int second_highest(int arr[], int n) {
    if (n < 2) return INT_MIN;
    
    int first = INT_MIN, second = INT_MIN;
    
    for (int i = 0; i < n; i++) {
        if (arr[i] > first) {
            second = first;
            first = arr[i];
        } else if (arr[i] > second && arr[i] != first) {
            second = arr[i];
        }
    }
    return second;
}

int main() {
    int arr[] = {12, 35, 1, 10, 34, 1};
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Second Highest: %d\n", second_highest(arr, n)); // Output: 34
    return 0;
}
```

---

## Q5.24: Maximum Repeating Element in an Array

```c
#include <stdio.h>

int max_repeating(int arr[], int n) {
    int freq[256] = {0};
    
    for (int i = 0; i < n; i++) {
        freq[arr[i]]++;
    }
    
    int max_count = 0, max_element = arr[0];
    for (int i = 0; i < n; i++) {
        if (freq[arr[i]] > max_count) {
            max_count = freq[arr[i]];
            max_element = arr[i];
        }
    }
    return max_element;
}
```

---

## Q5.25: Maximum Sum Subarray (Kadane's Algorithm)

```c
#include <stdio.h>

int max_subarray_sum(int arr[], int n) {
    int max_ending_here = arr[0];
    int max_so_far = arr[0];
    
    for (int i = 1; i < n; i++) {
        max_ending_here = (arr[i] > max_ending_here + arr[i]) ? arr[i] : max_ending_here + arr[i];
        
        if (max_ending_here > max_so_far) {
            max_so_far = max_ending_here;
        }
    }
    return max_so_far;
}

int main() {
    int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Maximum subarray sum: %d\n", max_subarray_sum(arr, n)); // Output: 6
    return 0;
}
```
**Time complexity:** O(N). **Space complexity:** O(1).

---

## Q5.26: Find Odd/Even Inside a Thread Function

```c
#include <stdio.h>
#include <pthread.h>

void* check_odd_even(void *arg) {
    int num = *(int*)arg;
    
    if (num & 1) {
        printf("%d is Odd\n", num);
    } else {
        printf("%d is Even\n", num);
    }
    return NULL;
}

int main() {
    pthread_t tid;
    int number = 17;
    
    pthread_create(&tid, NULL, check_odd_even, &number);
    pthread_join(tid, NULL);
    
    return 0;
}
// Compile: gcc -pthread odd_even_thread.c -o odd_even_thread
```

---
---

# 6. Embedded Systems, Protocols & Hardware

---

## Q6.1: How I2C Works (Master-Slave Communication)

I2C (Inter-Integrated Circuit) is a synchronous, multi-master, multi-slave, half-duplex, 2-wire serial bus. The two lines are:
- **SDA (Serial Data Line):** Bidirectional line for data transfer.
- **SCL (Serial Clock Line):** Clock line driven by the master.

Both lines are open-drain, meaning they must be pulled up to VCC via external pull-up resistors.

### Communication Steps
1. **Idle State:** Both SDA and SCL are pulled High.
2. **Start Condition (S):** Master pulls SDA Low while SCL is High.
3. **Address Frame:** Master transmits 7-bit slave address + 1 Read/Write bit (0 for Write, 1 for Read).
4. **ACK/NACK:** The slave with the matching address pulls SDA Low (ACK) during the 9th clock pulse.
5. **Data Frame:** Master or Slave transmits 8-bit data.
6. **ACK/NACK:** Receiver pulls SDA Low to acknowledge the byte.
7. **Stop Condition (P):** Master transitions SDA from Low to High while SCL is High.

---

## Q6.2: SCL State during SDA Start Bit

At the start of an I2C transaction, the **Start Condition** is defined as a High-to-Low transition on the SDA line while SCL is **High**. Therefore, SCL must be **High** when the Start bit begins.

```
SCL:  ───────┐
             └──────
SDA:  ────┐
          └─────────
          ^ START Bit (SDA falls while SCL is High)
```

---

## Q6.3: Is I2C or SPI Asynchronous?

**Neither.** Both I2C and SPI are **synchronous** protocols because they both utilize a dedicated clock line (SCL in I2C, SCK in SPI) to synchronize data transfer. 
- **UART** is the asynchronous protocol among common serial protocols (it has no clock line and relies on pre-configured baud rates).

---

## Q6.4: SPI Protocol CPOL and CPHA Modes

SPI (Serial Peripheral Interface) is a synchronous, full-duplex, 4-wire serial bus:
- **MOSI:** Master Out Slave In
- **MISO:** Master In Slave Out
- **SCLK:** Serial Clock
- **SS/CS:** Slave Select / Chip Select (Active Low)

### SPI Mode Table
| SPI Mode   | CPOL | CPHA | Sample Edge  | Shift Edge   |
|------------|------|------|--------------|--------------|
| **Mode 0** | 0    | 0    | Rising Edge  | Falling Edge |
| **Mode 1** | 0    | 1    | Falling Edge | Rising Edge  |
| **Mode 2** | 1    | 0    | Falling Edge | Rising Edge  |
| **Mode 3** | 1    | 1    | Rising Edge  | Falling Edge |

### Multi-Master SPI
If multiple masters share the SPI bus, they must use hardware chip select pins or software arbitration to ensure only one master drives the SCLK, MOSI, and SS lines at any given time.

### SPI Daisy Chain
Slaves are connected in series. The MISO of Slave 1 goes to MOSI of Slave 2, and so on. A single CS line controls all slaves. Data overflows from one slave to the next like a giant shift register.

### Quad-SPI (Q-SPI)
Q-SPI uses 4 data lines (IO0, IO1, IO2, IO3) instead of single MOSI/MISO lines. This allows transferring 4 bits per clock cycle, quadrupling throughput. Used primarily for interfacing with high-speed external Flash memories.

---

## Q6.5: UART Protocol Frame Format & Flow Control

UART (Universal Asynchronous Receiver-Transmitter) uses two lines: TX and RX.

### Frame Format
```
Idle (High) ──┐      ┌───┬───┬───┬───┬───┬───┬───┬───┬───────┬───────┐
              │      │ D0│ D1│ D2│ D3│ D4│ D5│ D6│ D7│Parity │ Stop  │─── Idle (High)
              └──────┴───┴───┴───┴───┴───┴───┴───┴───┴───────┴───────┘
              ^ START  (Data Bits, LSB first)                 ^ STOP
                (Low)                                           (High)
```

### RTS/CTS Hardware Flow Control
- **RTS (Request to Send):** Receiver pulls this Low when its buffer is empty, indicating it is ready to receive data.
- **CTS (Clear to Send):** Transmitter checks this line. If the receiver pulls it Low, the transmitter begins sending data.

---

## Q6.6: CAN Bus Protocol (Arbitration, Frame, Errors)

CAN (Controller Area Network) is a differential, multi-master, asynchronous serial bus.

### CAN Bit-by-Bit Arbitration
CAN uses dominant bits (`0`) and recessive bits (`1`). The message with the lower numerical ID has higher priority.

```
Node A (ID 0x0A = 0000 1010):  0 0 0 0 1 0 1 0
Node B (ID 0x0C = 0000 1100):  0 0 0 0 1 1 [Loses Bus! Stops transmitting]
Bus State:                     0 0 0 0 1 0 1 0
```

### CAN Frame Format
- **SOF:** Start of Frame (1 dominant bit).
- **Arbitration Field:** 11-bit identifier + RTR.
- **Control Field:** IDE + r0 + DLC (Data Length Code).
- **Data Field:** 0 to 8 bytes.
- **CRC Field:** 15-bit CRC + CRC Delimiter.
- **ACK Field:** ACK Slot + ACK Delimiter.
- **EOF:** End of Frame (7 recessive bits).

### 5 CAN Error Types
1. **Bit Error:** Node reads back a different bit value than what it transmitted.
2. **Stuff Error:** Violating the bit-stuffing rule (more than 5 consecutive identical bits).
3. **CRC Error:** Calculated CRC does not match the received frame CRC.
4. **Form Error:** Invalid structure inside standard fields.
5. **ACK Error:** Transmitter does not detect a dominant bit in the ACK slot.

---

## Q6.7: Synchronous vs Asynchronous Communication

| Feature        | Synchronous                     | Asynchronous                              |
|----------------|---------------------------------|-------------------------------------------|
| **Clock Line** | Shared clock between devices    | No shared clock                           |
| **Timing**     | Data sampled on clock edges     | Data sampled based on baud rate agreement |
| **Protocols**  | SPI, I2C                        | UART, CAN                                 |
| **Speed**      | Generally faster (clock-driven) | Slower (overhead of start/stop bits)      |
| **Wiring**     | More wires (extra clock line)   | Fewer wires                               |

---

## Q6.8: GPIO Initialization (STM32 Register-Level)

### Code Example (STM32 Cortex-M4 register-level C)
```c
#include <stdint.h>

#define RCC_AHB1ENR     (*(volatile uint32_t*)(0x40023800 + 0x30))
#define GPIOC_MODER     (*(volatile uint32_t*)(0x40020800 + 0x00))
#define GPIOC_OTYPER    (*(volatile uint32_t*)(0x40020800 + 0x04))
#define GPIOC_PUPDR     (*(volatile uint32_t*)(0x40020800 + 0x0C))

void GPIO_Init_PC13_Output(void) {
    RCC_AHB1ENR |= (1 << 2);           // Enable Clock for GPIOC
    GPIOC_MODER &= ~(3 << (13 * 2));   // Clear mode bits
    GPIOC_MODER |=  (1 << (13 * 2));   // Set mode to output
    GPIOC_OTYPER &= ~(1 << 13);        // Push-Pull
    GPIOC_PUPDR &= ~(3 << (13 * 2));   // No Pull
}
```

---

## Q6.9: Software Debouncing Logic

Mechanical switches bounce when pressed, causing false triggers. Software debouncing filters this by sampling the pin over a period.

```
Switch State: High ───┐   ┌─┐   ┌─┐   ┌───────── Low (Pressed)
                      └───┘ └───┘ └───┘
                      <── Bouncing ──> <── Stable ──>
```

---

## Q6.10: ADC Voltage Divider Calculation

Read 20V input using a 10-bit ADC with a reference voltage of 3.5V. Directly connecting 20V will destroy the MCU. Use a voltage divider to scale 20V down to a maximum of 3.5V.

---

## Q6.11: DMA (Direct Memory Access)

DMA allows hardware peripherals to send/receive data directly to/from the main RAM without involving the CPU.

### Solving High-Speed Byte-by-Byte Interrupt problem
Configure the DMA controller in Circular/Double-buffer mode to transfer incoming bytes automatically into a RAM buffer. Set the DMA to trigger an interrupt only when the buffer is half-full or completely full.

---

## Q6.12: Microcontroller Startup Sequence

Before `main()` is executed:

1. **Hardware Reset:** CPU reads the vector table starting at flash memory address `0x00000000`.
2. **MSP Load:** Loads the Initial Stack Pointer value from the first entry of the vector table.
3. **Program Counter Load:** Loads the address of the `Reset_Handler` (second entry) into the PC register.
4. **Execution of `Reset_Handler`:**
   - Copies initialized global/static variable values from Flash to SRAM (`.data` segment).
   - Zeroes out the `.bss` segment in SRAM.
   - Configures system clocks (PLL setup via `SystemInit()`).
5. **Jump to `main`.**

---

## Q6.13: Watchdog Timer — Types and Configuration

A watchdog timer is a countdown timer. If the software does not reset ("kick" or "feed") the watchdog before it reaches zero, it triggers a hardware reset.

### Independent Watchdog (IWDG) vs Window Watchdog (WWDG)

| Feature          | IWDG (Independent)                  | WWDG (Window)                                 |
|------------------|-------------------------------------|-----------------------------------------------|
| **Clock Source** | Internal low-speed oscillator (LSI) | APB clock (system clock derived)              |
| **Feed Window**  | Anytime before timeout              | Only within a specific time window            |
| **Detection**    | Software freeze (infinite loop)     | Software timing errors (too fast or too slow) |
| **Use Case**     | General system crash recovery       | Catching control flow anomalies               |

---

## Q6.14: General Purpose Timer Initialization

Steps to configure a hardware timer on a typical ARM Cortex-M MCU:

1. **Enable Timer Clock** via RCC register.
2. **Set Prescaler (PSC):** `Timer_Clock = APB_Clock / (PSC + 1)`.
3. **Set Auto-Reload Register (ARR).**
4. **Select Counting Mode.**
5. **Enable Update Interrupt (optional).**
6. **Enable the Timer** via CEN bit.

### Example: Generate 1-second interrupt with 72 MHz APB clock
```
Prescaler = 7200 - 1 = 7199   → Timer clock = 72 MHz / 7200 = 10 kHz
ARR = 10000 - 1 = 9999        → Overflow at 10000 counts / 10 kHz = 1 second
```

---

## Q6.15: FPU (Floating Point Unit) in Microcontrollers

| Core            | FPU Support                                 |
|-----------------|---------------------------------------------|
| Cortex-M0 / M0+ | ❌ No FPU (Software emulation)               |
| Cortex-M3       | ❌ No FPU                                    |
| Cortex-M4**F**  | ✅ Single-precision FPU (32-bit float)       |
| Cortex-M7       | ✅ Single + Double precision FPU (32/64-bit) |

---

## Q6.16: Oscilloscope vs Logic Analyzer

| Feature         | Oscilloscope                                  | Logic Analyzer                                    |
|-----------------|-----------------------------------------------|---------------------------------------------------|
| **Signal Type** | Analog (continuous voltage waveform)          | Digital (High/Low logic levels only)              |
| **Channels**    | Typically 2–4                                 | Typically 8–32+                                   |
| **Use Case**    | Signal integrity, noise, rise time, amplitude | Protocol decoding, timing analysis, state capture |
| **Displays**    | Voltage vs. Time waveform                     | Timing diagrams / State tables                    |

---

## Q6.17: Microcontroller Memory — Flash, SRAM, DRAM

| Memory Type     | Volatility   | Speed                    | Cost      | Use Case                               |
|-----------------|--------------|--------------------------|-----------|----------------------------------------|
| **Flash (NOR)** | Non-volatile | Medium                   | Medium    | Program storage (code), bootloader     |
| **SRAM**        | Volatile     | Very Fast                | Expensive | Data memory, stack, heap, variables    |
| **DRAM**        | Volatile     | Fast (but needs refresh) | Cheap     | External RAM for large data buffers    |
| **EEPROM**      | Non-volatile | Slow                     | Medium    | Configuration data, calibration values |

---

## Q6.18: Bootloader Memory Layout (Flash and RAM)

```
Flash Memory Layout:
+────────────────────────────+ 0x0800_0000 (Flash Start)
| Bootloader (16 KB)         |
|   - Vector Table           |
|   - Bootloader Code        |
|   - CRC / Version Info     |
+────────────────────────────+ 0x0800_4000
| Application Firmware       |
|   - App Vector Table       |
|   - Application Code       |
|   - .rodata (Constants)    |
+────────────────────────────+ 0x080X_XXXX (Flash End)

RAM (SRAM) Layout:
+────────────────────────────+ 0x2000_0000 (SRAM Start)
| .data (Initialized globals)|
+────────────────────────────+
| .bss (Zero-init globals)   |
+────────────────────────────+
| Heap (grows ↑)             |
|            ...              |
| Stack (grows ↓)            |
+────────────────────────────+ 0x2000_XXXX (SRAM End)
```

---

## Q6.19: What Makes ARM Stand Out from Other Processors?

1. **RISC Architecture:** Simpler instruction set with fixed-length instructions.
2. **Thumb/Thumb-2 Instruction Set:** 16-bit compressed instructions reduce code density by ~30%.
3. **Low Power Consumption:** Sleep/Deep Sleep modes, WFI/WFE instructions.
4. **Licensing Model:** ARM licenses IP designs, allowing hundreds of silicon vendors to customize.
5. **Rich Ecosystem:** Massive toolchain support (GCC, Keil, IAR), CMSIS standard.

---

## Q6.20: ARM vs x86 Architecture

| Feature                | ARM                            | x86 (Intel/AMD)                             |
|------------------------|--------------------------------|---------------------------------------------|
| **ISA Type**           | RISC (Reduced Instruction Set) | CISC (Complex Instruction Set)              |
| **Power**              | Very low power                 | High power consumption                      |
| **Instruction Length** | Fixed (32-bit or 16-bit Thumb) | Variable (1–15 bytes)                       |
| **Registers**          | 16 general-purpose (R0–R15)    | 8 GP registers (EAX, EBX, etc.) / 16 in x64 |
| **Endianness**         | Bi-endian (configurable)       | Little-endian only                          |
| **Market**             | Mobile, IoT, Embedded, MCU     | Desktops, Servers, Laptops                  |

---

## Q6.21: Von Neumann vs Harvard Architecture

| Feature        | Von Neumann                                 | Harvard                                   |
|----------------|---------------------------------------------|-------------------------------------------|
| **Buses**      | Single shared bus for instructions and data | Separate instruction and data buses       |
| **Speed**      | Bottleneck (bus contention)                 | Faster (simultaneous fetch + data access) |
| **Complexity** | Simpler hardware                            | More complex (duplicate bus wiring)       |
| **Example**    | x86 processors                              | ARM Cortex-M (Modified Harvard), DSPs     |

---

## Q6.22: Reset Handler and Its Role in Boot Process

The Reset Handler is the very first piece of code that runs after a hardware reset. Its address is stored at offset `0x00000004` in the Interrupt Vector Table.

### What the Reset Handler Does:
1. Sets up the stack pointer (if not done by hardware).
2. Copies `.data` section from Flash to SRAM.
3. Zeroes the `.bss` section in SRAM.
4. Calls `SystemInit()` to configure the system clock.
5. Calls C library initialization (`__libc_init_array` for C++ constructors).
6. Jumps to `main()`.

---

## Q6.23: Cold Boot vs Warm Boot

| Feature           | Cold Boot                                       | Warm Boot                                          |
|-------------------|-------------------------------------------------|----------------------------------------------------|
| **Trigger**       | Power applied from completely OFF state         | Software reset or watchdog reset while power is ON |
| **RAM State**     | Cleared / Random values                         | May retain content (depending on reset type)       |
| **Hardware Init** | Full (clock config, PLL lock, peripheral reset) | Partial (may skip PLL lock if already running)     |
| **Boot Time**     | Longer (full initialization)                    | Shorter (reduced init sequence)                    |

---

## Q6.24: Startup File Contents

The startup file (`startup_stm32f4xx.s` or equivalent) contains:
1. **Initial Stack Pointer value** (placed at vector table entry 0).
2. **Reset Handler address** (vector table entry 1).
3. **Interrupt Vector Table** (array of ISR function addresses, defaulting to weak handlers).
4. **Reset Handler implementation** (copies `.data`, zeros `.bss`, calls `SystemInit`, calls `main`).
5. **Default Handler** (infinite loop, catches unhandled interrupts).

---

## Q6.25: Memory Hierarchy

```
                 ┌─────────────┐  Fastest, Smallest, Most Expensive
                 │  Registers  │  (CPU internal, ~1 ns)
                 ├─────────────┤
                 │  L1 Cache   │  (32–64 KB, ~1–2 ns)
                 ├─────────────┤
                 │  L2 Cache   │  (256 KB–1 MB, ~3–10 ns)
                 ├─────────────┤
                 │  L3 Cache   │  (2–32 MB, ~10–30 ns)
                 ├─────────────┤
                 │  Main RAM   │  (DRAM, 4–64 GB, ~50–100 ns)
                 ├─────────────┤
                 │  SSD / Flash│  (~100 μs)
                 ├─────────────┤
                 │  HDD / Disk │  (~10 ms)
                 └─────────────┘  Slowest, Largest, Cheapest
```

---
---

# 7. Interrupts & RTOS

---

## Q7.1: What is an Interrupt? Working Flow

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

---

## Q7.2: Interrupt Vector Table (IVT)

The Interrupt Vector Table is an array of memory addresses located at a fixed location in memory. Each entry (vector) points to the start address of its corresponding ISR handler.

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

## Q7.3: ISR Rules and Best Practices

1. **Keep it short and fast:** Do not block.
2. **No blocking calls:** Do not use `delay()`, sleep, or wait for mutexes inside an ISR.
3. **No dynamic memory allocation:** Avoid `malloc` / `free`.
4. **Use `volatile` for shared variables.**
5. **No heavy I/O operations:** Avoid calling `printf` or writing to files.
6. **Top-Half / Bottom-Half processing:** Perform critical work in the ISR, defer long-running computations to a background thread.

### Can an interrupt exist without an ISR?
Yes. If no ISR address is specified, triggering it will load a null/garbage address into the PC, leading to a CPU crash (HardFault). Standard templates point unused vectors to a default dummy handler.

---

## Q7.4: Maskable vs Non-Maskable Interrupts (NMI)

- **Maskable Interrupts:** Can be disabled by clearing a bit in the interrupt mask register.
- **Non-Maskable Interrupts (NMI):** Cannot be disabled by software. Highest priority.
  - **Examples:** Power failure warning, hardware watchdog reset, ECC memory errors.

---

## Q7.5: Nested Interrupts (Preemption)

If a new interrupt has **higher priority** than the executing one, it preempts the current ISR. If **lower or equal priority**, it is set to **pending** and executes after the current ISR finishes.

---

## Q7.6: Interrupt Latency

Interrupt latency is the time delay between the hardware trigger and the execution of the first instruction in the ISR.

### How to measure it?
1. **Oscilloscope:** Toggle a GPIO pin at the start of the ISR.
2. **Hardware Timers:** Capture a free-running timer register on trigger.

---

## Q7.7: Interrupt Coalescing

A technique where a hardware peripheral buffers multiple incoming events and generates a single interrupt for all of them, preventing an interrupt storm.

---

## Q7.8: OS vs RTOS

| Feature               | General Purpose OS (Windows/Linux)      | RTOS (FreeRTOS, VxWorks)              |
|-----------------------|-----------------------------------------|---------------------------------------|
| **Determinism**       | Non-deterministic (Best effort latency) | Deterministic (Strict latency limits) |
| **Scheduling**        | Fairness-based (Time slicing sharing)   | Priority-based preemptive scheduling  |
| **Footprint**         | Large (Gigabytes)                       | Extremely small (Kilobytes)           |
| **Interrupt Latency** | Variable                                | Minimal and bounded                   |

---

## Q7.9: Preemptive Scheduling Preemption Flow

In a preemptive RTOS, the scheduler guarantees that the highest-priority task ready to run is always allocated CPU execution.

```
Priority 4:  (High)                   [Task 4 Runs]
Priority 3:            [Task 3 Runs]────────────────► (Preempted)
Priority 2: [Task 2] ────────────────► (Preempted)
Priority 1:  (Low)
```

---

## Q7.10: Priority Inversion and Priority Inheritance

```
Priority Inversion Scenario:
1. Low Task (P1) grabs Mutex M.
2. High Task (P3) wakes up, tries to grab Mutex M, blocks.
3. Medium Task (P2) wakes up and preempts Low Task (P1).
Result: High Task (P3) is blocked by Medium Task (P2) indefinitely!
```

### Solution: Priority Inheritance
When High Task (P3) blocks on the mutex held by Low Task (P1), the system temporarily raises P1's priority to P3. This prevents the Medium Task from preempting P1.

---

## Q7.11: RTOS Synchronization Techniques

### 1. Mutex (Mutual Exclusion)
- Has an **owner** (only the thread that locked it can unlock it).
- Implements **priority inheritance**.

### 2. Semaphores
- **Binary Semaphore:** Used for signaling. No owner concept.
- **Counting Semaphore:** Tracks resource counts.

### 3. Spinlock vs Mutex
- **Mutex:** Puts the waiting thread to sleep. Good for long waits.
- **Spinlock:** Busy-waiting in a loop. Good for short waits, mandatory inside ISR context.

---

## Q7.12: Deadlocks

Two or more threads are unable to proceed because each is waiting for the other to release a resource.

### How to avoid:
1. **Lock ordering:** Always acquire resources in the same order.
2. **Timeout:** Use lock acquisition calls that timeout rather than blocking indefinitely.

---

## Q7.13: Static Variable inside an RTOS Task

A static variable defined inside an RTOS task function is stored in the global `.data` or `.bss` memory segment, NOT on the task's stack.
- **Trap:** If you run multiple instances of the same task function, they will all share the same static variable instance, leading to data corruption.

---

## Q7.14: ISR1 and ISR2 Sharing a Resource — Making it Atomic

### Solutions

#### 1. Disable Interrupts (Critical Section in ISR)
```c
void ISR1_Handler(void) {
    __disable_irq();   // Enter critical section
    shared_buffer[index++] = new_data;
    __enable_irq();    // Exit critical section
}
```

#### 2. Priority Management (NVIC)
Set both ISRs to the **same priority level** to prevent preemption.

#### 3. Lock-Free Data Structures
Use atomic operations or lock-free ring buffers (single-producer/single-consumer pattern).

---

## Q7.15: Reading SPI Data Inside ISR — Synchronization

Use a **Ring Buffer:** The ISR reads SPI bytes and writes them into a circular buffer. The main loop reads from the buffer at its own pace.

```c
#define BUF_SIZE 256
volatile uint8_t rx_buf[BUF_SIZE];
volatile uint16_t head = 0, tail = 0;

void SPI_IRQHandler(void) {
    uint8_t data = SPI_DR;
    uint16_t next_head = (head + 1) % BUF_SIZE;
    if (next_head != tail) {
        rx_buf[head] = data;
        head = next_head;
    }
    SPI_SR &= ~SPI_FLAG_RXNE;
}
```

---

## Q7.16: Top-Half / Bottom-Half Interrupt Processing (Linux Kernel)

```
Interrupt Trigger
       │
       ▼
  [ Top-Half (ISR) ]          ← Runs in interrupt context (fast, non-blocking)
   │  - Acknowledge hardware
   │  - Read critical data
   │  - Schedule bottom-half
   │
   ▼
  [ Bottom-Half ]              ← Runs in process/kernel context (can sleep, block)
   │  - Complex data processing
   │  - Network packet handling
```

| Mechanism        | Context          | Can Sleep? | Use Case                                          |
|------------------|------------------|------------|---------------------------------------------------|
| **Softirq**      | Soft interrupt   | ❌ No       | High-frequency processing (networking, block I/O) |
| **Workqueue**    | Kernel thread    | ✅ Yes      | Heavy processing, can call blocking APIs          |
| **Threaded IRQ** | Kernel thread    | ✅ Yes      | Real-time safe deferred handling                  |
| **Tasklet**      | Built on softirq | ❌ No       | Lightweight deferred work                         |

---

## Q7.17: Preemptive vs Non-Preemptive Scheduling

| Feature            | Preemptive Scheduling                               | Non-Preemptive (Cooperative)                   |
|--------------------|-----------------------------------------------------|------------------------------------------------|
| **Context Switch** | OS forces switch when higher-priority task is ready | Task runs until it voluntarily yields / blocks |
| **Responsiveness** | High (guaranteed latency for high-priority tasks)   | Low (a task can hog the CPU indefinitely)      |
| **RTOS Examples**  | FreeRTOS (default), VxWorks, QNX                    | Cooperative schedulers, bare-metal super loops |

---

## Q7.18: When is a Thread Executed?

A thread transitions from **Ready** to **Running** when:
1. **Higher-priority thread blocks** (sleep, mutex wait, I/O).
2. **Time slice expires** (round-robin scheduling).
3. **Preemption** by a higher-priority thread becoming ready.

---

## Q7.19: Thread Communication Methods

1. **Shared Variables** (with mutex protection)
2. **Condition Variables** (`pthread_cond_wait/signal`)
3. **Message Queues** (RTOS)
4. **Semaphores**
5. **Pipes/Sockets** (Linux)

---

## Q7.20: Thread Synchronization with Mutex (Code)

```c
#include <stdio.h>
#include <pthread.h>

int shared_counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        shared_counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Counter: %d\n", shared_counter); // Always 200000 with mutex
    return 0;
}
```

---
---

# 8. Operating Systems, Linux & System Programming

---

## Q8.1: Process vs Thread Comparison

| Feature                | Process                                         | Thread                                     |
|------------------------|-------------------------------------------------|--------------------------------------------|
| **Address Space**      | Private/Isolated (Virtual memory space)         | Shared (Shares parent process's space)     |
| **Creation Cost**      | High (Requires copying descriptor tables)       | Low (Shares resources)                     |
| **Switching Overhead** | High (Requires swapping MMU Page Tables)        | Low (No Page Table swap, only SP/PC)       |
| **IPC**                | Requires OS mechanisms (Shared Memory, Sockets) | Direct access to global variables          |
| **Crash Safety**       | Crash in one process doesn't affect others      | Crash in one thread crashes entire process |

---

## Q8.2: Process Creation via `fork()`

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        printf("Fork Failed\n");
    } else if (pid == 0) {
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    } else {
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
    }
    return 0;
}
```

---

## Q8.3: Multithreading with `pthread`

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
    
    pthread_create(&thread1, NULL, print_even, NULL);
    pthread_create(&thread2, NULL, print_odd, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Threads Finished Execution.\n");
    return 0;
}
```

### Thread Cancellation and Detaching
- **`pthread_cancel`:** Sends a cancellation request to a thread.
- **`pthread_detach`:** Marks a thread as detached. Resources are automatically released.
  - **Trap:** If you call `pthread_join` on a detached thread, it will return `EINVAL`.

---

## Q8.4: Thread Cancellation and Detaching (Detailed Code)

### `pthread_cancel` Code Example
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cleanup_handler(void *arg) {
    printf("Cleanup: Releasing resource '%s'\n", (char*)arg);
}

void* worker(void *arg) {
    pthread_cleanup_push(cleanup_handler, "mutex_lock");
    
    printf("Worker: Running...\n");
    while (1) {
        sleep(1); // Cancellation point!
        printf("Worker: Still running...\n");
    }
    
    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);
    
    sleep(3);
    printf("Main: Cancelling worker thread...\n");
    pthread_cancel(tid);
    
    pthread_join(tid, NULL);
    printf("Main: Worker thread joined.\n");
    return 0;
}
```

---

## Q8.5: `fork()` vs `vfork()`

- **`fork()`:** Creates a child process with a separate address space. Uses Copy-on-Write (COW).
- **`vfork()`:** Creates a child without copying the address space. Parent is suspended until child calls `exec()` or `_exit()`.
  - **Trap:** Modifying variables in the child directly modifies them in the parent.
- **`clone()`:** General-purpose system call; `fork()` and `vfork()` are implemented on top of it.

---

## Q8.6: Orphan vs Zombie Processes

- **Orphan Process:** Parent terminates before its child. Adopted by PID 1 (`init`/`systemd`).
- **Zombie Process:** Child terminates but parent hasn't read its exit status via `wait()`.
  - **Reaping Zombies:** Parent traps `SIGCHLD` and calls `waitpid(-1, NULL, WNOHANG)`.

---

## Q8.7: Inter-Process Communication (IPC) Types

| IPC Type              | Max Data Capacity                    | Description                                                       |
|-----------------------|--------------------------------------|-------------------------------------------------------------------|
| **Pipe (Anonymous)**  | 65,536 bytes (Linux default)         | Half-duplex communication between related processes.              |
| **FIFO (Named Pipe)** | 65,536 bytes (Linux default)         | Half-duplex, appears as a file, communicates unrelated processes. |
| **Message Queue**     | Regulated by sysctl limit            | Structured messaging with priority control.                       |
| **Shared Memory**     | System RAM limit (GBs)               | Fastest IPC. Maps memory block directly. Needs mutexes.           |
| **Socket (Unix/TCP)** | Network buffer limited               | Communicates processes across network or locally.                 |
| **Signals**           | 1 bit (Just interrupt signal number) | Asynchronous notifications. Cannot send raw data payload safely.  |

---

## Q8.8: System Calls vs API

- **System Call:** An entry path into kernel space to request hardware resources (e.g., `sys_write`, `sys_fork`).
- **API:** A wrapper library function (e.g., `printf` or `malloc`).
  - **Relationship:** `printf()` (API) internally calls `write()` (System Call).

---

## Q8.9: Virtual Memory, MMU, Page Table, Page Fault

### Virtual Memory
Maps non-contiguous physical RAM to a contiguous virtual space, isolating processes.

### Memory Management Unit (MMU)
Hardware unit that translates virtual addresses to physical addresses using Page Tables.
- **Stack/Heap Protection:** MMU marks pages with permission flags (R/W/X).

### Page Table and Page Fault
- **Page Table:** Maps virtual page numbers to physical frames.
- **Page Fault:** Triggered when accessing a virtual page not in physical RAM.

### Translation Lookaside Buffer (TLB)
Hardware cache inside the MMU that stores recent virtual-to-physical address translations.

---

## Q8.10: Cache Memory and Coherency

- **Cache Hit:** Data is present in the cache.
- **Cache Miss:** Data is missing, forcing fetch from RAM.

### Cache Coherency Issues with DMA
- **Write-back problem:** CPU writes to cache, DMA reads old data from RAM.
- **Read-invalidate problem:** DMA writes new data to RAM, CPU reads stale cache.

### Solution: Cache Invalidation & Cleaning
- **Cache Invalidation:** Clears valid bit, forcing CPU to fetch from RAM.
- **Cache Clean (Flush):** Forces cache lines to write back to RAM.

---

## Q8.11: Linux Device Drivers

A device driver is a kernel module that abstracts physical hardware, presenting standard file interfaces to user-space.

### Module Insertion Commands
- `insmod <module.ko>`: Directly inserts specified module.
- `modprobe <module>`: Inserts module while resolving dependencies.

### User to Kernel Data Copies
- **`copy_to_user(dest, src, size)`:** Copies kernel buffer to user space.
- **`copy_from_user(dest, src, size)`:** Copies user space buffer to kernel memory.

---

## Q8.12: Device Tree Source (DTS)

A Device Tree describes the physical hardware layout. During boot, the kernel parses the Device Tree blob (`.dtb`) and matches device drivers with hardware nodes via the `compatible` property string.

```
  [ Device Tree Node ]              [ Device Driver ]
  compatible = "st,stm32-uart"      of_match_table = "st,stm32-uart"
           │                                ▲
           └──────────── Match ─────────────┘
                         │
                         ▼
                  Calls .probe()
```

---

## Q8.13: IOCTL (I/O Control) — How It Works

`ioctl` is a system call for sending device-specific commands to a kernel driver.

### Kernel Driver IOCTL Implementation
```c
#include <linux/ioctl.h>

#define MY_MAGIC 'M'
#define IOCTL_SET_VALUE _IOW(MY_MAGIC, 1, int)
#define IOCTL_GET_VALUE _IOR(MY_MAGIC, 2, int)

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
    static int value = 0;
    
    switch (cmd) {
        case IOCTL_SET_VALUE:
            if (copy_from_user(&value, (int __user *)arg, sizeof(int)))
                return -EFAULT;
            break;
        case IOCTL_GET_VALUE:
            if (copy_to_user((int __user *)arg, &value, sizeof(int)))
                return -EFAULT;
            break;
        default:
            return -EINVAL;
    }
    return 0;
}
```

---

## Q8.14: Netlink — Kernel-User Communication

Netlink is a Linux IPC mechanism for bidirectional, asynchronous, multicast communication between kernel and user-space.

| Feature           | IOCTL                            | Netlink                                                    |
|-------------------|----------------------------------|------------------------------------------------------------|
| **Direction**     | User → Kernel (request/response) | Bidirectional + multicast                                  |
| **Async Support** | ❌ Synchronous only               | ✅ Asynchronous messages                                    |
| **Use Cases**     | Device control commands          | Network configuration (iproute2), audit, generic messaging |

---

## Q8.15: `cdev` Struct — Character Device

```c
#include <linux/cdev.h>

struct cdev my_cdev;

static int __init my_init(void) {
    dev_t dev_num;
    alloc_chrdev_region(&dev_num, 0, 1, "my_char_dev");
    
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    cdev_add(&my_cdev, dev_num, 1);
    
    return 0;
}
```

---

## Q8.16: Major and Minor Numbers

- **Major Number:** Identifies the **driver**.
- **Minor Number:** Identifies a **specific device instance** managed by that driver.

---

## Q8.17: How `read`/`write` System Calls Flow from User to Kernel

```
User Space:                        Kernel Space:
+──────────────+                   +──────────────────────+
| write(fd,    |                   |                      |
|  buf, count) | ─── syscall ────► | sys_write()          |
+──────────────+                   |   │                  |
                                   |   ▼                  |
                                   | VFS Layer            |
                                   |   ▼                  |
                                   | driver->write()      |
                                   |   ▼                  |
                                   | copy_from_user()     |
                                   |   ▼                  |
                                   | Hardware Register    |
                                   +──────────────────────+
```

---

## Q8.18: `/proc` vs `/dev` Filesystem

| Feature        | `/proc`                                 | `/dev`                                   |
|----------------|-----------------------------------------|------------------------------------------|
| **Purpose**    | Virtual filesystem exposing kernel info | Device node files for accessing hardware |
| **Content**    | Process info, kernel stats              | Character/block device nodes             |
| **Created By** | Kernel automatically                    | `mknod` or `udev` daemon                 |

---

## Q8.19: Finding OS and Binary Architecture

```bash
uname -a          # Full system info
cat /etc/os-release  # Distribution details
file ./my_program    # Determine if binary is 32-bit or 64-bit
```

---

## Q8.20: Paging Mechanism (Detailed)

```
Virtual Address:
+──────────────────┬────────────────+
| Page Number (20) | Offset (12)    |  (For 4 KB pages on 32-bit system)
+──────────────────┴────────────────+
          │
          ▼ (Page Table Lookup)
     ┌──────────┐
     │ Page     │ → Frame Number (Physical)
     │ Table    │ → Permission Bits (R/W/X)
     │ Entry    │ → Present/Valid Bit
     └──────────┘
```

### Page Replacement Algorithms
- **FIFO:** Evict the oldest loaded page.
- **LRU (Least Recently Used):** Evict the page that hasn't been accessed for the longest time.
- **Optimal:** Evict the page that won't be used for the longest time (theoretical best).

---

## Q8.21: Process Life Cycle (States)

```
                ┌────────────────────────┐
                │                        │
                ▼                        │
  [ New ] ──► [ Ready ] ──► [ Running ] ─┤
                ▲               │        │
                │               ▼        │
                └─── [ Waiting/Blocked ] │
                                         │
                                         ▼
                                   [ Terminated ]
```

---

## Q8.22: User Space vs Kernel Space

```
  Virtual Address Space (32-bit system):
  +-----------------------+ 0xFFFFFFFF
  |    Kernel Space       |  (Upper 1 GB) - Ring 0 (Full HW access)
  +-----------------------+ 0xC0000000
  |    User Space         |  (Lower 3 GB) - Ring 3 (Restricted access)
  +-----------------------+ 0x00000000
```

User space cannot directly access hardware registers or kernel memory. It must use **system calls**.

---

## Q8.23: SMMU / IOMMU

An SMMU (ARM) / IOMMU (Intel) is an MMU for I/O devices. It translates DMA addresses from I/O Virtual Addresses to Physical Addresses, providing memory isolation and virtual addressing for DMA.

---

## Q8.24: Full Board Boot Sequence

```
1. Power On / Reset
2. ROM Bootloader (BootROM / BIOS / UEFI) - POST, init clocks/DRAM, load SPL
3. Secondary Program Loader (SPL / MLO) - Init DRAM, load U-Boot
4. U-Boot (Bootloader) - Init peripherals, load kernel + DTB
5. Linux Kernel - Parse DTB, init drivers, mount rootfs, start PID 1
6. User Space (init / systemd) - Start services and daemons
```

### How to Reduce Boot Time
1. **Kernel:** Remove unused drivers, use `initramfs`.
2. **U-Boot:** `bootdelay=0`, use Falcon mode.
3. **Userspace:** Optimize `systemd` service dependencies, use `systemd-analyze blame`.

---

## Q8.25: How to Write a Makefile

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g -O2
LDFLAGS = -lpthread

SRC = main.c utils.c driver.c
OBJ = $(SRC:.c=.o)
TARGET = my_program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
```

---

## Q8.26: Driver Registration with Kernel + User Space Code

### Kernel Module (Full Registration)
```c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "my_driver"
static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static char kernel_buf[256] = "Hello from kernel!\n";

static int my_open(struct inode *i, struct file *f) { return 0; }
static int my_release(struct inode *i, struct file *f) { return 0; }

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
    size_t to_copy = min(len, (size_t)(strlen(kernel_buf) - *off));
    if (to_copy <= 0) return 0;
    if (copy_to_user(buf, kernel_buf + *off, to_copy)) return -EFAULT;
    *off += to_copy;
    return to_copy;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .release = my_release,
};

static int __init my_init(void) {
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_num, 1);
    my_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);
    return 0;
}

static void __exit my_exit(void) {
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
```

---

## Q8.27: Common System Calls and Header Files

| System Call          | Header File      | Description                        |
|----------------------|------------------|------------------------------------|
| `fork()`             | `<unistd.h>`     | Create child process               |
| `exec()`             | `<unistd.h>`     | Replace process image              |
| `wait()`             | `<sys/wait.h>`   | Wait for child process             |
| `open()`             | `<fcntl.h>`      | Open a file                        |
| `read()` / `write()` | `<unistd.h>`     | Read/Write from/to file descriptor |
| `close()`            | `<unistd.h>`     | Close file descriptor              |
| `socket()`           | `<sys/socket.h>` | Create a network socket            |
| `ioctl()`            | `<sys/ioctl.h>`  | Device control                     |
| `mmap()`             | `<sys/mman.h>`   | Memory mapping                     |
| `pthread_create()`   | `<pthread.h>`    | Create a thread                    |
| `pipe()`             | `<unistd.h>`     | Create an anonymous pipe           |
| `kill()`             | `<signal.h>`     | Send signal to a process           |
| `brk()` / `sbrk()`   | `<unistd.h>`     | Adjust heap boundary               |

---

## Q8.28: `fs.h` Contents

`<linux/fs.h>` defines the **Virtual File System (VFS)** interfaces:
- `struct file_operations` — function pointers for `open`, `read`, `write`, `ioctl`, `release`.
- `struct inode` — represents a file on disk.
- `struct file` — represents an open file in a process.
- `register_chrdev()` / `unregister_chrdev()` functions.

---

## Q8.29: Kernel Architecture Overview

```
  +──────────────────────────────────────────────────────+
  |                    User Space                        |
  |   [ App 1 ]  [ App 2 ]  [ Shell ]  [ Daemon ]      |
  +──────────────────────────────────────────────────────+
            │          System Call Interface (SCI)
  +──────────────────────────────────────────────────────+
  |                   Kernel Space                       |
  |  Process Scheduler | Memory Manager | File System    |
  |  Network Stack     | Device Drivers | IPC            |
  +──────────────────────────────────────────────────────+
            │          Hardware Abstraction Layer
  +──────────────────────────────────────────────────────+
  |           Hardware (CPU, Memory, I/O Devices)        |
  +──────────────────────────────────────────────────────+
```

---
---

# 9. Networking & Sockets

---

## Q9.1: The OSI Model and Its 7 Layers

```
       OSI Reference Model
  +───────────────────────────+
  |  7. Application Layer     |  <-- HTTP, FTP, DNS
  +───────────────────────────+
  |  6. Presentation Layer    |  <-- SSL/TLS, ASCII, JPEG
  +───────────────────────────+
  |  5. Session Layer         |  <-- NetBIOS, RPC
  +───────────────────────────+
  |  4. Transport Layer       |  <-- TCP, UDP
  +───────────────────────────+
  |  3. Network Layer         |  <-- IP, ICMP
  +───────────────────────────+
  |  2. Data Link Layer       |  <-- Ethernet, PPP
  +───────────────────────────+
  |  1. Physical Layer        |  <-- RS232, RJ45
  +───────────────────────────+
```

---

## Q9.2: IP Address vs MAC Address

| Feature    | IP Address (Internet Protocol)            | MAC Address (Media Access Control)          |
|------------|-------------------------------------------|---------------------------------------------|
| **Layer**  | Network Layer (Layer 3)                   | Data Link Layer (Layer 2)                   |
| **Type**   | Logical Address (Assigned by DHCP/Router) | Physical Address (Burned into NIC hardware) |
| **Format** | 32-bit (IPv4) or 128-bit (IPv6)           | 48-bit hex (e.g., `00:0a:95:9d:68:16`)      |
| **Scope**  | Global (Used for routing across networks) | Local (Used within local subnet)            |

---

## Q9.3: TCP vs UDP

| Feature          | TCP (Transmission Control Protocol)            | UDP (User Datagram Protocol)                      |
|------------------|------------------------------------------------|---------------------------------------------------|
| **Connection**   | Connection-oriented (Handshake required)       | Connectionless (Just fire-and-forget)             |
| **Reliability**  | Guaranteed delivery (Retransmits lost packets) | Not guaranteed (Packets can be lost/out-of-order) |
| **Speed**        | Slower (overhead of acknowledgments)           | Faster (minimal header size)                      |
| **Flow Control** | Yes (sliding window)                           | No                                                |
| **Header Size**  | Minimum 20 bytes                               | Fixed 8 bytes                                     |

---

## Q9.4: TCP Handshake and Connection Flow

### 3-Way Handshake (Establish Connection)
```
  Client                                 Server
    │                                      │
    │─────── SYN (Seq=X) ─────────────────►│
    │◄────── SYN-ACK (Seq=Y, Ack=X+1) ─────│
    │─────── ACK (Ack=Y+1) ───────────────►│ (Established)
```

### 4-Way Termination (Close Connection)
```
  Client                                 Server
    │─────── FIN ─────────────────────────►│
    │◄────── ACK ──────────────────────────│
    │◄────── FIN ──────────────────────────│
    │─────── ACK ─────────────────────────►│
```

---

## Q9.5: Socket Programming (TCP Client-Server C Code)

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

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

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

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, hello, strlen(hello), 0);
    read(sock, buffer, 1024);
    printf("Server responded: %s\n", buffer);

    close(sock);
    return 0;
}
```

---

## Q9.6: Encryption and Security Libraries

- **Symmetric Encryption:** Same key for encryption and decryption. Very fast. (e.g., **AES**, DES).
- **Asymmetric Encryption:** Public key for encryption, private key for decryption. (e.g., **RSA**, ECC).
- **Hashing:** One-way cryptographic hash functions (e.g., **SHA-256**).
- **Embedded Security Libraries:**
  - **mbedTLS:** Light footprint for resource-constrained MCUs.
  - **wolfSSL:** Extremely fast SSL/TLS library.

---

## Q9.7: TLS (Transport Layer Security) Handshake

```
  Client                                 Server
    │─────── ClientHello ────────────────►│
    │◄────── ServerHello & Certificate ───│
    │◄────── ServerKeyExchange ──────────│
    │─────── ClientKeyExchange ─────────►│ (Pre-master secret encrypted)
    │─────── ChangeCipherSpec ──────────►│ (Switch to symmetric encryption)
    │◄────── Finished ───────────────────│
```

---

## Q9.8: UDP Send/Receive Flow

```
  Client                                 Server
    │                              bind() + recvfrom() (blocking)
    │─────── sendto(data) ────────────────►│ (Receive data)
    │◄────── sendto(response) ─────────────│ (Optional response)
  No connection setup, no teardown. Fire-and-forget.
```

---

## Q9.9: Types of Sockets

| Socket Type         | Macro         | Protocol | Connection          | Data Boundary        |
|---------------------|---------------|----------|---------------------|----------------------|
| **Stream Socket**   | `SOCK_STREAM` | TCP      | Connection-oriented | No (byte stream)     |
| **Datagram Socket** | `SOCK_DGRAM`  | UDP      | Connectionless      | Yes (per-datagram)   |
| **Raw Socket**      | `SOCK_RAW`    | IP/ICMP  | Connectionless      | Yes (raw IP packets) |

`SOCK_RAW` requires root/`CAP_NET_RAW` privilege.


## Q9.11: Socket Functions Summary (TCP vs UDP)

| Function                  | TCP Server | TCP Client | UDP Server | UDP Client |
|---------------------------|------------|------------|------------|------------|
| `socket()`                | ✅          | ✅          | ✅          | ✅          |
| `bind()`                  | ✅          | Optional   | ✅          | Optional   |
| `listen()`                | ✅          | ❌          | ❌          | ❌          |
| `accept()`                | ✅          | ❌          | ❌          | ❌          |
| `connect()`               | ❌          | ✅          | ❌          | Optional   |
| `send()` / `recv()`       | ✅          | ✅          | ❌          | ❌          |
| `sendto()` / `recvfrom()` | ❌          | ❌          | ✅          | ✅          |
| `close()`                 | ✅          | ✅          | ✅          | ✅          |

---
---

# 10. C++ Concepts

---

## Q10.1: Virtual Destructor

If a derived class object is deleted using a base class pointer, and the base class destructor is **not virtual**, the base class destructor executes but the derived class destructor does not. This causes resource leaks.

---

## Q10.2: Constructor/Destructor Call Order

- **Constructors:** Called top-down: `Base` -> `Derived` -> `Grandchild`.
- **Destructors:** Called bottom-up: `Grandchild` -> `Derived` -> `Base`.

### Code Example
```cpp
#include <iostream>

class Base {
public:
    Base()  { std::cout << "Base Constructor\n"; }
    virtual ~Base() { std::cout << "Base Destructor\n"; }
};

class Derived : public Base {
public:
    Derived()  { std::cout << "Derived Constructor\n"; }
    ~Derived() { std::cout << "Derived Destructor\n"; }
};

class Grandchild : public Derived {
public:
    Grandchild()  { std::cout << "Grandchild Constructor\n"; }
    ~Grandchild() { std::cout << "Grandchild Destructor\n"; }
};

int main() {
    std::cout << "--- Creating Grandchild object ---\n";
    Grandchild g;
    std::cout << "--- Destroying Grandchild object ---\n";
    return 0;
}
```

### Output
```
--- Creating Grandchild object ---
Base Constructor
Derived Constructor
Grandchild Constructor
--- Destroying Grandchild object ---
Grandchild Destructor
Derived Destructor
Base Destructor
```
