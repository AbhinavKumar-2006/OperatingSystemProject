
#define MAX_TASKS 15
#define CORE_COUNT 2
#define QUANTUM_Q0 2
#define QUANTUM_Q1 4
#define QUANTUM_Q2 8

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "types.h"

// helper function to draw out a row for the timeline chart
void display_timeline(int curr_tick, Core proc_cores[], int core_count, Task task_arr[]) {
    printf("%02d\t", curr_tick);
    for (int idx = 0; idx < core_count; idx++) {
        if (proc_cores[idx].active_task_id != -1) {
            printf("[T%d]\t", task_arr[proc_cores[idx].active_task_id].task_id);
        } else {
            printf("[--]\t");
        }
    }
    printf("\n");
}

// this function grabs the final numbers and displays them after execution is done
void show_stats(Task task_arr[], int total_tasks) {
    printf("\n--- Performance Stats ---\n");
    printf("TID\tArrival\tBurst\tStart\tFinish\tTurnaround\tWaiting\n");
    printf("---\t-------\t-----\t-----\t------\t----------\t-------\n");

    float sum_tat = 0, sum_wt = 0;
    for (int j = 0; j < total_tasks; j++) {
        int tt = task_arr[j].time_done - task_arr[j].time_arrival;
        int wat  = tt - task_arr[j].time_burst;
        sum_tat += tt;
        sum_wt  += wat;
        printf("T%d\t%d\t%d\t%d\t%d\t%d\t\t%d\n",
               task_arr[j].task_id,
               task_arr[j].time_arrival,
               task_arr[j].time_burst,
               task_arr[j].time_start,
               task_arr[j].time_done,
               tt, wat);
    }
    printf("\nMean Turnaround Time : %.2f\n", sum_tat / total_tasks);
    printf("Mean Waiting Time    : %.2f\n", sum_wt  / total_tasks);
}

// lottery scheduling core implementation
// we use random tickets to assign priority
void execute_lottery_sched(Task task_arr[], int total_tasks, int core_count) {
    printf("\n--- Commencing Lottery Scheduling (Quantum = 4) ---\n");
    printf("Tick\t");
    for (int k = 0; k < core_count; k++) {
        printf("Core %d\t", k);
    }
    printf("\n---------------------------------\n");

    Core proc_cores[core_count];
    for (int idx = 0; idx < core_count; idx++) {
        proc_cores[idx].core_id = idx;
        proc_cores[idx].active_task_id = -1;
        proc_cores[idx].quantum_rem  = 0;
    }

    int clock_tick = 0;
    int done_tasks = 0;
    int slice_limit = 4;

    srand((unsigned int)time(NULL));

    while (done_tasks < total_tasks) {

        // first step: select winners from our ready tasks to place on empty cores
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id != -1)
                continue;   // skip busy cores

            // sum up all the tickets of available ready tasks
            int sum_tickets = 0;
            for (int t = 0; t < total_tasks; t++) {
                if (task_arr[t].status == STATUS_READY &&
                    task_arr[t].time_arrival <= clock_tick) {
                    sum_tickets += task_arr[t].lottery_tickets;
                }
            }

            if (sum_tickets == 0)
                continue;   // nobody waiting right now

            // pick a lucky winner based on tickets
            int lucky_pick = (rand() % sum_tickets) + 1;
            int agg_tickets = 0;

            for (int t = 0; t < total_tasks; t++) {
                if (task_arr[t].status == STATUS_READY &&
                    task_arr[t].time_arrival <= clock_tick) {
                    agg_tickets += task_arr[t].lottery_tickets;
                    if (agg_tickets >= lucky_pick) {
                        // send this task to run
                        proc_cores[idx].active_task_id = t;
                        task_arr[t].status  = STATUS_RUN;
                        task_arr[t].processor_id = idx;
                        proc_cores[idx].quantum_rem = slice_limit;

                        if (task_arr[t].time_start == -1) {
                            task_arr[t].time_start = clock_tick;
                        }
                        break;
                    }
                }
            }
        }

        // next step: record this moment in the timeline
        display_timeline(clock_tick, proc_cores, core_count, task_arr);

        // final step: actually run the tasks on the cores for this tick
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id == -1)
                continue;

            int active_t = proc_cores[idx].active_task_id;
            task_arr[active_t].time_rem--;
            proc_cores[idx].quantum_rem--;

            // mark as done if remaining time is up
            if (task_arr[active_t].time_rem == 0) {
                task_arr[active_t].status = STATUS_DONE;
                task_arr[active_t].time_done = clock_tick + 1;
                proc_cores[idx].active_task_id = -1;
                done_tasks++;
            }
            // or put back to ready if its time block expired
            else if (proc_cores[idx].quantum_rem == 0) {
                task_arr[active_t].status       = STATUS_READY;
                proc_cores[idx].active_task_id = -1;
            }
        }

        clock_tick++;
    }

    show_stats(task_arr, total_tasks);
}

