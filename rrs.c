#include <stdio.h>
#include <limits.h> // used for INT_MAX if needed in expansions

// -------------------------------------------
// PROCESS STRUCTURE
// -------------------------------------------
struct Process
{
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int burst;      // Original Burst Time
    int remaining;  // Remaining time (will decrease during execution)
    int finish;     // Finish Time
    int waiting;    // Waiting Time
    int turnaround; // Turnaround Time
    int start;
    int responseTime;
};

int main()
{

    int n, quantum;

    // -------------------------------------------
    // INPUT: NUMBER OF PROCESSES + TIME QUANTUM
    // -------------------------------------------
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);

    struct Process p[n];

    // -------------------------------------------
    // INPUT: DETAILS OF EACH PROCESS
    // -------------------------------------------
    for (int i = 0; i < n; i++)
    {

        p[i].pid = i + 1; // Assign PID 1,2,3...

        printf("\nProcess %d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].arrival);

        printf("Process %d Burst Time: ", p[i].pid);
        scanf("%d", &p[i].burst);

        // Remaining time = full burst time (initially)
        p[i].remaining = p[i].burst;
    }

    // -------------------------------------------
    // ROUND ROBIN CPU SIMULATION VARIABLES
    // -------------------------------------------
    int time = 0;      // Current CPU time
    int completed = 0; // Count how many processes are finished
    int doneFlag;      // Checks if at least one process ran in a cycle

    // -------------------------------------------
    // MAIN ROUND-ROBIN LOOP (runs until all complete)
    // -------------------------------------------
    while (completed < n)
    {

        doneFlag = 0; // reset for this pass

        // -------------------------------------------
        // ITERATE OVER ALL PROCESSES IN ROUND ROBIN ORDER
        // -------------------------------------------
        for (int i = 0; i < n; i++)
        {

            // PROCESS CAN RUN ONLY IF:
            // 1. It has already arrived (arrival <= time)
            // 2. It still has some remaining time
            // P1 3 6 4
            // P2 0 4 0
            // P3 5 2 0
            // P4 1 3 1
            // P5 8 4 4
            // quantum 2
            // time 10
            if (p[i].arrival <= time && p[i].remaining > 0)
            {

                doneFlag = 1; // at least one process got CPU time

                if (p[i].remaining == p[i].burst)
                {
                    p[i].start = time;
                    p[i].responseTime = p[i].start - p[i].arrival;
                }

                // CASE 1: Remaining time > quantum → run for "quantum"
                if (p[i].remaining > quantum)
                {

                    // Use quantum units on CPU
                    time += quantum; // 6

                    // Subtract used time from remaining
                    p[i].remaining -= quantum; // 4
                }

                // CASE 2: Remaining time <= quantum → process will finish!
                else
                {

                    // Increase time only by the amount required to finish
                    time += p[i].remaining; // 11

                    p[i].remaining = 0; // now completed

                    completed++; // increase completed count

                    // Record finish time
                    p[i].finish = time;

                    // Calculate turnaround time = FT - AT
                    p[i].turnaround = p[i].finish - p[i].arrival; // 10

                    // Waiting time = TAT - BT
                    p[i].waiting = p[i].turnaround - p[i].burst; // 7
                }
            }
        }

        // -------------------------------------------
        // CPU WAS IDLE (nobody arrived yet)
        // -------------------------------------------
        if (doneFlag == 0)
        {
            time++; // move time forward
        }
    }

    // -------------------------------------------
    // OUTPUT TABLE
    // -------------------------------------------
    printf("\nPID\tAT\tBT\tST\tFT\tWT\tTAT\tRD\n");
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