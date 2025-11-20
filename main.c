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