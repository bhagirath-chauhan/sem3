#include <stdio.h>

struct Process
{
    int pid;
    int arrival;
    int burst;
    int priority; // 1 = highest priority
    int start;
    int finish;
    int responseTime;
    int waiting;
    int turnAroundTime;
    int isCompleted; // 0 = not completed, 1 = completed
};

int main()
{
    int n, i;
    int time = 0, completed = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // Input section
    for (i = 0; i < n; i++)
    {
        p[i].pid = i + 1;
        printf("\nEnter Arrival Time, Burst Time, Priority for Process %d: ",
               p[i].pid);
        scanf("%d %d %d", &p[i].arrival, &p[i].burst, &p[i].priority);
        p[i].isCompleted = 0;
    }

    while (completed < n)
    {

        int idx = -1;
        int highestPriority = 999999; // Smaller number → higher priority

        // P1 0 4 2
        // P2 1 2 1
        // P3 2 6 3
        //  Find highest priority available process
        for (i = 0; i < n; i++)
        {
            if (!p[i].isCompleted && p[i].arrival <= time)
            {
                if (p[i].priority < highestPriority)
                {
                    highestPriority = p[i].priority; // 1
                    idx = i;                         // 1
                }
            }
        }

        if (idx == -1)
        {
            time++; // CPU idle
        }
        else
        {
            // P1 0 4 2
            // P2 1 2 1
            // P3 2 6 3
            p[idx].start = time;                                    // 0 4
            p[idx].finish = p[idx].start + p[idx].burst;            // 4 6
            p[idx].turnAroundTime = p[idx].finish - p[idx].arrival; // 4 5
            p[idx].waiting = p[idx].turnAroundTime - p[idx].burst;  // 0 3
            p[idx].responseTime = p[idx].start - p[idx].arrival;    // 0 3

            p[idx].isCompleted = 1;
            time = p[idx].finish; // 6
            completed++;          // 2
        }
    }

    // Output
    printf("\nPID\tAT\tBT\tPR\tST\tFT\tRT\tWT\tTAT\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].priority, p[i].start, p[i].finish,
               p[i].responseTime, p[i].waiting, p[i].turnAroundTime);
    }

    return 0;
}