// rms scheduling core implementation
// priority is completely driven by how short the period interval is


// Helper to display visually meaningful timeline
void display_timeline_mlfq(int curr_tick, Core proc_cores[], int core_count, Task task_arr[], int total_tasks) {
    printf("%02d\t", curr_tick);
    
    // Print CPU states
    for (int idx = 0; idx < core_count; idx++) {
        if (proc_cores[idx].active_task_id != -1) {
            int t_id = proc_cores[idx].active_task_id;
            printf("[Q%d-T%d]\t", task_arr[t_id].current_queue, task_arr[t_id].task_id);
        } else {
            printf("[--]\t");
        }
    }

    // Print IO waiting tasks
    printf("| IO: ");
    int num_waiting = 0;
    for (int t = 0; t < total_tasks; t++) {
        if (task_arr[t].status == STATUS_WAIT) {
            printf("[T%d:%dR] ", task_arr[t].task_id, task_arr[t].io_rem);
            num_waiting++;
        }
    }
    if (num_waiting == 0) {
        printf("None");
    }

    printf("\n");
}

void show_stats_mlfq(Task task_arr[], int total_tasks) {
    printf("\n--- Performance Stats ---\n");
    printf("TID\tArrival\tBurst\tStart\tFinish\tTurnaround\tWaiting\n");
    printf("---\t-------\t-----\t-----\t------\t----------\t-------\n");

    float sum_tat = 0, sum_wt = 0;
    for (int j = 0; j < total_tasks; j++) {
        int tt = task_arr[j].time_done - task_arr[j].time_arrival;
        int wat  = tt - task_arr[j].time_burst; 
        // Note: Waiting time here technically includes IO Wait and Ready Queue Wait
        sum_tat += tt;
        sum_wt  += wat;
        printf("T%d\t%d\t%d\t%d\t%d\t%d\t\t%d\n",
               task_arr[j].task_id,
               task_arr[j].time_arrival,
               task_arr[j].time_burst,
               task_arr[j].time_start,
               task_arr[j].time_done,
               tt, wat);
    }
    printf("\nMean Turnaround Time : %.2f\n", sum_tat / total_tasks);
    printf("Mean Waiting Time    : %.2f\n", sum_wt  / total_tasks);
}

int get_quantum(int queue_level) {
    if (queue_level == 0) return QUANTUM_Q0;
    if (queue_level == 1) return QUANTUM_Q1;
    return QUANTUM_Q2; // FCFS behaves as big quantum or bounded by 8
}

