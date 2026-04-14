#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "types.h"

#define MAX_TASKS 15

#define QUANTUM_Q0 2
#define QUANTUM_Q1 4
#define QUANTUM_Q2 8

/* =========================================================================
 *  SHARED UTILITY FUNCTIONS
 * ========================================================================= */

// Helper to display visually meaningful timeline with IO states
void display_timeline(int curr_tick, Core proc_cores[], int core_count, Task task_arr[], int total_tasks) {
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

// Simpler timeline display for CFS (no queue level, shows vruntime instead)
void display_cfs_timeline(int curr_tick, Core proc_cores[], int core_count, Task task_arr[], int total_tasks) {
    printf("%02d\t", curr_tick);

    // Print CPU states
    for (int idx = 0; idx < core_count; idx++) {
        if (proc_cores[idx].active_task_id != -1) {
            int t_id = proc_cores[idx].active_task_id;
            printf("[T%d:vr=%.1f]\t", task_arr[t_id].task_id, task_arr[t_id].vruntime);
        } else {
            printf("[--]\t\t");
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

void show_stats(Task task_arr[], int total_tasks) {
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

/* =========================================================================
 *  MLFQ SCHEDULING ALGORITHM
 *  -------------------------
 *  Three-level feedback queue with IO handling:
 *    Q0 (quantum=2) -> Q1 (quantum=4) -> Q2 (quantum=8, FCFS-like)
 *  Tasks that complete IO burst are promoted back to Q0.
 * ========================================================================= */

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
        display_timeline(clock_tick, proc_cores, core_count, task_arr, total_tasks);

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

    show_stats(task_arr, total_tasks);
}

/* =========================================================================
 *  COMPLETELY FAIR SCHEDULER (CFS) ALGORITHM
 *  ------------------------------------------
 *  Inspired by the Linux CFS. The core idea:
 *    - Every task tracks a "virtual runtime" (vruntime).
 *    - The task with the SMALLEST vruntime is picked next.
 *    - vruntime increments are weighted by "nice" value:
 *        vruntime_increment = (1.0 / weight) * time_executed
 *      Tasks with higher weight (lower nice) accumulate vruntime slower,
 *      so they get more real CPU time.
 *    - Dynamic time slices are computed per-task proportional to weight.
 *    - IO handling: tasks that block for IO have their vruntime set to
 *      the minimum vruntime of all ready tasks upon return (sleeper fairness).
 *
 *  Weight table (simplified):
 *    nice -5 => weight 3277
 *    nice -4 => weight 2621
 *    nice -3 => weight 2097
 *    nice -2 => weight 1678
 *    nice -1 => weight 1342
 *    nice  0 => weight 1024 (default)
 *    nice  1 => weight  820
 *    nice  2 => weight  655
 *    nice  3 => weight  526
 *    nice  4 => weight  423
 *    nice  5 => weight  335
 * ========================================================================= */

// CFS weight lookup table (from Linux sched_prio_to_weight, scaled down)
static const int cfs_weight_table[] = {
    3277,  // nice = -5
    2621,  // nice = -4
    2097,  // nice = -3
    1678,  // nice = -2
    1342,  // nice = -1
    1024,  // nice =  0  (default weight)
     820,  // nice =  1
     655,  // nice =  2
     526,  // nice =  3
     423,  // nice =  4
     335   // nice =  5
};

int cfs_get_weight(int nice_val) {
    // nice_val ranges from -5 to 5, maps to index 0..10
    int idx = nice_val + 5;
    if (idx < 0)  idx = 0;
    if (idx > 10) idx = 10;
    return cfs_weight_table[idx];
}

// Compute a dynamic time quantum for CFS based on weight proportion
// Minimum granularity is 1 tick, target latency is 6 ticks
int cfs_compute_timeslice(Task task_arr[], int total_tasks, int current_task, int clock_tick) {
    int total_weight = 0;
    int ready_count = 0;

    for (int t = 0; t < total_tasks; t++) {
        if ((task_arr[t].status == STATUS_READY || task_arr[t].status == STATUS_RUN) &&
             task_arr[t].time_arrival <= clock_tick) {
            total_weight += task_arr[t].weight;
            ready_count++;
        }
    }

    if (total_weight == 0 || ready_count == 0) return 1;

    int target_latency = 6; // sched_latency equivalent
    if (ready_count > 6) {
        target_latency = ready_count; // scale up for many tasks
    }

    int slice = (int)((double)task_arr[current_task].weight / total_weight * target_latency);
    if (slice < 1) slice = 1; // minimum granularity
    return slice;
}

// Find the minimum vruntime among all ready/running tasks (for sleeper fairness)
double cfs_min_vruntime(Task task_arr[], int total_tasks, int clock_tick) {
    double min_vr = DBL_MAX;
    for (int t = 0; t < total_tasks; t++) {
        if ((task_arr[t].status == STATUS_READY || task_arr[t].status == STATUS_RUN) &&
             task_arr[t].time_arrival <= clock_tick) {
            if (task_arr[t].vruntime < min_vr) {
                min_vr = task_arr[t].vruntime;
            }
        }
    }
    return (min_vr == DBL_MAX) ? 0.0 : min_vr;
}

void execute_cfs_sched(Task task_arr[], int total_tasks, int core_count) {
    printf("\n--- Commencing CFS (Completely Fair Scheduler) ---\n");
    printf("Scheduling by minimum vruntime; weighted by nice value\n");
    printf("Tick\t");
    for (int k = 0; k < core_count; k++) {
        printf("Core %d\t\t", k);
    }
    printf("| External Wait States\n");
    printf("--------------------------------------------------------------------------\n");

    // Initialize weights based on nice values
    for (int t = 0; t < total_tasks; t++) {
        task_arr[t].weight = cfs_get_weight(task_arr[t].nice_val);
    }

    Core proc_cores[core_count];
    for (int idx = 0; idx < core_count; idx++) {
        proc_cores[idx].core_id = idx;
        proc_cores[idx].active_task_id = -1;
        proc_cores[idx].quantum_rem = 0;
    }

    int clock_tick = 0;
    int done_tasks = 0;

    while (done_tasks < total_tasks) {

        // Step 1: Handle IO completion
        double min_vr = cfs_min_vruntime(task_arr, total_tasks, clock_tick);
        for (int t = 0; t < total_tasks; t++) {
            if (task_arr[t].status == STATUS_WAIT) {
                task_arr[t].io_rem--;
                if (task_arr[t].io_rem <= 0) {
                    task_arr[t].status = STATUS_READY;
                    task_arr[t].cpu_burst_dur = 0;
                    // Sleeper fairness: set vruntime to min so returning IO tasks
                    // get a fair chance, but don't let them starve others
                    task_arr[t].vruntime = min_vr;
                }
            }
        }

        // Step 2: Check if running tasks have exhausted their time slice
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id == -1) continue;
            if (proc_cores[idx].quantum_rem <= 0) {
                int running_t = proc_cores[idx].active_task_id;
                task_arr[running_t].status = STATUS_READY;
                proc_cores[idx].active_task_id = -1;
            }
        }

        // Step 3: Pick the task with the SMALLEST vruntime for each idle core
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id != -1) continue;

            int best_task = -1;
            double best_vruntime = DBL_MAX;

            for (int t = 0; t < total_tasks; t++) {
                if (task_arr[t].status == STATUS_READY && task_arr[t].time_arrival <= clock_tick) {
                    if (task_arr[t].vruntime < best_vruntime) {
                        best_vruntime = task_arr[t].vruntime;
                        best_task = t;
                    }
                }
            }

            if (best_task != -1) {
                proc_cores[idx].active_task_id = best_task;
                task_arr[best_task].status = STATUS_RUN;
                task_arr[best_task].processor_id = idx;
                proc_cores[idx].quantum_rem = cfs_compute_timeslice(task_arr, total_tasks, best_task, clock_tick);

                if (task_arr[best_task].time_start == -1) {
                    task_arr[best_task].time_start = clock_tick;
                }
            }
        }

        // Step 4: Preemption check — if a ready task has significantly lower
        // vruntime than a running task, preempt the running task
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id == -1) continue;
            int run_t = proc_cores[idx].active_task_id;

            for (int t = 0; t < total_tasks; t++) {
                if (task_arr[t].status == STATUS_READY && task_arr[t].time_arrival <= clock_tick) {
                    // Preempt if ready task's vruntime is significantly less
                    if (task_arr[t].vruntime + 2.0 < task_arr[run_t].vruntime) {
                        // Check this ready task isn't already assigned elsewhere
                        int already_assigned = 0;
                        for (int c = 0; c < core_count; c++) {
                            if (proc_cores[c].active_task_id == t) {
                                already_assigned = 1;
                                break;
                            }
                        }
                        if (!already_assigned) {
                            task_arr[run_t].status = STATUS_READY;
                            proc_cores[idx].active_task_id = t;
                            task_arr[t].status = STATUS_RUN;
                            task_arr[t].processor_id = idx;
                            proc_cores[idx].quantum_rem = cfs_compute_timeslice(task_arr, total_tasks, t, clock_tick);
                            if (task_arr[t].time_start == -1) {
                                task_arr[t].time_start = clock_tick;
                            }
                            break;
                        }
                    }
                }
            }
        }

        // Step 5: Display current tick state
        display_cfs_timeline(clock_tick, proc_cores, core_count, task_arr, total_tasks);

        // Step 6: Execute tasks for 1 tick and update vruntime
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id == -1) continue;

            int active_t = proc_cores[idx].active_task_id;
            task_arr[active_t].time_rem--;
            task_arr[active_t].cpu_burst_dur++;
            proc_cores[idx].quantum_rem--;

            // Update vruntime: inversely proportional to weight
            // Higher weight => slower vruntime growth => more CPU time overall
            task_arr[active_t].vruntime += (1024.0 / task_arr[active_t].weight);

            // Handle condition: Task Finishes COMPLETELY
            if (task_arr[active_t].time_rem == 0) {
                task_arr[active_t].status      = STATUS_DONE;
                task_arr[active_t].time_done   = clock_tick + 1;
                proc_cores[idx].active_task_id = -1;
                done_tasks++;
            }
            // Handle condition: Task Requests IO
            else if (task_arr[active_t].io_freq > 0 &&
                     task_arr[active_t].cpu_burst_dur >= task_arr[active_t].io_freq) {
                task_arr[active_t].status = STATUS_WAIT;
                task_arr[active_t].io_rem = task_arr[active_t].io_dur;
                proc_cores[idx].active_task_id = -1;
            }
            // Handle condition: Time slice expired (handled at top of next iteration)
        }

        clock_tick++;
    }

    show_stats(task_arr, total_tasks);
}
