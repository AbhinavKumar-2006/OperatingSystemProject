#ifndef ADITYA_TYPES_H
#define ADITYA_TYPES_H

typedef enum { STATUS_READY, STATUS_RUN, STATUS_WAIT, STATUS_DONE } ProcStatus;

// Standard structure representing our system task
typedef struct {
    int task_id;
    int time_arrival;
    int time_burst;
    int time_rem;

    // MLFQ & IO specific tracking
    int io_freq;           // How often task triggers memory/IO
    int io_dur;            // How long task waits for IO
    int io_rem;            // Remaining ticks in current IO wait
    int cpu_burst_dur;     // Time spent on CPU since last IO wait
    int current_queue;     // 0: High priority, 1: Medium, 2: FCFS

    // CFS specific tracking
    double vruntime;       // Virtual runtime for CFS scheduling
    int weight;            // Weight based on niceness for CFS
    int nice_val;          // Nice value (-5 to 5 range in our sim)

    // Standard variables
    int prio_lvl;
    ProcStatus status;

    // Fields for stats
    int time_done;
    int time_start;
    int processor_id;
} Task;

// Represents simulated CPU compute cores
typedef struct {
    int core_id;
    int active_task_id; // mapped active task id, idle state mapped as -1
    int quantum_rem;    // track time slicing limits
} Core;

#endif
