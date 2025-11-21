#include <stdio.h>   // Standard input/output library

int main() {
    // Declare arrays and variables
    int pages[30];     // Can hold up to 30 page references
    int frames[10];    // Can hold up to 10 frames in memory
    int n, f;          // n = number of pages, f = number of frames
    int i, j, k;       // Loop counters
    int flag;          // Used to check if page already exists in frames
    int pageFault = 0; // Count of total page faults

    // -------------------------------
    // Step 1: Input number of pages
    // -------------------------------
    printf("Enter number of pages (max 30): ");
    scanf("%d", &n);

    // Validate input for page count
    if (n <= 0 || n > 30) {
        printf("Error: Number of pages must be between 1 and 30.\n");
        return 1;  // Exit the program early
    }

    // -------------------------------
    // Step 2: Input page reference string
    // -------------------------------
    printf("Enter page reference string: ");
    for (i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    // -------------------------------
    // Step 3: Input number of frames
    // -------------------------------
    printf("Enter number of frames (max 10): ");
    scanf("%d", &f);

    // Validate input for frame count
    if (f <= 0 || f > 10) {
        printf("Error: Number of frames must be between 1 and 10.\n");
        return 1;  // Exit the program early
    }

    // -------------------------------
    // Step 4: Initialize frames to -1 (empty)
    // -------------------------------
    for (i = 0; i < f; i++)
        frames[i] = -1;

    j = 0; // j tracks which frame will be replaced next (FIFO order)

    // -------------------------------
    // Step 5: Process each page one by one
    // -------------------------------
    // 7, 0, 1, 2, 0, 3, 0, 4
    // j = 0
    for (i = 0; i < n; i++) {
        flag = 0;  // Reset flag for each page

        // Check if the current page already exists in any frame
        // 7, 0, 1, 2, 0, 3, 0, 4
        for (k = 0; k < f; k++) {
            if (frames[k] == pages[i]) {
                flag = 1; // Page found — no page fault
                break;
            }
        }

        // -------------------------------
        // Step 6: If page not found → page fault
        // -------------------------------
        if (flag == 0) {
            frames[j] = pages[i];         // Replace the oldest page
            j = (j + 1) % f;              // Move to next frame (FIFO circular order)
            pageFault++;                  // Increase page fault count
        }

        // -------------------------------
        // Step 7: Display current frame state
        // -------------------------------
        printf("\nFor page %d: ", pages[i]);
        for (k = 0; k < f; k++) {
            if (frames[k] != -1)
                printf("%d ", frames[k]); // Display page number
            else
                printf("- ");              // Display empty slot
        }
    }

    // -------------------------------
    // Step 8: Display final result
    // -------------------------------
    printf("\n\nTotal Page Faults = %d\n", pageFault);

    return 0; // End of program
}
