# Part 4: Data Structures and Algorithms

## Q1: Reverse a String in C

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

## Q2: String Operations Implemented from Scratch

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

## Q3: Implement `strcpy` in a Single Line

### Answer
```c
char* my_strcpy_oneline(char *dest, const char *src) {
    while ((*dest++ = *src++));
    return dest; // Note: returns pointer to the end of dest + 1 in some variations, 
                 // but this typical syntax returns initial dest if reference saved beforehand.
}
```

---

## Q4: Substring Copy Function and Test Cases

Function: copies string in `source` to `destination` from `start_index` to `end_index` (inclusive).

```c
#include <stdio.h>
#include <string.h>

void copy_substring(const char *source, char *dest, int start, int end) {
    if (!source || !dest) {
        if (dest) dest[0] = '\0';
        return;
    }
    int len = strlen(source);
    
    // Bounds checking
    if (start < 0 || start >= len || end < start) {
        dest[0] = '\0';
        return;
    }
    
    // Adjust end if it exceeds string length
    if (end >= len) {
        end = len - 1;
    }
    
    int i, j = 0;
    for (i = start; i <= end; i++) {
        dest[j++] = source[i];
    }
    dest[j] = '\0'; // Null terminator
}
```

### Test Cases Matrix
1. **Normal Case:** `source = "Embedded"`, `start = 2`, `end = 5` -> `dest = "bedd"`.
2. **Invalid Indices:** `start = -1` or `start > end`.
3. **End index out of bounds:** `end` larger than string size.
4. **Empty strings / NULL pointers:** Check robustness.

---

## Q5: Most Frequent Character in "Hello world"

If there is a tie, return the character that appeared first in the string.

```c
#include <stdio.h>

char most_frequent_char(const char *str) {
    if (!str || *str == '\0') return '\0';
    
    int freq[256] = {0};
    
    // Step 1: Count occurrences
    for (int i = 0; str[i] != '\0'; i++) {
        freq[(unsigned char)str[i]]++;
    }
    
    // Step 2: Scan string again to find the first character with max count
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

## Q6: Count Occurrences of Substring "to"

In: `"Welcome to gdb Welcome to gdb Welcome to gdb"`

```c
#include <stdio.h>
#include <string.h>

int count_substring(const char *str, const char *sub) {
    int count = 0;
    int sub_len = strlen(sub);
    const char *temp = str;
    
    while ((temp = strstr(temp, sub)) != NULL) {
        count++;
        temp += sub_len; // Move past the matched substring
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

## Q7: Singly Linked List Implementation

Complete CRUD operations. Features deleting a node with value `50`.

```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

// Insert at beginning
void insert_at_head(struct Node **head, int val) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = val;
    new_node->next = *head;
    *head = new_node;
}

// Insert at end
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

// Delete by value (e.g. 50)
void delete_by_value(struct Node **head, int val) {
    if (*head == NULL) return;
    struct Node *temp = *head;
    struct Node *prev = NULL;
    
    // If head node itself holds the value
    if (temp != NULL && temp->data == val) {
        *head = temp->next;
        free(temp);
        return;
    }
    
    // Search for the value
    while (temp != NULL && temp->data != val) {
        prev = temp;
        temp = temp->next;
    }
    
    // Value not found
    if (temp == NULL) return;
    
    // Unlink node and free memory
    prev->next = temp->next;
    free(temp);
}

// Print List
void print_list(struct Node *head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}
```

---

## Q8: Doubly Linked List Implementation

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

## Q9: Reverse a Linked List

### 1. Iterative Reverse (O(N) Time, O(1) Space)

```
Initial State:
[Head] -> [A] -> [B] -> [C] -> NULL

Pointers definition:
prev = NULL, curr = head, next = NULL

Loop iteration:
1. Save next:      next = curr->next
2. Reverse link:   curr->next = prev
3. Shift prev:     prev = curr
4. Shift curr:     curr = next
```

```c
struct Node* reverse_iterative(struct Node *head) {
    struct Node *prev = NULL;
    struct Node *curr = head;
    struct Node *next = NULL;
    
    while (curr != NULL) {
        next = curr->next; // Store next node
        curr->next = prev; // Reverse the link
        prev = curr;       // Move prev forward
        curr = next;       // Move curr forward
    }
    return prev; // New head of reversed list
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

## Q10: Print Linked List in Reverse using Recursion

```c
void print_reverse(struct Node *head) {
    if (head == NULL) return;
    print_reverse(head->next);
    printf("%d ", head->data);
}
```

---

## Q11: Find Middle Node of Linked List (Slow/Fast Pointer)

```c
struct Node* find_middle(struct Node *head) {
    if (head == NULL) return NULL;
    struct Node *slow = head;
    struct Node *fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;       // Moves 1 step
        fast = fast->next->next; // Moves 2 steps
    }
    return slow; // Middle node
}
```

---

## Q12: Detect Loop in Linked List (Floyd's Cycle-Finding)

```
        +--------------+
        v              |
[A] -> [B] -> [C] -> [D]  (Cycle between D and B)
```

```c
#include <stdbool.h>

bool detect_loop(struct Node *head) {
    struct Node *slow = head;
    struct Node *fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true; // Loop detected
        }
    }
    return false; // No loop
}
```

---

## Q13: Find Nth Node from End

```c
struct Node* find_nth_from_end(struct Node *head, int n) {
    struct Node *main_ptr = head;
    struct Node *ref_ptr = head;
    
