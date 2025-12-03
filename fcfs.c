#include <stdio.h> // Standard I/O library for printf and scanf

// Structure to store all details of each process
struct Process
{
    int pid;        // Process ID (1, 2, 3, ...)
    int arrival;    // Arrival Time
    int burst;      // Burst Time
    int start;      // Time at which process starts executing
    int finish;     // Time at which process finishes
    int waiting;    // Waiting Time = Start Time - Arrival Time
    int turnaround; // Turnaround Time = Finish Time - Arrival Time
};

int main()
{

    int n, time;
    printf("Enter number of processes: ");
    scanf("%d", &n); // Input: number of processes

    struct Process p[n]; // Array of processes

    // ------------------------------
    // INPUT SECTION
    // ------------------------------
    for (int i = 0; i < n; i++)
    {
        p[i].pid = i + 1; // Assign process ID automatically

        printf("\nProcess %d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].arrival); // Input arrival time

        printf("Process %d Burst Time: ", p[i].pid);
        scanf("%d", &p[i].burst); // Input burst time
    }

    // ------------------------------
    // SORT PROCESSES BY ARRIVAL TIME (FCFS RULE)
    // ------------------------------

    // Simple bubble sort to sort array by arrival time
    // P1 3 6
    // P2 0 4
    // P3 5 2
    // P4 1 3
    // P5 8 4
    // n = 5;

    // 0 1 3 8
    for (int i = 0; i < n - 1; i++)
    { // Outer loop runs n-1 times

        for (int j = i + 1; j < n; j++)
        { // Inner loop compares pairs
            if (p[j].arrival < p[i].arrival)
            { // If next process arrives earlier, swap
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    // ------------------------------
    // FCFS SCHEDULING LOGIC
    // ------------------------------

    time = 0; // Keeps track of current CPU time

    // P2 0 4
    // P4 1 3
    // P1 3 6
    // P3 5 2
    // P5 8 4
    for (int i = 0; i < n; i++)
    {
        // time = 0
        // If CPU is idle and next process arrives later
        if (time < p[i].arrival)
        {
            time = p[i].arrival;
            // CPU waits until process arrives
        }

        p[i].start = time;                            // 0 4                            // Process starts at current time
        p[i].finish = time + p[i].burst;              // 4 7              // Finish = start + burst
        p[i].turnaround = p[i].finish - p[i].arrival; // 4 6// TAT formula
        p[i].waiting = p[i].start - p[i].arrival;     // 0 3    // Waiting formula

        time = p[i].finish; // 7 // Update CPU time for next process
    }

    // ------------------------------
    // OUTPUT THE RESULT TABLE
    // ------------------------------

    printf("\nPID\tAT\tBT\tST\tFT\tWT\tTAT\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].start,
               p[i].finish,
               p[i].waiting,
               p[i].turnaround);
    }

    return 0;
}