#ifndef TYPES_H
#define TYPES_H

#define MAX 50
#define QUEUES 3

typedef struct {
    int pid, arrival, burst, remaining;
    int deadline;
    int waiting, turnaround;
    int completed;
    int queue;
} Process;

#endif
