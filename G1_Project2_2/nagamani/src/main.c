#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "types.h"

// set up initialization for all our core mock tasks
// this handles assigning predefined tickets, periods, and bursts
void init_tasks(Task task_mem[], int n) {
    Task base_data[20] = {
        // initial early arrivals
        { 1,   0,   8,  8,  20,  10, 0, STATUS_READY, 0, -1, -1},
        { 2,   1,  25, 25,  80,   5, 0, STATUS_READY, 0, -1, -1},
        { 3,   2,   3,  3,  10,  20, 0, STATUS_READY, 0, -1, -1},
        { 4,   5,  12, 12,  40,   8, 0, STATUS_READY, 0, -1, -1},
        { 5,   8,   2,  2,  15,  25, 0, STATUS_READY, 0, -1, -1},

        // arriving around the middle of sim
        { 6,  10,  40, 40, 120,   3, 0, STATUS_READY, 0, -1, -1},
        { 7,  12,   5,  5,  25,  15, 0, STATUS_READY, 0, -1, -1},
        { 8,  15,   6,  6,  30,  12, 0, STATUS_READY, 0, -1, -1},
        { 9,  20,  15, 15,  50,   7, 0, STATUS_READY, 0, -1, -1},
        {10,  22,   4,  4,  20,  18, 0, STATUS_READY, 0, -1, -1},

        // arriving tasks near the end
        {11,  25,   8,  8,  35,  10, 0, STATUS_READY, 0, -1, -1},
        {12,  30,  50, 50, 150,   2, 0, STATUS_READY, 0, -1, -1},
        {13,  35,   2,  2,  12,  30, 0, STATUS_READY, 0, -1, -1},
        {14,  40,   9,  9,  45,   9, 0, STATUS_READY, 0, -1, -1},
        {15,  45,   3,  3,  18,  22, 0, STATUS_READY, 0, -1, -1},

        // latest arrivals
        {16,  50,  18, 18,  70,   6, 0, STATUS_READY, 0, -1, -1},
        {17,  55,   5,  5,  22,  16, 0, STATUS_READY, 0, -1, -1},
        {18,  60,   7,  7,  30,  11, 0, STATUS_READY, 0, -1, -1},
        {19,  65,  22, 22, 100,   4, 0, STATUS_READY, 0, -1, -1},
        {20,  70,   4,  4,  16,  20, 0, STATUS_READY, 0, -1, -1}
    };

    for (int k = 0; k < n; k++) {
        task_mem[k] = base_data[k];
    }
}

int main() {
    int total_tasks = 20;
    int total_cores = 2;
    Task task_list[20];

    int user_choice;

    printf("\n+++ Multiprocessor Scheduling Simulator +++\n");
    printf("   Algorithms available: Lottery & RMS\n");
    printf("   Number of Processing Cores: %d\n", total_cores);
    printf("   Total tasks initialized: %d\n\n", total_tasks);

    while (1) {
        init_tasks(task_list, total_tasks);

        printf("Available choices:\n");
        printf(" 1. Run Lottery Scheduler\n");
        printf(" 2. Run Rate Monotonic Scheduler (RMS)\n");
        printf(" 3. Quit\n> ");

        if (scanf("%d", &user_choice) != 1) break;

        switch (user_choice) {
            case 1:
                execute_lottery_sched(task_list, total_tasks, total_cores);
                break;
            case 2:
                execute_rms_sched(task_list, total_tasks, total_cores);
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
