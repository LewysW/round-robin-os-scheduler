#include "sched.h"

/**
Initialises queue mutex, head, and tail.
@queue - to initialise
@return - result of initialisation
*/
int initQueue(Queue** queue) {
    *queue = (Queue*) malloc(sizeof(Queue));
    if (pthread_mutex_init(&((*queue)->lock), NULL) != 0) {
        perror("Queue mutex initialisation failed");
        return -1;
    }

    (*queue)->head = (*queue)->tail = NULL;
    return 0;
}

/**
Makes a node for the queue
@p - struct process to make node for
@return - pointer to new node
*/
Node* makeNode(struct process* p) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->proc = p;
    node->next = NULL;
    return node;
}

/**
Frees node data structure
@node - to be freed
**/
void freeNode(Node* node) {
    free(node->proc->path);
    for (int i = 1; i < node->proc->argc; i++) free(node->proc->args[i]);
    free(node->proc);
    free(node);
}

/**
Creates node and enqueues in queue
@p - process to create node for and
*/
void enqueue(Queue* queue, struct process* p) {
    Node* node = makeNode(p);

    if (queue->head == NULL) {
        queue->tail = queue->head = node;
    }

    queue->tail->next = node;
    node->next = NULL;
    queue->tail = node;
}

/**
Dequeues head of queue
@queue - to dequeue head of
@return - dequeued node
*/
Node* dequeue(Queue* queue) {
    if (isEmpty(queue)) return NULL;

    Node* node;
    node = queue->head;
    queue->head = queue->head->next;
    return node;
}

/**
Checks if queue is empty
@queue - queue to check
@return - whether queue is empty
*/
bool isEmpty(Queue* queue) {
    return (queue->head == NULL);
}

/**
Moves node at head of queue to tail
@queue - queue to perform move operation on
@return - result of operation
*/
int headToTail(Queue* queue) {
    if (queue->head == NULL) return -1;

    struct process* temp;
    temp = queue->head->proc;
    queue->head = queue->head->next;
    enqueue(queue, temp);
    return 0;
}

/**
Prints process struct of each node in queue
@queue - queue to iterate over and print
*/
void printQueue(Queue* queue) {
    Node* node = queue->head;
    printf("\nHEAD --> ");

    while (node != NULL) {
        printf("[%d %s", node->proc->priority, node->proc->path);
        for (int i = 0; i < node->proc->argc; i++) printf(" %s", node->proc->args[i]);
        printf("] --> ");
        node = node->next;
    }

    printf("TAIL --> ");
    printf("NULL\n");
}
