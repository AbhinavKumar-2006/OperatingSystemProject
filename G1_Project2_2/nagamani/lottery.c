#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Process {
    int pid;
    int bt;       
    int tickets;  
    int wt;       
    int tat;      
    int remaining; 
    int done;     
};

// Lottery draw: returns index of winning process
int lotteryDraw(struct Process p[], int n, int totalTickets) {
    int winning = (rand() % totalTickets) + 1;
    int cumulative = 0;
    for (int i = 0; i < n; i++) {
        if (!p[i].done) {
            cumulative += p[i].tickets;
            if (winning <= cumulative)
                return i;
        }
    }
    return -1;
}

int main() {
    int n;
    srand(time(0));

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &p[i].bt);
        printf("Enter tickets for process %d: ", i + 1);
        scanf("%d", &p[i].tickets);
        p[i].remaining = p[i].bt;
        p[i].wt  = 0;
        p[i].tat = 0;
        p[i].done = 0;
    }

    int currentTime = 0;
    int completed = 0;

    // Calculate total tickets (only undone processes)
    while (completed < n) {
        int totalTickets = 0;
        for (int i = 0; i < n; i++)
            if (!p[i].done)
                totalTickets += p[i].tickets;

        int idx = lotteryDraw(p, n, totalTickets);
        if (idx == -1) break;

        printf("\nSelected Process: P%d\n", p[idx].pid);

        // Run selected process to completion (non-preemptive)
        p[idx].wt  = currentTime;
        currentTime += p[idx].bt;
        p[idx].tat = currentTime;
        p[idx].done = 1;
        completed++;
    }

    // Display Results
    printf("\nProcess\tBT\tWT\tTAT\n");
    float avgWT = 0, avgTAT = 0;
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n", p[i].pid, p[i].bt, p[i].wt, p[i].tat);
        avgWT  += p[i].wt;
        avgTAT += p[i].tat;
    }

    printf("\nAverage Waiting Time    : %.2f\n", avgWT / n);
    printf("Average Turnaround Time : %.2f\n", avgTAT / n);

    return 0;
}
   

   
