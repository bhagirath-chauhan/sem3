#include <stdio.h> // Standard I/O functions

// Structure to hold all details of each process
struct Process
{
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int burst;      // Burst Time
    int start;      // Start Time
    int finish;     // Finish Time
    int waiting;    // Waiting Time
    int turnaround; // Turnaround Time
    int responseTime;
    int completed; // Flag to mark if process is finished
};

int main()
{
    int n, i;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // ------------------------------
    // INPUT SECTION
    // ------------------------------
    for (i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("\nProcess %d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].arrival);

        printf("Process %d Burst Time: ", p[i].pid);
        scanf("%d", &p[i].burst);

        p[i].completed = 0; // Mark all processes as not completed initially
    }

    // ------------------------------
    // SJF SCHEDULING STARTS
    // ------------------------------

    int time = 0;           // Keeps track of CPU time
    int completedCount = 0; // How many processes finished

    while (completedCount < n)
    {

        int idx = -1;          // Index of process selected to run
        int minBurst = 999999; // A very large number for comparison

        // ---------------------------------------------------
        // FIND PROCESS WITH SHORTEST BURST AMONG ARRIVED ONES
        // ---------------------------------------------------
        for (int i = 0; i < n; i++)
        {
            // Condition to choose:
            // 1. process is not completed
            // 2. arrival time <= current time (means it has arrived)
            // 3. burst time is the shortest

            // P1 3 6
            // P2 0 4
            // P3 5 2
            // P4 1 3
            // P5 8 4

            // time = 7
            // arrival = 5
            // burst = 2
            if (p[i].completed == 0 && p[i].arrival <= time)
            {
                if (p[i].burst < minBurst)
                {
                    minBurst = p[i].burst;
                    // 2

                    idx = i; // select this process 2
                }
            }
        }

        // ---------------------------------------------------
        // IF NO PROCESS HAS ARRIVED YET, CPU IS IDLE
        // ---------------------------------------------------
        if (idx == -1)
        {
            time++; // CPU stays idle, move time forward
            continue;
        }

        // ---------------------------------------------------
        // RUN THE SELECTED PROCESS (NON-PREEMPTIVE)
        // ---------------------------------------------------

        // P2 0 4
        // P4 1 3
        // P3 5 2

        p[idx].start = time;                                // 0 4 7                // Start time = current CPU time
        p[idx].finish = time + p[idx].burst;                // 4 7 9 // Finish time = start + burst
        p[idx].turnaround = p[idx].finish - p[idx].arrival; // 4 6 4
        p[idx].waiting = p[idx].turnaround - p[idx].burst;  // 0 3 2
        p[idx].responseTime = p[idx].start - p[idx].arrival;

        time = p[idx].finish; // 4 7 9 // Update time for next process
        p[idx].completed = 1; // Mark process as finished
        completedCount++;     // Increase completed count
    }

    // ------------------------------
    // OUTPUT SECTION
    // ------------------------------

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