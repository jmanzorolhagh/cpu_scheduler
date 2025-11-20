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
Process* read_processes(const char* filename, int* count) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    int lines = 0;
    char buffer[256];
        if (fgets(buffer, sizeof(buffer), fp) == NULL) return NULL; 

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(buffer) > 1) lines++; 
    }
    *count = lines;
    Process* list = (Process*)malloc(lines * sizeof(Process));

    rewind(fp);
    fgets(buffer, sizeof(buffer), fp); 

    int i = 0;
    while (i < lines && fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(buffer) <= 1) continue;

        int pid, arrival, burst, priority;
        
        int itemsRead = sscanf(buffer, "%d,%d,%d,%d", &pid, &arrival, &burst, &priority);
        
        list[i].pid = pid;
        list[i].arrivalTime = arrival;
        list[i].burstTime = burst;

        if (itemsRead == 4) {
            list[i].priority = priority;
        } else {
            list[i].priority = 0; 
        }

        if (list[i].priority < 0) list[i].priority = 0;
        if (list[i].priority >= MAX_PRIORITY) list[i].priority = MAX_PRIORITY - 1;

        list[i].remainingTime = list[i].burstTime;
        list[i].waitingTime = 0;
        list[i].turnaroundTime = 0;
        list[i].completionTime = 0;
        list[i].responseTime = -1; 
        i++;
    }

    fclose(fp);
    return list;
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <time_quantum>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int time_quantum = atoi(argv[2]);
    
    if (time_quantum <= 0) {
        printf("Error: Time quantum must be positive.\n");
        return 1;
    }

    int count = 0;
    Process* list = read_processes(filename, &count);
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j].arrivalTime > list[j+1].arrivalTime) {
                Process temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
            }
        }
    }
    printf("\nStarting Simulation | Quantum: %d | Processes: %d\n", time_quantum, count);

    Queue* queues[MAX_PRIORITY];
    for(int i = 0; i < MAX_PRIORITY; i++) {
        queues[i] = createQueue();
    }
    
    int currentTime = 0;
    int completedProcesses = 0;
    int timeInQuantum = 0;
    int contextSwitchCounter = 0;
    Process* currentProcess = NULL;
    
}