void execute_mlfq_sched(Task task_arr[], int total_tasks, int core_count) {
    printf("\n--- Commencing MLFQ Scheduling ---\n");
    printf("Queues: Q0(Q=2) -> Q1(Q=4) -> Q2(Q=8)\n");
    printf("Tick\t");
    for (int k = 0; k < core_count; k++) {
        printf("Core %d\t", k);
    }
    printf("| External Wait States\n");
    printf("----------------------------------------------------------\n");

    Core proc_cores[core_count];
    for (int idx = 0; idx < core_count; idx++) {
        proc_cores[idx].core_id = idx;
        proc_cores[idx].active_task_id = -1;
        proc_cores[idx].quantum_rem  = 0;
    }

    int clock_tick  = 0;
    int done_tasks  = 0;

    while (done_tasks < total_tasks) {

        // Step 1: Manage IO processing / Waiting Tasks
        for (int t = 0; t < total_tasks; t++) {
            if (task_arr[t].status == STATUS_WAIT) {
                task_arr[t].io_rem--;
                if (task_arr[t].io_rem <= 0) {
                    // IO complete, promote to top queue Q0
                    task_arr[t].status = STATUS_READY;
                    task_arr[t].current_queue = 0;
                    task_arr[t].cpu_burst_dur = 0;
                }
            }
        }

        // Step 2: Extract current ready tasks into queue bucket arrays
        int q0[MAX_TASKS], q0_len = 0;
        int q1[MAX_TASKS], q1_len = 0;
        int q2[MAX_TASKS], q2_len = 0;

        for (int t = 0; t < total_tasks; t++) {
            if (task_arr[t].status == STATUS_READY && task_arr[t].time_arrival <= clock_tick) {
                if (task_arr[t].current_queue == 0) q0[q0_len++] = t;
                else if (task_arr[t].current_queue == 1) q1[q1_len++] = t;
                else q2[q2_len++] = t;
            }
        }

        // Step 3: Schedule READY tasks onto empty cores based on MLFQ strict priority
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id != -1) continue; // Skip busy cores
            
            int selected_task = -1;
            int q_id = -1;

            if (q0_len > 0) {
                selected_task = q0[0]; q_id = 0;
                // Shift array
                for(int s=0; s<q0_len-1; s++) q0[s] = q0[s+1];
                q0_len--;
            } else if (q1_len > 0) {
                selected_task = q1[0]; q_id = 1;
                for(int s=0; s<q1_len-1; s++) q1[s] = q1[s+1];
                q1_len--;
            } else if (q2_len > 0) {
                selected_task = q2[0]; q_id = 2;
                for(int s=0; s<q2_len-1; s++) q2[s] = q2[s+1];
                q2_len--;
            }

            if (selected_task != -1) {
                proc_cores[idx].active_task_id = selected_task;
                task_arr[selected_task].status = STATUS_RUN;
                task_arr[selected_task].processor_id = idx;
                proc_cores[idx].quantum_rem = get_quantum(q_id);
                
                if (task_arr[selected_task].time_start == -1) {
                    task_arr[selected_task].time_start = clock_tick;
                }
            }
        }

        // Step 4: Display current tick state
        display_timeline_mlfq(clock_tick, proc_cores, core_count, task_arr, total_tasks);

        // Step 5: Execute tasks for 1 tick bounds
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id == -1) continue;

            int active_t = proc_cores[idx].active_task_id;
            task_arr[active_t].time_rem--;
            task_arr[active_t].cpu_burst_dur++;
            proc_cores[idx].quantum_rem--;

            // Handle condition: Task Finishes COMPLETELY
            if (task_arr[active_t].time_rem == 0) {
                task_arr[active_t].status           = STATUS_DONE;
                task_arr[active_t].time_done        = clock_tick + 1;
                proc_cores[idx].active_task_id      = -1;
                done_tasks++;
            }
            // Handle condition: Task Requests IO (Burst hit IO Freq limit)
            else if (task_arr[active_t].io_freq > 0 && 
                     task_arr[active_t].cpu_burst_dur >= task_arr[active_t].io_freq) {
                task_arr[active_t].status = STATUS_WAIT;
                task_arr[active_t].io_rem = task_arr[active_t].io_dur;
                proc_cores[idx].active_task_id = -1;
            }
            // Handle condition: Task quantum expires (Demote to lower Queue)
            else if (proc_cores[idx].quantum_rem == 0) {
                task_arr[active_t].status = STATUS_READY;
                // Demote if not already at bottom queue
                if (task_arr[active_t].current_queue < 2) {
                    task_arr[active_t].current_queue++;
                }
                proc_cores[idx].active_task_id = -1;
            }
        }

        clock_tick++;
    }

    show_stats_mlfq(task_arr, total_tasks);
}
