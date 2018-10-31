#include "sched.h"

int initQueue(Queue* queue) {
    if (pthread_mutex_init(&(queue->lock), NULL) != 0) {
        perror("Queue mutex initialisation failed");
        exit(1);
    }

    queue->head = queue->tail = NULL;
    return 0;
}

/**
Makes a node for the queue
@p - struct process to make node for
@return - pointer to new node
*/
Node* makeNode(struct process p) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->proc = p;
    node->next = NULL;
    return node;
}

/**
Creats node and enqueues in queue
@p - process to create node for and
*/
void enqueue(Queue* queue, struct process p) {
    Node* node = makeNode(p);

    if (queue->head == NULL) {
        queue->tail = queue->head = node;
    }

    queue->tail->next = node;
    node->next = NULL;
    queue->tail = node;
}

int headToTail(Queue* queue) {
    if (queue->head == NULL) return -1;

    struct process temp;
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
    printf("HEAD\n-->");

    while (node != NULL) {
        printf("%d %s", node->proc.priority, node->proc.path);
        for (int i = 0; i < node->proc.argc; i++) printf(" %s", node->proc.args[i]);
        printf("-->");
        node = node->next;
    }

    printf("TAIL\n-->");
    printf("NULL\n");
}
