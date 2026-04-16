#include <stdio.h>
#include "algorithms.h"
#include "types.h"

int main() {
    Process p[MAX];
    int n = 5;          // number of processes
    int cpu_count = 2;  // number of CPUs

    // Predefined process data
    // {pid, arrival, burst, remaining, deadline, waiting, turnaround, completed, queue}

    p[0] = (Process){1, 0, 5, 5, 10, 0, 0, 0, 0};
    p[1] = (Process){2, 1, 3, 3, 8,  0, 0, 0, 0};
    p[2] = (Process){3, 2, 4, 4, 6,  0, 0, 0, 0};
    p[3] = (Process){4, 3, 2, 2, 7,  0, 0, 0, 0};
    p[4] = (Process){5, 4, 6, 6, 12, 0, 0, 0, 0};

    int choice;

    printf("\n=== Scheduling Simulator ===\n");
    printf("1. SJF\n2. EDF\nChoose: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nRunning SJF...\n");
        sjf(p, n, cpu_count);
    } else if (choice == 2) {
        printf("\nRunning EDF...\n");
        edf(p, n, cpu_count);
    } else {
        printf("Invalid choice\n");
        return 0;
    }

    display(p, n);

    return 0;
}
