
# Interview Material

This directory contains resources for C/C++ interview preparation, including notes, coding questions, and data structure implementations.

## Files Overview

### 1. `_notes.md`
Description: A comprehensive revision guide for C programming.
Contents:
- Basics: Data types, format specifiers, operators.
- Memory Management: `malloc`, `calloc`, `free`, memory layout (Stack vs Heap).
- Pointers: Arithmetic, function pointers, pointer vs reference.
- Bit Manipulation: Operators, macros, common algorithms (checking power of 2, counting set bits).
- Advanced Topics: Threading (C++), synchronization, endianness.

### 2. `_questions.cpp`
Description: A collection of common C++ interview coding questions.
Contents:
- Number Theory: Prime check, Fibonacci series, Palindrome, ARMSTRONG number.
- String Manipulation: Anagram check.
- Pattern Printing: Various star and number patterns.
- Basics: Factorial, GCD/LCM, Decimal to Binary conversion.
*Note: Most implementations are commented out for easy selective compilation/testing.*

### 3. `ds_implementations.c`
Description: Implementation of fundamental data structures in C.
Contents:
- Singly Linked List: Basic operations (Insert Head/Tail, Delete by Value).
- Stack: Array-based implementation (Fixed size, LIFO).
- Queue: Circular Array-based implementation (FIFO).
- Deque: Double-ended Queue using Circular Array.
- Binary Search Tree (BST): Insert, Search, Inorder Traversal.

## Usage
- To Compile `ds_implementations.c`:
  ```bash
  gcc ds_implementations.c -o ds_implementations
  ./ds_implementations
  ```
- To Practice Questions:  
  Uncomment the relevant section in `_questions.cpp` and compile using a C++ compiler (e.g., `g++`).
