#include "../sched.h"
#include <string.h>


//Defines red and green character codes for test output
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define WHITE   "\033[39m"
//Defines number of tests to be run by test suite
#define NUM_TESTS 16

/**
Tests that queue is correctly initialised by initQueue
**/
bool testInitQueue1() {
    Queue* queue;
    int status = initQueue(&queue);
    return queue != NULL && status == 0;
}

/**
Tests that head and tail of queue are correct after initialisation
**/
bool testInitQueue2() {
    Queue* queue;
    initQueue(&queue);
    return (queue->head == NULL && queue->tail == NULL);
}

/**
Tests that makeNode() successfully initialises a node
**/
bool testMakeNode1() {
    struct process p;
    Node* node = makeNode(&p);
    return (node != NULL);
}

/**
Tests that makeNode() successfully initialises a nodes next value
**/
bool testMakeNode2() {
    struct process p;
    Node* node = makeNode(&p);
    return (node->next == NULL);
}

/**
Tests that makeNode() successfully initialises a nodes process value
**/
bool testMakeNode3() {
    struct process p;
    Node* node = makeNode(&p);
    return (&p == node->proc);
}

/**
Tests that a single process can be successfully enqueued to a queue
**/
bool testEnqueue1() {
    Queue* queue;
    initQueue(&queue);
    struct process p;
    enqueue(queue, &p);
    return (queue->head->proc == &p && queue->tail->proc == &p);
}

/**
Tests that two processes can be successfully enqueued to a queue
**/
bool testEnqueue2() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    struct process p2;
    enqueue(queue, &p1);
    enqueue(queue, &p2);
    return (queue->head->proc == &p1 && queue->tail->proc == &p2 \
            && queue->head->next == queue->tail);
}

/**
Tests that three processes can be successfully enqueued to a queue
**/
bool testEnqueue3() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    struct process p2;
    struct process p3;
    enqueue(queue, &p1);
    enqueue(queue, &p2);
    enqueue(queue, &p3);
    return (queue->head->proc == &p1 && queue->tail->proc == &p3 \
            && queue->head->next->proc == &p2);
}

/**
Tests that a new queue is initially empty
**/
bool testIsEmpty1() {
    Queue* queue;
    initQueue(&queue);
    return (isEmpty(queue));
}

/**
Tests that a queue with a node enqueued is not empty
**/
bool testIsEmpty2() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    enqueue(queue, &p1);
    return (!isEmpty(queue));
}

/**
Tests that one process can be successfully returned using dequeue
**/
bool testEnqueueDequeue1() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    enqueue(queue, &p1);
    Node* node = dequeue(queue);
    return (node->proc == &p1);
}

/**
Tests that processes are dequeued in the order they were enqueued in (FIFO)
**/
bool testEnqueueDequeue2() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    struct process p2;
    enqueue(queue, &p1);
    enqueue(queue, &p2);
    Node* node1 = dequeue(queue);
    Node* node2 = dequeue(queue);
    return (node1->proc == &p1 && node2->proc == &p2);
}

/**
Tests that the queue is empty after an enqueue and dequeue operation
**/
bool testIsEmpty3() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    enqueue(queue, &p1);
    dequeue(queue);
    return (isEmpty(queue));
}

/**
Tests that the queue is empty after two enqueue and dequeue operations
**/
bool testIsEmpty4() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    struct process p2;
    enqueue(queue, &p1);
    enqueue(queue, &p2);
    dequeue(queue);
    dequeue(queue);
    return (isEmpty(queue));
}

/**
Tests that headToTail returns -1 for an empty queue
**/
bool testHeadToTail1() {
    Queue* queue;
    initQueue(&queue);
    return (headToTail(queue) == -1);
}

/**
Tests that the headToTail moves process at head of queue to tail successfully
**/
bool testHeadToTail2() {
    Queue* queue;
    initQueue(&queue);
    struct process p1;
    struct process p2;
    struct process p3;
    enqueue(queue, &p1);
    enqueue(queue, &p2);
    enqueue(queue, &p3);
    headToTail(queue);
    return (queue->tail->proc == &p1 && queue->head->proc == &p2);
}

/**
Runs a given list of unit tests.
Iterates over each unit test in the list of test functions. If calling the
current function returns true then the test has passed, otherwise it has failed.
@testFunctions - list of unit tests to run
@numTests - num tests to run
@returns - number of passing unit tests
*/
int runTests(bool (*testFunctions[]) (), int numTests) {
    //Number of tests that have passed
    int numPassingTests = 0;

    //Lists result of each unit test
    int i;
    for (i = 0; i < numTests; i++) {
        if ((*testFunctions[i]) ()) {
            numPassingTests += 1;
            printf(GREEN);
            printf("\n***TEST ");
            printf("%d", i + 1);
            printf(" PASSED***\n");
        } else {
            printf(RED);
            printf("\n***TEST ");
            printf("%d", i + 1);
            printf(" FAILED***\n");
        }
    }

    //Displays total number of unit tests which have passed
    printf("\n***");
    printf("%d", numPassingTests);
    printf("/");
    printf("%d", numTests);
    printf(" TESTS PASSED***\n");

    printf(WHITE);
    return numPassingTests;
}

/**
Initialises the array of tests to be run with the address of each unit test function.
@testFunctions - list of test functions to be called
**/
void initTests(bool (*testFunctions[]) ()) {
    testFunctions[0] = testInitQueue1;
    testFunctions[1] = testInitQueue2;
    testFunctions[2] = testMakeNode1;
    testFunctions[3] = testMakeNode2;
    testFunctions[4] = testMakeNode3;
    testFunctions[5] = testEnqueue1;
    testFunctions[6] = testEnqueue2;
    testFunctions[7] = testEnqueue3;
    testFunctions[8] = testIsEmpty1;
    testFunctions[9] = testIsEmpty2;
    testFunctions[10] = testEnqueueDequeue1;
    testFunctions[11] = testEnqueueDequeue2;
    testFunctions[12] = testIsEmpty3;
    testFunctions[13] = testIsEmpty4;
    testFunctions[14] = testHeadToTail1;
    testFunctions[15] = testHeadToTail2;
}

int main() {
    //Creates list of bool functions to store test functions
    bool (*unitTests[NUM_TESTS]) ();
    initTests(unitTests);
    runTests(unitTests, NUM_TESTS);
}
