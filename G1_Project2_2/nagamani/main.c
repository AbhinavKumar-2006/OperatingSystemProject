#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mlfq() {
    int n, i;
    int bt[10], wt[10], tat[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("Enter burst time for process %d: ", i+1);
        scanf("%d", &bt[i]);
    }

    wt[0] = 0;
    for(i = 1; i < n; i++) {
        wt[i] = wt[i-1] + bt[i-1];
    }

    for(i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
    }

    printf("\nMLFQ Output:\n");
    printf("Process\tBT\tWT\tTAT\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n", i+1, bt[i], wt[i], tat[i]);
    }
}

void lottery() {
    int n, i, total = 0, winner;
    int bt[10], tickets[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("Enter burst time for process %d: ", i+1);
        scanf("%d", &bt[i]);

        printf("Enter tickets for process %d: ", i+1);
        scanf("%d", &tickets[i]);

        total += tickets[i];
    }

    srand(time(0));
    winner = rand() % total;

    int sum = 0;
    for(i = 0; i < n; i++) {
        sum += tickets[i];
        if(winner < sum) {
            printf("\nSelected Process: P%d\n", i+1);
            break;
        }
    }
}

int main() {
    int choice;

    printf("\nSelect Scheduling Algorithm:\n");
    printf("1. MLFQ\n");
    printf("2. Lottery Scheduling\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            mlfq();
            break;
        case 2:
            lottery();
            break;
        default:
            printf("Invalid choice\n");
    }

    return 0;
}
