#include <stdio.h>

int findLRU(int time[], int n) {
    int i, minimum = time[0], pos = 0;

    for (i = 1; i < n; ++i) {
        if (time[i] < minimum) {
            minimum = time[i];
            pos = i;
        }
    }
    return pos;
}

int main() {
    int frames, pages, frame[10], page[30], time[10];
    int counter = 0, flag1, flag2, i, j, pos,  faults = 0;

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter number of pages: ");
    scanf("%d", &pages);

    printf("Enter page reference string:\n");
    for (i = 0; i < pages; ++i)
        scanf("%d", &page[i]);

    for (i = 0; i < frames; ++i)
        frame[i] = -1;

    for (i = 0; i < pages; ++i) {
        flag1 = flag2 = 0;

        // Check if page already in frame
        // 7, 0, 1, 2, 0, 3, 0, 4
        for (j = 0; j < frames; ++j) {
            if (frame[j] == page[i]) {
                counter++;
                time[j] = counter;  // update last used time
                flag1 = flag2 = 1;
                break;
            }
        }

        // If page not in frame
        if (flag1 == 0) {
            for (j = 0; j < frames; ++j) {
                if (frame[j] == -1) { // empty frame
                    faults++;
                    counter++;
                    frame[j] = page[i];
                    time[j] = counter;
                    flag2 = 1;
                    break;
                }
            }
        }

        // If no empty frame → replace LRU page
        if (flag2 == 0) {
            pos = findLRU(time, frames);
            faults++;
            counter++;
            frame[pos] = page[i];
            time[pos] = counter;
        }

        printf("\nPage %d -> ", page[i]);
        for (j = 0; j < frames; ++j)
            printf("%d ", frame[j]);
    }

    printf("\n\nTotal Page Faults = %d\n", faults);
    return 0;
}