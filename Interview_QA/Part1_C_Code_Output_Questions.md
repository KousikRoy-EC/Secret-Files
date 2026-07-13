# Part 1: C Code Output & Behavior Questions

---

## Q1: Pointer Arithmetic with Post-Increment

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

## Q2: Undefined Behavior with `i++ + ++i`

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

## Q3: do-while with Post/Pre Increment

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

## Q4: Returning Address of Local Variable (Dangling Pointer)

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

## Q5: Double Pointer Dereference

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

## Q6: Pointer Assignment and Arithmetic

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

## Q7: Negative Array Indexing

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

## Q8: Static Variable in Function with Multiple Calls

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

## Q9: The `a+++a` Puzzle (Maximal Munch Rule)

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

**Common compiler behavior:**
1. `a++` evaluates to `3` (post-increment, `a` becomes 4 after)
2. `+ a`: At this point `a` may be `3` or `4` depending on the compiler

**Most common output (GCC):**
- `a++` gives 3, then `a` becomes 4
- `b = 3 + 4 = 7` (using new value) 

**With GCC (typical):** `b = a++ + a` = `3 + 4 = 7`, then `a = 4`

### Output (GCC typical)
```
4 7
```

> **Interview Trap:** Know the maximal munch rule. `a+++a` → `(a++) + a`, NOT `a + (++a)`. In C11 and later, this is technically defined as `a` is modified once and read once with a sequenced-before relationship in some interpretations, but it's best to avoid such expressions.

---

## Q10: `sizeof('A')` in C

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

## Q11: Modifying a String Literal

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

## Q12: `sizeof(1.5)`

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

## Q13: Post-Increment in printf

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

## Q14: Assignment in `if` Condition (= vs ==)

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

## Q15: Casting `char` Array to `int*`

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
- Integer = `0xDDCCBBAA`

```
Reading 4 bytes from arr[0]:
Byte 0 (0xAA) → bits [7:0]    (LSB)
Byte 1 (0xBB) → bits [15:8]
Byte 2 (0xCC) → bits [23:16]
Byte 3 (0xDD) → bits [31:24]  (MSB)
Result: 0xDDCCBBAA
```

**Big-Endian (PowerPC, SPARC):**
- Byte at lowest address = most significant byte
- Integer = `0xAABBCCDD`

```
Byte 0 (0xAA) → bits [31:24]  (MSB)
Byte 1 (0xBB) → bits [23:16]
Byte 2 (0xCC) → bits [15:8]
Byte 3 (0xDD) → bits [7:0]    (LSB)
Result: 0xAABBCCDD
```

### Output
| System        | Output     |
|---------------|------------|
| Little-Endian | `ddccbbaa` |
| Big-Endian    | `aabbccdd` |

---

## Q16: How `char arr[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE}` is Stored

### Key Insight: Byte Arrays are NOT Affected by Endianness

Endianness only affects how **multi-byte values** (int, short, long) are stored. A char array is a sequence of individual bytes, and they are **always stored in the same order** regardless of endianness.

```
Both Little-Endian and Big-Endian:
Address:  0x100   0x101   0x102   0x103   0x104
Value:    0xAA    0xBB    0xCC    0xDD    0xEE
```

The bytes are stored in **declaration order** on both systems.

> **Interview Trap:** Many candidates confuse this. Endianness only matters when you interpret multiple bytes as a single larger type (like casting `char[]` to `int*` as in Q15). The raw byte order in memory is identical.

---

## Q17: Array Pointer Operations (`a++` is Illegal)

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

## Q18: `*p++` vs `++*p`

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

## Q19: What do `printf` and `scanf` Return?

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
