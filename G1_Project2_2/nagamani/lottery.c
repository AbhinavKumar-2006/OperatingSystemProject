#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n, i, total_tickets = 0, winner;
    int bt[10], tickets[10], wt[10], tat[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("Enter burst time for process %d: ", i+1);
        scanf("%d", &bt[i]);

        printf("Enter tickets for process %d: ", i+1);
        scanf("%d", &tickets[i]);

        total_tickets += tickets[i];
    }

    srand(time(0));
    winner = rand() % total_tickets;

    int sum = 0;
    int selected = -1;

    for(i = 0; i < n; i++) {
        sum += tickets[i];
        if(winner < sum) {
            selected = i;
            break;
        }
    }

    printf("\nSelected Process: P%d\n", selected + 1);

    wt[selected] = 0;
    tat[selected] = bt[selected];

    printf("\nProcess\tBT\tWT\tTAT\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n", i+1, bt[i], wt[i], tat[i]);
    }

    return 0;
}
