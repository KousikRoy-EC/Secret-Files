# Part 2: C Language Concepts

---

## Q1: What is `volatile`? What is the use of `volatile`? Any practical applications?

### Answer
The `volatile` keyword in C is a qualifier that tells the compiler that a variable's value can be changed by something outside the control of the program's code itself at any time. Consequently, the compiler **must not optimize** any reads or writes to this variable. It forces the compiler to read the value directly from the memory address every time it is referenced, rather than caching it in a CPU register.

### compiler Optimization without volatile
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

## Q2: Use of the `static` keyword?
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

```
+------------------------+      +------------------------+
| file1.c                |  | file2.c         |
|------------------------|--|-----------------|
| static void helper() { |  | void helper() { |
| // Private             |  | // Public       |
| }                      |  | }               |
+------------------------+      +------------------------+
              \                            /
               No conflict during linking phase!
```

### Questions:
#### Q2a: What happens during compilation if I use a static function?
During compilation, the compiler generates the code for the static function normally but limits its symbol visibility to local scope inside the object file (`.o`). The linker will not resolve any references to this function symbol from other object files, preventing linking conflicts.

#### Q2b: Static variable defined in a header file, and this header file included in 2 different files, what will happen?
If you define a static variable in a header file (e.g., `static int x = 10;`) and include it in `file1.c` and `file2.c`, **each C file gets its own independent copy of the variable**. Modifying `x` in `file1.c` will not affect `x` in `file2.c`. This is usually a bug if you intended to share the variable.
- **Trap:** It increases memory usage since multiple copies exist. To share, declare it as `extern int x;` in the header and define it as `int x = 10;` in one C file.

#### Q2c: How to indirectly modify a static variable from another file?
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

## Q3: Use of `extern` keyword? (Internal vs External Linkage)

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

## Q4: Storage Classes in C

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

## Q5: Steps of Compiling a C Program

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

## Q6: Weak Functions (`__attribute__((weak))`)

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

## Q7: Macro Function vs Inline Function

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

## Q8: Structure, Union, and Enum differences

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

## Q9: Storage of static members inside structures

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

## Q10: Void Pointer (`void*`)

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

## Q11: Function Pointer with Code Example

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

## Q12: Callback Function

### Answer
A callback function is a function that is passed as an argument to another function, to be called when a certain event occurs or a task completes.

### Why use Callback instead of Normal Function?
Callbacks enable **loose coupling** and **modularity**. For example, a generic timer driver doesn't need to know what specific action (blink LED, send packet) needs to run on timer expiry. It simply accepts a function pointer (callback) from the application layer and executes it when the timer fires.

```
+------------------+                   +--------------------+
| Application Layer |                                     | Hardware Driver |
|-------------------|-------------------------------------|-----------------|
| void blink()      | ──Register Callback──>              |                 |
|                   |                                     | On Interrupt:   |
|                   | <───Execute Callback───  callback() |                 |
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

## Q13: Function Declaration vs Definition (e.g. `printf`, `scanf`)

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

## Q14: Signed vs Unsigned Integers

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

## Q15: Typecasting vs Type Conversion

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

## Q16: `typedef`

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

## Q17: Static vs Shared Libraries

### Answer
- **Static Library (`.a` / `.lib`):** The code of the library is physically copied and linked into the application executable during build time.
  - **Pros:** Fast execution, independent executable.
  - **Cons:** Larger executable size, requires rebuilding application if library updates.
- **Shared / Dynamic Library (`.so` / `.dll`):** The library code is loaded into memory only when the application starts or runs. The executable contains only references.
  - **Pros:** Smaller executable size, multiple apps can share a single copy in memory.
  - **Cons:** Startup overhead, runtime dependency ("DLL hell").

---

## Q18: Implement custom `sizeof` using Macro

### Answer
```c
#define sizeof_custom(type) ((size_t)((type*)0 + 1))
// Or for variables:
#define var_sizeof(var) ((char *)(&var + 1) - (char *)&var)
```

### Explanation of `my_sizeof`
1. `(type*)0` casts the address `0` to a pointer of the target type.
2. `((type*)0 + 1)` increments the pointer by `sizeof(type)` bytes, effectively pointing to the memory location immediately after the first element.
3. `&` gets the address of that element. Since the array started at `0`, the address of the element at index 1 is exactly equal to `sizeof(type)`.

---

## Q19: Pointers to Strings & Storage Location

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
