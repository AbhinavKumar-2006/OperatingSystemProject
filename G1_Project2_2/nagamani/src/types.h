#ifndef MY_TYPES_H
#define MY_TYPES_H

typedef enum { STATUS_READY, STATUS_RUN, STATUS_DONE } ProcStatus;

// standard structure representing our system task
typedef struct {
    int task_id;
    int time_arrival;
    int time_burst;
    int time_rem;
    int interval;             // tracking period for the rms scheduler
    int lottery_tickets;      // ticket count for probability lottery scheduler
    int prio_lvl;             // ignoring priority levels for this demo
    ProcStatus status;

    // fields for stats later
    int time_done;
    int time_start;
    int processor_id;
} Task;

// represents simulated cpu compute cores
typedef struct {
    int core_id;
    int active_task_id; // mapped active task id, idle state mapped as -1
    int quantum_rem;  // track time slicing limits
} Core;

#endif
