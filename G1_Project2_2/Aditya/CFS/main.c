#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "types.h"

#define MAX_TASKS 15
#define CORE_COUNT 2

// Set up initialization for all our core mock tasks
// This handles assigning predefined IO frequencies, durations, and nice values
void init_tasks(Task task_arr[], int total_tasks) {
    Task base_data[MAX_TASKS] = {
        // task_id, arrival, burst, time_rem, io_freq, io_dur, io_rem, cpu_burst_dur, current_queue,
        // vruntime, weight, nice_val, prio_lvl, status, time_done, time_start, processor_id

        // CPU bound tasks (0 IO freq) — these test pure scheduling behavior
        {  1,  0, 15, 15,  0,  0, 0, 0, 0,  0.0, 1024,  0, 0, STATUS_READY, 0, -1, -1 },
        // High IO frequency tasks — tests IO handling and queue promotion
        {  2,  1,  6,  6,  2,  3, 0, 0, 0,  0.0, 1024,  2, 0, STATUS_READY, 0, -1, -1 },
        {  3,  2, 10, 10,  4,  5, 0, 0, 0,  0.0, 1024, -2, 0, STATUS_READY, 0, -1, -1 },
        {  4,  4,  4,  4,  0,  0, 0, 0, 0,  0.0, 1024,  0, 0, STATUS_READY, 0, -1, -1 },
        {  5,  5, 20, 20,  8,  4, 0, 0, 0,  0.0, 1024, -5, 0, STATUS_READY, 0, -1, -1 },
        {  6,  8,  5,  5,  0,  0, 0, 0, 0,  0.0, 1024,  3, 0, STATUS_READY, 0, -1, -1 },
        {  7, 10, 12, 12,  3,  2, 0, 0, 0,  0.0, 1024, -1, 0, STATUS_READY, 0, -1, -1 },
        {  8, 12,  8,  8,  0,  0, 0, 0, 0,  0.0, 1024,  1, 0, STATUS_READY, 0, -1, -1 },
        {  9, 15, 18, 18,  5,  6, 0, 0, 0,  0.0, 1024,  5, 0, STATUS_READY, 0, -1, -1 },
        { 10, 18,  9,  9,  0,  0, 0, 0, 0,  0.0, 1024,  0, 0, STATUS_READY, 0, -1, -1 },
        { 11, 20,  3,  3,  1,  2, 0, 0, 0,  0.0, 1024, -3, 0, STATUS_READY, 0, -1, -1 },
        { 12, 22, 25, 25, 10,  5, 0, 0, 0,  0.0, 1024,  4, 0, STATUS_READY, 0, -1, -1 },
        { 13, 25,  7,  7,  3,  3, 0, 0, 0,  0.0, 1024, -4, 0, STATUS_READY, 0, -1, -1 },
        { 14, 28, 14, 14,  6,  4, 0, 0, 0,  0.0, 1024,  0, 0, STATUS_READY, 0, -1, -1 },
        { 15, 30,  5,  5,  0,  0, 0, 0, 0,  0.0, 1024,  0, 0, STATUS_READY, 0, -1, -1 }
    };
    for (int k = 0; k < total_tasks; k++) {
        task_arr[k] = base_data[k];
    }
}

int main() {
    int total_tasks = MAX_TASKS;
    int total_cores = CORE_COUNT;
    Task task_list[MAX_TASKS];

    int user_choice;

    printf("\n+++ Multiprocessor Scheduling Simulator (MLFQ & CFS) +++\n");
    printf("   Algorithms: MLFQ (Multilevel Feedback Queue) & CFS (Completely Fair Scheduler)\n");
    printf("   Number of Processing Cores: %d\n", total_cores);
    printf("   Total tasks initialized: %d\n\n", total_tasks);

    while (1) {
        init_tasks(task_list, total_tasks);

        printf("Available choices:\n");
        printf(" 1. Run MLFQ Scheduler\n");
        printf(" 2. Run CFS (Completely Fair Scheduler)\n");
        printf(" 3. Quit\n> ");

        scanf("%d", &user_choice);

        switch (user_choice) {
            case 1:
                execute_mlfq_sched(task_list, total_tasks, total_cores);
                break;
            case 2:
                execute_cfs_sched(task_list, total_tasks, total_cores);
                break;
            case 3:
                printf("\nExiting. See you later!\n");
                exit(0);
            default:
                printf("\nThat didn't work. Pick 1, 2, or 3.\n");
        }
    }

    return 0;
}
