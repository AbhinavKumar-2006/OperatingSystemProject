#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define MAX 50

typedef struct {
    int pid, arrival, burst, remaining;
    int deadline;
    int waiting, turnaround;
    int completed;
} Process;

Process p[MAX];
int n;

int select_process(int time) {
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

void edf(int cpu_count) {
    int time = 0, completed = 0;
    int cpu[cpu_count];

    for (int i = 0; i < cpu_count; i++)
        cpu[i] = -1;

    while (completed < n) {

        for (int c = 0; c < cpu_count; c++) {
            if (cpu[c] == -1) {
                cpu[c] = select_process(time);
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

void display() {
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

int main() {
    int cpu_count;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of CPUs: ");
    scanf("%d", &cpu_count);

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        p[i].pid = i + 1;

        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);

        printf("Burst Time: ");
        scanf("%d", &p[i].burst);

        printf("Deadline: ");
        scanf("%d", &p[i].deadline);

        p[i].remaining = p[i].burst;
        p[i].completed = 0;
    }

    edf(cpu_count);
    display();

    return 0;
}