#include <stdio.h>
#include <stdbool.h>

int main() {
    int n = 5; // number of processes
    int m = 3; // number of resources

    int alloc[5][3] = {
        {0, 1, 0}, // P0
        {2, 0, 0}, // P1
        {3, 0, 2}, // P2
        {2, 1, 1}, // P3
        {0, 0, 2}  // P4
    };

    int max[5][3] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int avail[3] = {3, 3, 2}; // Available instances

    int need[5][3];
    int finish[5] = {0};
    int safeSeq[5];
    int work[3];

    // Step 1: Calculate Need matrix
    printf("Calculating NEED matrix = MAX - ALLOC:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
        printf("P%d Need: [%d %d %d]\n", i, need[i][0], need[i][1], need[i][2]);
    }

    // Step 2: Initialize Work = Available
    for (int j = 0; j < m; j++) {
        work[j] = avail[j];
    }

    printf("\nInitial Available resources: [%d %d %d]\n", work[0], work[1], work[2]);

//    return 0;

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    printf("\nProcess P%d can be allocated (Need <= Available)\n", i);
                    printf("Before allocation: Available = [%d %d %d]\n", work[0], work[1], work[2]);

                    for (int k = 0; k < m; k++) {
                        work[k] += alloc[i][k];
                    }
                   
// P1 P3
//                      3 3 2
//                      2 0 0
//                      5 3 2
//                      2 1 1
//                      7 4 3
//                      0 0 2
//                      7 4 5 
//                      0 1 0
//                      7 5 5
//                      3 0 2
//                      10 5 7    
                        

                    printf("After releasing P%d resources: Available = [%d %d %d]\n",
                           i, work[0], work[1], work[2]);

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("\nSystem is NOT in a safe state!\n");
            return 0;
        }
    }

    printf("\nSystem is in a SAFE STATE.\nSafe Sequence: ");
    for (int i = 0; i < n; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");

    return 0;
}