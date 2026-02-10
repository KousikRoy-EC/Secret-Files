
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100 // Maximum size for Array-based Stack, Queue, Deque

/* 
 * ==========================================
 * Data Structures Implementation in C
 * aimed for last-minute revision.
 * Includes: 
 * 1. Linked List (Singly)
 * 2. Stack (Array-based)
 * 3. Queue (Circular Array-based)
 * 4. Deque (Circular Array-based)
 * 5. BST (Pointer-based)
 * ==========================================
 */

// ==========================================
// 1. SINGLY LINKED LIST
// ==========================================
/*
 * Structure: Node
 * Contains data and a pointer to the next node.
 */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

/*
 * Function: createNode
 * Allocates memory for a new node.
 */
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/*
 * Function: insertAtHead (Linked List)
 * Time Complexity: O(1)
 */
void insertAtHead(Node** head, int data) {
    Node* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
    printf("Linked List: Inserted %d at head\n", data);
}

/*
 * Function: insertAtTail (Linked List)
 * Time Complexity: O(n)
 */
void insertAtTail(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        printf("Linked List: Inserted %d at head (list was empty)\n", data);
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    printf("Linked List: Inserted %d at tail\n", data);
}

/*
 * Function: deleteValue (Linked List)
 * Time Complexity: O(n)
 */
void deleteValue(Node** head, int key) {
    Node* temp = *head;
    Node* prev = NULL;

    // If head node itself holds the key
    if (temp != NULL && temp->data == key) {
        *head = temp->next;
        free(temp);
        printf("Linked List: Deleted %d\n", key);
        return;
    }

    // Search for the key
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }

    // Key not present
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;
    free(temp);
    printf("Linked List: Deleted %d\n", key);
}

/*
 * Function: displayList
 * Time Complexity: O(n)
 */
void displayList(Node* head) {
    printf("List: ");
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

// ==========================================
// 2. STACK (Array Based)
// ==========================================
/*
 * Stack follows LIFO (Last In First Out).
 * Implemented using a fixed-size array.
 */
typedef struct Stack {
    int items[MAX];
    int top;
} Stack;

/*
 * Function: initStack
 * Initializes stack top to -1.
 */
void initStack(Stack* s) {
    s->top = -1;
}

/*
 * Function: isStackFull
 * Returns true if stack is full.
 */
bool isStackFull(Stack* s) {
    return s->top == MAX - 1;
}

/*
 * Function: isStackEmpty
 * Returns true if stack is empty.
 */
bool isStackEmpty(Stack* s) {
    return s->top == -1;
}

/*
 * Function: push
 * Time Complexity: O(1)
 */
void push(Stack* s, int value) {
    if (isStackFull(s)) {
        printf("Stack Overflow! Cannot push %d\n", value);
    } else {
        s->items[++(s->top)] = value;
        printf("Stack: Pushed %d\n", value);
    }
}

/*
 * Function: pop
 * Time Complexity: O(1)
 */
int pop(Stack* s) {
    if (isStackEmpty(s)) {
        printf("Stack Underflow! Cannot pop\n");
        return -1;
    } else {
        int poppedValue = s->items[(s->top)--];
        return poppedValue;
    }
}

/*
 * Function: peek
 * Time Complexity: O(1)
 */
int peek(Stack* s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\n");
        return -1;
    }
    return s->items[s->top];
}

// ==========================================
// 3. CIRCULAR QUEUE (Array Based)
// ==========================================
/*
 * Queue follows FIFO (First In First Out).
 * Implemented using a circular array to efficiently utilize space.
 */
typedef struct Queue {
    int items[MAX];
    int front, rear;
} Queue;

/*
 * Function: initQueue
 * Initializes front and rear to -1.
 */
void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

/*
 * Function: isQueueFull
 * Checks if the next position of rear wraps around to front.
 */
bool isQueueFull(Queue* q) {
    return ((q->rear + 1) % MAX == q->front);
}

/*
 * Function: isQueueEmpty
 * Checks if queue is initially empty.
 */
bool isQueueEmpty(Queue* q) {
    return (q->front == -1);
}

/*
 * Function: enqueue
 * Time Complexity: O(1)
 */
void enqueue(Queue* q, int value) {
    if (isQueueFull(q)) {
        printf("Queue Overflow! Cannot enqueue %d\n", value);
        return;
    }
    if (isQueueEmpty(q)) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % MAX;
    q->items[q->rear] = value;
    printf("Queue: Enqueued %d\n", value);
}

/*
 * Function: dequeue
 * Time Complexity: O(1)
 */
int dequeue(Queue* q) {
    if (isQueueEmpty(q)) {
        printf("Queue Underflow! Cannot dequeue\n");
        return -1;
    }
    int item = q->items[q->front];
    if (q->front == q->rear) {
        // Last element removed, reset queue
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX;
    }
    return item;
}

// ==========================================
// 4. DEQUE (Double Ended Queue - Array Based)
// ==========================================
/*
 * Deque allows insertion and deletion at both ends.
 * Implemented using a circular array logic.
 */
typedef struct Deque {
    int items[MAX];
    int front, rear;
} Deque;

void initDeque(Deque* dq) {
    dq->front = -1;
    dq->rear = -1;
}

bool isDequeFull(Deque* dq) {
    return ((dq->front == 0 && dq->rear == MAX - 1) || (dq->front == dq->rear + 1));
}

bool isDequeEmpty(Deque* dq) {
    return (dq->front == -1);
}

