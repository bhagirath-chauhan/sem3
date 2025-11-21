#include <stdio.h>

int findOptimal(int page[], int frame[], int pages, int currentIndex, int frames) {
    int i, j;
    int farthest = -1;
    int pos = -1;

    // For each frame, find when its page will next be used
    for (i = 0; i < frames; ++i) {
        int nextIndex = -1; // -1 means not used again (best candidate)
        for (j = currentIndex + 1; j < pages; ++j) {
            if (frame[i] == page[j]) {
                nextIndex = j;
                break;
            }
        }

        // If a page is never used again, we can replace it immediately
        if (nextIndex == -1) {
            return i; // best choice: not used in future
        }

        // Otherwise choose the one with the farthest next use
        if (nextIndex > farthest) {
            farthest = nextIndex;
            pos = i;
        }
    }

    // pos holds index of frame whose next use is farthest in future
    return pos;
}

int main() {
    int frames, pages;
    int frame[10], page[30];
    int i, j, k;
    int flag1, flag2, pos;
    int faults = 0;

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter number of pages: ");
    scanf("%d", &pages);

    printf("Enter page reference string:\n");
    for (i = 0; i < pages; ++i)
        scanf("%d", &page[i]);

    // initialize frames as empty
    for (i = 0; i < frames; ++i)
        frame[i] = -1;

    // process each page reference
    for (i = 0; i < pages; ++i) {
        flag1 = flag2 = 0;

        // check if page is already in some frame (hit)
        for (j = 0; j < frames; ++j) {
            if (frame[j] == page[i]) {
                flag1 = flag2 = 1; // hit
                break;
            }
        }

        // if not a hit, try to place in empty frame
        if (flag1 == 0) {
            for (j = 0; j < frames; ++j) {
                if (frame[j] == -1) {
                    faults++;
                    frame[j] = page[i];
                    flag2 = 1;
                    break;
                }
            }
        }

        // page = 7, 0, 1, 2, 0, 3, 0, 4
        // frames =  [-1, -1, -1]
        // if page not placed yet, replace using Optimal strategy
        if (flag2 == 0) {
            pos = findOptimal(page, frame, pages, i, frames);
            faults++;
            frame[pos] = page[i];
        }

        // print current frames after this reference
        printf("\nPage %d -> ", page[i]);
        for (k = 0; k < frames; ++k)
            printf("%d ", frame[k]);
    }

    printf("\n\nTotal Page Faults = %d\n", faults);
    return 0;
}