#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIORITY 10

typedef struct {
    int pid;
    int arrivalTime;
    int burstTime;
    int priority; 
    int remainingTime;
    
    int waitingTime;
    int turnaroundTime;
    int completionTime;
    int responseTime;
} Process;

typedef struct Node {
    Process* data;      
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}
void push(Queue* q, Process* p) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = p;
    newNode->next = NULL;

    if (q->front == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}
Process* pop(Queue* q) {
    if (q->front == NULL) return NULL;

    Node* temp = q->front;
    Process* p = temp->data;

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp); 
    return p;
}