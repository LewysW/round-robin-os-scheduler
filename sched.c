#include "sched.h"
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

static bool finished = false;

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
    pthread_t tid;
    pid_t pid;

    fp = fopen(fileName, "r");

    if (fp == NULL) return -1;

    pthread_create(&(tid), NULL, schedule, queue);

    //Reads file line by line, converting each line into a process
    while ((bytesRead = getline(&line, &len, fp)) != EOF) {
        int status = initStruct(line, &proc);

        if (status != 0) {
            perror("Invalid config file entry");
            exit(status);
        }

        pid = fork();

        if (pid < 0) {
            return -1;
        } else if (pid > 0) {
            kill(pid, SIGSTOP);
            proc.pid = pid;
        } else {
            int status = execvp(proc.path, proc.args);
            printf("ERROR: %s\n", strerror(errno));
            exit(status);
        }

        //Enqueues a process to the queue
        pthread_mutex_lock(&(queue->lock));
        enqueue(queue, proc);
        pthread_mutex_unlock(&(queue->lock));
    }
    printQueue(queue);

    finished = true;
    pthread_join(tid, NULL);
    free(line);
    fclose(fp);
    return 0;
}

/**
Round robin scheduler for processes using second thread of execution
@arg - queue of processes to be run
*/
void* schedule(void* arg) {
    Queue* queue = ((Queue*) arg);
    printQueue(queue);

    while(true) {
        if (finished && isEmpty(queue)) break;
        if (!isEmpty(queue)) {
            kill(queue->head->proc.pid, SIGCONT);
            usleep(500000);
            kill(queue->head->proc.pid, SIGSTOP);
            pthread_mutex_lock(&(queue->lock));
            headToTail(queue);
            printQueue(queue);
            pthread_mutex_unlock(&(queue->lock));
        }
    }
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
    proc->completed = false;

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

    //Stores path in first argument of args
    proc->argc = 0;
    proc->args = (char**) malloc(sizeof(char**));
    proc->args[proc->argc] = (char*) malloc(sizeof(char*));
    strncpy(proc->args[proc->argc++], proc->path, strlen(proc->path) + 1);

    //Tokenises and stores arguments of config file entry
    while ((token = strtok(NULL, delim)) != NULL) {
        proc->args[proc->argc] = (char*) malloc(sizeof(char*));
        strncpy(proc->args[proc->argc++], token, strlen(token) + 1);
    }

    //Sets final argument to NULL so it can be run by execvp()
    proc->args[proc->argc] = NULL;

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
