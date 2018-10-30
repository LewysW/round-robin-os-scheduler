#ifndef SCHED_H
#define SCHED_H
#include <stdbool.h>

#define DECIMAL 10

//Struct to store process data
struct process {
    int pid;
    int priority;
    char* path;
    char* arguments;
    bool completed;
};

//Parse file line by line
int parseFile(char* fileName);

//Initialises a process struct given a string and process pointer
int initStruct(char* line, struct process* proc);

//Checks if path is valid executable file
bool isExec(char* path);

//Checks if string is numeric
bool isNumeric(char* str);

#endif
