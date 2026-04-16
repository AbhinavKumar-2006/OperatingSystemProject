#include <stdio.h>
#include "algorithms.h"

int time_quantum[QUEUES] = {2, 4, 8};

/* ---------------- MLFQ ---------------- */
/* ---------------- SJF ---------------- */
void sjf(Process p[], int n, int cpu_count) {
    int time = 0, completed = 0;
    int cpu[cpu_count];

    for (int i = 0; i < cpu_count; i++)
        cpu[i] = -1;

    while (completed < n) {

        // assign shortest job to free CPUs
        for (int c = 0; c < cpu_count; c++) {
            if (cpu[c] == -1) {
                int idx = -1;
                int min_burst = 9999;

                for (int i = 0; i < n; i++) {
                    if (p[i].remaining > 0 && p[i].arrival <= time) {
                        if (p[i].burst < min_burst) {
                            min_burst = p[i].burst;
                            idx = i;
                        }
                    }
                }

                if (idx != -1)
                    cpu[c] = idx;
            }
        }

        // execute
        for (int c = 0; c < cpu_count; c++) {
            if (cpu[c] != -1) {
                int i = cpu[c];

                p[i].remaining--;

                if (p[i].remaining == 0) {
                    completed++;
                    p[i].turnaround = time + 1 - p[i].arrival;
                    p[i].waiting = p[i].turnaround - p[i].burst;
                    cpu[c] = -1;
                }
            }
        }

        time++;
    }
}
/* ---------------- EDF ---------------- */
int select_process(Process p[], int n, int time) {
    int idx = -1;
    int min_deadline = 9999;

    for (int i = 0; i < n; i++) {
        if (!p[i].completed && p[i].arrival <= time) {
            if (p[i].deadline < min_deadline) {
                min_deadline = p[i].deadline;
                idx = i;
            }
        }
    }
    return idx;
}

void edf(Process p[], int n, int cpu_count) {
    int time = 0, completed = 0;
    int cpu[cpu_count];

    for (int i = 0; i < cpu_count; i++)
        cpu[i] = -1;

    while (completed < n) {

        for (int c = 0; c < cpu_count; c++) {
            if (cpu[c] == -1) {
                cpu[c] = select_process(p, n, time);
            }
        }

        for (int c = 0; c < cpu_count; c++) {
            if (cpu[c] != -1) {
                int i = cpu[c];

                p[i].remaining--;

                if (p[i].remaining == 0) {
                    p[i].completed = 1;
                    completed++;
                    p[i].turnaround = time + 1 - p[i].arrival;
                    p[i].waiting = p[i].turnaround - p[i].burst;
                    cpu[c] = -1;
                }
            }
        }

        time++;
    }
}

/* ---------------- DISPLAY ---------------- */
void display(Process p[], int n) {
    float avg_wt = 0, avg_tat = 0;

    printf("\nPID\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", p[i].pid, p[i].waiting, p[i].turnaround);
        avg_wt += p[i].waiting;
        avg_tat += p[i].turnaround;
    }

    printf("\nAverage WT: %.2f", avg_wt / n);
    printf("\nAverage TAT: %.2f\n", avg_tat / n);
}
