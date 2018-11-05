#ifndef SCHED_H
#define SCHED_H
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define DECIMAL 10
#define QUANTUM 500000

//Struct to store process data
struct process {
    pid_t pid;
    int priority;
    char* path;
    char** args;
    int argc;
};

//Struct to store queue node
typedef struct node {
    struct process proc;
    struct node* next;
} Node;

//Struct to store queue
typedef struct queue {
    Node* head;
    Node* tail;
    pthread_mutex_t lock;
} Queue;

//Initialises a queue
int initQueue(Queue* queue);

//Creates a queue node
Node* makeNode(struct process p);

//Enqueues a new node to the queue
void enqueue(Queue* queue, struct process p);

//Dequeues head node of queue and returns it
Node* dequeue(Queue* queue);

//Moves head of queue to tail
int headToTail(Queue* queue);

//Checks if queue is empty
bool isEmpty(Queue* queue);

//Prints contents of struct processes in queue
void printQueue(Queue* queue);

//Parse file line by line
int parseFile(char* fileName);

//Schedules processes using second thread of execution
void* schedule(void* arg);

//Initialises a process struct given a string and process pointer
int initStruct(char* line, struct process* proc);

//Checks if string is numeric
bool isNumeric(char* str);

#endif
