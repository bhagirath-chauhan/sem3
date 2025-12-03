#include <stdio.h>  // Standard Input/Output
#include <limits.h> // For INT_MIN (very small number)

// Structure to store process information
struct Process
{
    int pid;          // Process ID
    int arrival;      // Arrival time
    int burst;        // Burst time
    int priority;     // Priority (Larger number = higher priority)
    int remaining;    // Remaining execution time
    int start;        // First time it gets CPU
    int finish;       // Finish time
    int waiting;      // Waiting time
    int turnaround;   // Turnaround time
    int responseTime; // Response time
    int startedFlag;  // To check if process has started earlier
};

int main()
{
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // --------------------- INPUT SECTION ---------------------
    for (int i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("\nProcess %d Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].arrival);

        printf("Process %d Burst Time: ", p[i].pid);
        scanf("%d", &p[i].burst);

        printf("Process %d Priority: ", p[i].pid);
        scanf("%d", &p[i].priority);

        p[i].remaining = p[i].burst; // remaining time initially = burst time
        p[i].startedFlag = 0;        // not started yet
    }

    // ------------------ VARIABLES FOR LOGIC -------------------
    int completed = 0;    // Number of completed processes
    int time = 0;         // Current time
    int selectIndex = -1; // Which process is selected
    int highestPriority;  // Highest priority found

    // ------------- MAIN LOOP: RUN UNTIL ALL COMPLETE ----------
    while (completed < n)
    {
        highestPriority = INT_MIN; // Reset to lowest value
        selectIndex = -1;

        // ----------- STEP 1: FIND HIGHEST PRIORITY PROCESS -------
        for (int i = 0; i < n; i++)
        {
            // Eligible only if:
            // 1. arrived
            // 2. remaining > 0
            // 3. priority is highest (bigger number means higher priority)
            if (p[i].arrival <= time && p[i].remaining > 0)
            {
                if (p[i].priority > highestPriority)
                {
                    highestPriority = p[i].priority;
                    selectIndex = i;
                }
            }
        }

        // If no process arrived yet → CPU idle
        if (selectIndex == -1)
        {
            time++;
            continue;
        }

        // ----------- STEP 2: SET START TIME & RESPONSE TIME -------
        if (p[selectIndex].startedFlag == 0)
        {
            p[selectIndex].start = time;
            p[selectIndex].responseTime = p[selectIndex].start - p[selectIndex].arrival;
            p[selectIndex].startedFlag = 1;
        }

        // ----------- STEP 3: EXECUTE FOR 1 TIME UNIT -------------
        p[selectIndex].remaining--;
        time++;

        // ----------- STEP 4: CHECK IF COMPLETED ------------------
        if (p[selectIndex].remaining == 0)
        {
            completed++;

            p[selectIndex].finish = time;

            // Turnaround time = finish - arrival
            p[selectIndex].turnaround = p[selectIndex].finish - p[selectIndex].arrival;

            // Waiting time = turnaround - burst
            p[selectIndex].waiting = p[selectIndex].turnaround - p[selectIndex].burst;
        }
    }

    // ---------------------- OUTPUT SECTION -----------------------
    printf("\nPID\tAT\tBT\tPR\tST\tFT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].priority,
               p[i].start,
               p[i].finish,
               p[i].waiting,
               p[i].turnaround,
               p[i].responseTime);
    }

    return 0;
}