    // Move ref_ptr n steps ahead
    for (int i = 0; i < n; i++) {
        if (ref_ptr == NULL) return NULL; // List size is less than n
        ref_ptr = ref_ptr->next;
    }
    
    // Move both pointers until ref_ptr hits NULL
    while (ref_ptr != NULL) {
        main_ptr = main_ptr->next;
        ref_ptr = ref_ptr->next;
    }
    return main_ptr;
}
```

---

## Q14: Replace Nth Node Value

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

## Q15: Detect Intersection of Two Linked Lists

```c
struct Node* get_intersection_node(struct Node *headA, struct Node *headB) {
    if (!headA || !headB) return NULL;
    struct Node *ptrA = headA;
    struct Node *ptrB = headB;
    
    while (ptrA != ptrB) {
        ptrA = (ptrA == NULL) ? headB : ptrA->next;
        ptrB = (ptrB == NULL) ? headA : ptrB->next;
    }
    return ptrA; // returns intersection node or NULL
}
```

---

## Q16: Use Cases of Circular Linked List in Embedded Systems

1. **Task Scheduler (Round Robin):** In operating systems (RTOS), active tasks are kept in a circular queue. The scheduler cycles through the list indefinitely giving CPU time to each task.
2. **Circular Buffers / Ring Buffers:** Used for serial interface queueing (UART/DMA packet buffers).

---

## Q17: Stack Implementation using Structures

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

## Q18: Sorting Techniques

### Comparison Table

| Algorithm          | Best Time       | Average Time    | Worst Time      | Space Complexity | Stable? | When to Use                     |
|--------------------|-----------------|-----------------|-----------------|------------------|---------|---------------------------------|
| **Bubble Sort**    | \(O(N)\)        | \(O(N^2)\)      | \(O(N^2)\)      | \(O(1)\)         | Yes     | Almost-sorted small arrays      |
| **Selection Sort** | \(O(N^2)\)      | \(O(N^2)\)      | \(O(N^2)\)      | \(O(1)\)         | No      | Minimize memory write cycles    |
| **Insertion Sort** | \(O(N)\)        | \(O(N^2)\)      | \(O(N^2)\)      | \(O(1)\)         | Yes     | Real-time streaming data        |
| **Merge Sort**     | \(O(N \log N)\) | \(O(N \log N)\) | \(O(N \log N)\) | \(O(N)\)         | Yes     | Stable sorting of linked lists  |
| **Quick Sort**     | \(O(N \log N)\) | \(O(N \log N)\) | \(O(N^2)\)      | \(O(\log N)\)    | No      | Default general purpose sorting |

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

## Q19: Matrix Operations in C

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
