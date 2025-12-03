#include <stdio.h>  // Standard input/output functions
#include <limits.h> // For INT_MAX (a very large number used for comparison)

// Structure to store each process information
struct Process
{
    int pid;     // Process ID
    int arrival; // Arrival time of process
    int burst;   // Original burst time
    int start;
    int responseTime;
    int remaining;  // Remaining time left for execution
    int finish;     // Finishing time (when process completed)
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
};

int main()
{

    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n); // Input total number of processes

    // Create an array of 'n' processes
    struct Process p[n];

    // ---------------- INPUT SECTION -----------------
    for (int i = 0; i < n; i++)
    {

        p[i].pid = i + 1; // Assign process ID like P1, P2, P3...

        printf("\nProcess %d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].arrival); // Input arrival time

        printf("Process %d Burst Time: ", p[i].pid);
        scanf("%d", &p[i].burst); // Input burst time

        p[i].remaining = p[i].burst; // Remaining time initially = burst time
    }

    // --------------- VARIABLES FOR SRTF LOGIC ----------------
    int completed = 0; // Number of processes finished
    int time = 0;      // Current CPU time
    int minIndex = -1; // Index of process with least remaining time
    int minRemaining;  // Stores smallest remaining time found in loop

    // ----------- MAIN LOOP: RUN UNTIL ALL PROCESSES COMPLETE -----------
    while (completed < n)
    {

        minRemaining = INT_MAX; // Set to largest possible number
        minIndex = -1;          // Reset process selection

        // ----------- STEP 1: FIND THE SHORTET REMAINING TIME PROCESS ---------
        for (int i = 0; i < n; i++)
        {
            // P1 0 8 7
            // P2 1 4 0
            // P3 2 9 9
            // P4 3 5 5
            // time 5
            // A process is eligible only if:
            // 1. It has arrived (arrival <= current time)
            // 2. It is not already completed (remaining > 0)
            // 3. It has the least remaining time among all eligible
            if (p[i].arrival <= time &&
                p[i].remaining > 0 &&
                p[i].remaining < minRemaining)
            {

                minRemaining = p[i].remaining; // 5
                minIndex = i;                  // 3                  // Store this process index // 1
            }
        }

        // ----------- STEP 2: IF NO PROCESS HAS ARRIVED → JUST INCREMENT TIME --------
        if (minIndex == -1)
        {
            time++;   // Move CPU time forward (idle)
            continue; // Re-check again
        }

        if (p[minIndex].burst == p[minIndex].remaining)
        {
            p[minIndex].start = time;                                           // 5
            p[minIndex].responseTime = p[minIndex].start - p[minIndex].arrival; // 0 0 2
        }

        // ----------- STEP 3: EXECUTE SELECTED PROCESS FOR 1 UNIT OF TIME --------
        p[minIndex].remaining--; // Reduce remaining time
        time++;                  // Increase global time

        // ----------- STEP 4: CHECK IF THE PROCESS HAS COMPLETED -----------
        if (p[minIndex].remaining == 0)
        {

            completed++;               // Increase completed count
            p[minIndex].finish = time; // Time at which it completed // 5

            // Turnaround time = finish time – arrival time
            p[minIndex].turnaround = p[minIndex].finish - p[minIndex].arrival; // 4

            // Waiting time = turnaround time – original burst time
            p[minIndex].waiting = p[minIndex].turnaround - p[minIndex].burst; // 0
        }
    }

    // --------------------- OUTPUT SECTION --------------------------
    printf("\nPID\tAT\tBT\tST\tFT\tWT\tTAT\tRT\n");

    // Print all process details
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