/*
 * Function: insertFront
 * Time Complexity: O(1)
 */
void insertFront(Deque* dq, int value) {
    if (isDequeFull(dq)) {
        printf("Deque Overflow! Cannot insert %d at front\n", value);
        return;
    }
    if (isDequeEmpty(dq)) {
        dq->front = 0;
        dq->rear = 0;
    } else if (dq->front == 0) {
        dq->front = MAX - 1; // Wrap around
    } else {
        dq->front--;
    }
    dq->items[dq->front] = value;
    printf("Deque: Inserted %d at Front\n", value);
}

/*
 * Function: insertRear
 * Time Complexity: O(1)
 */
void insertRear(Deque* dq, int value) {
    if (isDequeFull(dq)) {
        printf("Deque Overflow! Cannot insert %d at rear\n", value);
        return;
    }
    if (isDequeEmpty(dq)) {
        dq->front = 0;
        dq->rear = 0;
    } else if (dq->rear == MAX - 1) {
        dq->rear = 0; // Wrap around
    } else {
        dq->rear++;
    }
    dq->items[dq->rear] = value;
    printf("Deque: Inserted %d at Rear\n", value);
}

/*
 * Function: deleteFront
 * Time Complexity: O(1)
 */
void deleteFront(Deque* dq) {
    if (isDequeEmpty(dq)) {
        printf("Deque Underflow! Cannot delete from front\n");
        return;
    }
    int item = dq->items[dq->front];
    if (dq->front == dq->rear) {
        dq->front = -1;
        dq->rear = -1;
    } else if (dq->front == MAX - 1) {
        dq->front = 0;
    } else {
        dq->front++;
    }
    printf("Deque: Deleted %d from Front\n", item);
}

/*
 * Function: deleteRear
 * Time Complexity: O(1)
 */
void deleteRear(Deque* dq) {
    if (isDequeEmpty(dq)) {
        printf("Deque Underflow! Cannot delete from rear\n");
        return;
    }
    int item = dq->items[dq->rear];
    if (dq->front == dq->rear) {
        dq->front = -1;
        dq->rear = -1;
    } else if (dq->rear == 0) {
        dq->rear = MAX - 1;
    } else {
        dq->rear--;
    }
    printf("Deque: Deleted %d from Rear\n", item);
}

// ==========================================
// 5. BINARY SEARCH TREE (BST)
// ==========================================
/*
 * BST Properties: Left child < Root < Right child.
 */
typedef struct BSTNode {
    int data;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

BSTNode* createBSTNode(int data) {
    BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

/*
 * Function: insertBST
 * Time Complexity: O(h), h = height of tree
 */
BSTNode* insertBST(BSTNode* root, int data) {
    if (root == NULL) return createBSTNode(data);

    if (data < root->data)
        root->left = insertBST(root->left, data);
    else if (data > root->data)
        root->right = insertBST(root->right, data);

    return root;
}

/*
 * Function: searchBST
 * Time Complexity: O(h)
 */
bool searchBST(BSTNode* root, int key) {
    if (root == NULL) return false;
    if (root->data == key) return true;

    if (key < root->data)
        return searchBST(root->left, key);
    
    return searchBST(root->right, key);
}

/*
 * Function: inorderTraversal (Left, Root, Right)
 * Produces sorted output for BST.
 * Time Complexity: O(n)
 */
void inorderTraversal(BSTNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

// ==========================================
// MAIN FUNCTION (Driver Code)
// ==========================================
int main() {
    printf("=== LINKED LIST DEMO ===\n");
    Node* head = NULL;
    insertAtHead(&head, 10);
    insertAtTail(&head, 20);
    insertAtHead(&head, 5);
    displayList(head); // 5 -> 10 -> 20 -> NULL
    deleteValue(&head, 10);
    displayList(head); // 5 -> 20 -> NULL
    printf("\n");

    printf("=== STACK DEMO (Array) ===\n");
    Stack stack;
    initStack(&stack);
    push(&stack, 100);
    push(&stack, 200);
    printf("Popped from stack: %d\n", pop(&stack)); // 200
    printf("Top element is %d\n", peek(&stack)); // 100
    printf("\n");

    printf("=== QUEUE DEMO (Circular Array) ===\n");
    Queue q;
    initQueue(&q);
    enqueue(&q, 10);
    enqueue(&q, 20);
    printf("Dequeued: %d\n", dequeue(&q)); // 10
    printf("Dequeued: %d\n", dequeue(&q)); // 20
    printf("\n");

    printf("=== DEQUE DEMO (Array) ===\n");
    Deque dq;
    initDeque(&dq);
    insertRear(&dq, 5);
    insertFront(&dq, 10);
    deleteRear(&dq); // Removes 5
    deleteFront(&dq); // Removes 10
    printf("\n");

    printf("=== BST DEMO ===\n");
    BSTNode* bstRoot = NULL;
    bstRoot = insertBST(bstRoot, 50);
    insertBST(bstRoot, 30);
    insertBST(bstRoot, 20);
    insertBST(bstRoot, 40);
    insertBST(bstRoot, 70);
    insertBST(bstRoot, 60);
    insertBST(bstRoot, 80);

    printf("Inorder traversal: ");
    inorderTraversal(bstRoot); // 20 30 40 50 60 70 80
    printf("\nSearch 40: %s\n", searchBST(bstRoot, 40) ? "Found" : "Not Found");
    printf("Search 90: %s\n", searchBST(bstRoot, 90) ? "Found" : "Not Found");

    return 0;
}
