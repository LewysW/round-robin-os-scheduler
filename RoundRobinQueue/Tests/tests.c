#include "../sched.h"


//Defines red and green character codes for test output
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define WHITE   "\033[39m"
//Defines number of tests to be run by test suite
#define NUM_TESTS 0

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

}

int main() {
    //Creates list of bool functions to store test functions
    bool (*unitTests[NUM_TESTS]) ();
    initTests(unitTests);
    runTests(unitTests, NUM_TESTS);
}
