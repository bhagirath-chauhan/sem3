#include <stdio.h>

struct Process
{
    int pid;
    int arrival;
    int burst;
    int remaining;
    int start;
    int finish;
    int waiting;
    int turnaround;
    int responseTime;
};

int main()
{

    int n, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);

    struct Process p[n];

    for (int i = 0; i < n; i++)
    {
        p[i].pid = i + 1;
        printf("\nProcess %d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].arrival);

        printf("Process %d Burst Time: ", p[i].pid);
        scanf("%d", &p[i].burst);

        p[i].remaining = p[i].burst;
        p[i].start = -1; // set start time not executed yet
    }

    // Ready Queue
    int queue[100];
    int front = 0, rear = 0;

    int time = 0;
    int completed = 0;
    int visited[n];

    for (int i = 0; i < n; i++)
        visited[i] = 0;

    // push processes that arrive at time 0
    // quantum 2
    // P1 0 2
    // P2 2 5
    // [P1]
    // rear = 1
    // front = 0
    // time = 0;

    for (int i = 0; i < n; i++)
    {
        if (p[i].arrival == 0)
        {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    // main loop

    // quantum 2
    // P1 0 2 2
    // P2 2 5
    // P3 1 3
    // P4 4 4
    // P5 6 1
    // [P1]
    // rear = 1
    // front = 0
    // time = 0;
    while (completed < n)
    {
        // if queue empty → idle CPU
        if (front == rear)
        {
            time++;
            // push newly arrived processes
            for (int i = 0; i < n; i++)
            {
                if (p[i].arrival == time && visited[i] == 0)
                {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++]; // front 1

        // FIRST TIME executing
        if (p[idx].start == -1)
        {
            p[idx].start = time;
            p[idx].responseTime = p[idx].start - p[idx].arrival;
        }

        // run for quantum or remaining
        int runTime = (p[idx].remaining > quantum) ? quantum : p[idx].remaining;
        time += runTime;             // 2
        p[idx].remaining -= runTime; // 0

        // quantum 2
        // P1 0 2 2
        // P2 2 5 5
        // P3 1 3 3
        // P4 4 4 4
        // P5 6 1 1
        // after time increases → add new arrivals
        for (int t = time - runTime + 1; t <= time; t++)
        {
            for (int i = 0; i < n; i++)
            {
                if (p[i].arrival == t && visited[i] == 0)
                {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
        }

        // if process still not completed → push back
        if (p[idx].remaining > 0)
        {
            queue[rear++] = idx;
        }
        else
        {
            // process finished
            completed++;
            p[idx].finish = time;                               // 2
            p[idx].turnaround = p[idx].finish - p[idx].arrival; // 2
            p[idx].waiting = p[idx].turnaround - p[idx].burst;  // 0
        }
    }

    // OUTPUT
    printf("\nPID\tAT\tBT\tST\tFT\tWT\tTAT\tRT\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].start,
               p[i].finish,
               p[i].waiting,
               p[i].turnaround,
               p[i].responseTime);
    }

    return 0;
}