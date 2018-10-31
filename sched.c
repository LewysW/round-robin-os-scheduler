#include "sched.h"
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
    //If number of args is incorrect, print usage and exit with error code -1
    if (argc < 2) {
        printf("Usage: './sched <name of config file>'\n");
        exit(0);
    }

    //If readFile fails, print error message and exit.
    if ((parseFile(argv[1])) != 0) {
        perror("Error reading config file");
        exit(1);
    }

    return 0;
}

/**
Parse a given file line-by-line
@fileName - file to read
@return - status code
*/
int parseFile(char* fileName) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t bytesRead;
    struct process proc;
    Queue* queue = (Queue*) malloc(sizeof(queue));

    fp = fopen(fileName, "r");

    if (fp == NULL) return -1;

    while ((bytesRead = getline(&line, &len, fp)) != EOF) {
        int status = initStruct(line, &proc);

        if (status != 0) {
            perror("Invalid config file entry");
            exit(status);
        }

        enqueue(queue, proc);
        //TODO add process to queue HERE!!!
    }

    printQueue(queue);
    printf("\n");
    headToTail(queue);
    printQueue(queue);
    headToTail(queue);
    printQueue(queue);
    headToTail(queue);
    printQueue(queue);
    free(line);
    fclose(fp);
    return 0;
}

/**
Intiliases a process struct given a config file entry
@line - entry to be tokenised into struct
@proc - process struct to be initialised
@return - the status code
*/
int initStruct(char* line, struct process* proc) {
    const char* delim = " ";
    char* token;

    //Checks for valid token
    if ((token = strtok(line, delim)) == NULL) return -1;
    //Checks that priority is numeric
    if (!(isNumeric(token))) return -1;
    proc->priority = atoi(token);

    //Checks for valid token
    if ((token = strtok(NULL, delim)) == NULL) return -1;

    //Checks that path is valid executable file
    if (!(isExec(token))) return -1;
    proc->path = (char*) malloc(sizeof(char*));
    strncpy(proc->path, token, strlen(token));

    proc->argc = 0;
    proc->args = (char**) malloc(sizeof(char**));

    //Tokenises and stores arguments of config file entry
    while ((token = strtok(NULL, delim)) != NULL) {
        proc->args[proc->argc] = (char*) malloc(sizeof(char*));
        strncpy(proc->args[proc->argc++], token, strlen(token) + 1);
    }

    return 0;
}

/**
Frees the memory allocated to a process struct
@p - process struct pointer to free
*/
int freeStruct(struct process* p) {
    free(p->path);
    for (int i = 0; i < p->argc; i++) free(p->args[i]);
    free(p->args);
    return 0;
}

/**
Determines if string is valid path to executable file
@path - path of file to check
@return - true or false if file is valid executable
*/
bool isExec(char* path) {
    struct stat buf;
    //If file is valid, is a regular file, and is executable
    if (!(stat(path, &buf)) && S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR)) return true;

    return false;
}

/**
Checks if a string is a number
@str - string to evaluate.
@return - true or false if string is numeric
*/
bool isNumeric(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }
    return true;
}
