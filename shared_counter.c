// shared_counter.c
#include <stdio.h> // for printf(), perror()
#include <stdlib.h> // for exit()
#include <sys/mman.h>  // for mmap()
#include <sys/wait.h>  // for wait()
#include <unistd.h>    // for fork(), _exit()

int main() {
    int nprocs = 5;   // create 5 child processes

    // Step 1: Create shared memory for counter variable
    int *counter = mmap(NULL, sizeof(int),
                        PROT_READ | PROT_WRITE,     // read + write
                        MAP_SHARED | MAP_ANONYMOUS, // shared among processes
                        -1, 0);

    int i;

    if (counter == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    *counter = 0; // initialize shared counter

    printf("Parent start: counter = %d\n", *counter);

    // Step 2: Create multiple child processes
    for (i = 0; i < nprocs; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            // ---- Child process ----
            (*counter)++; // increment the shared variable
            printf("Child %d (PID=%d): counter = %d\n", i + 1, getpid(), *counter);

            _exit(0); // end child
        }
        // Parent continues to create next child
    }

    // Step 3: Parent waits for all children to finish
    for (i = 0; i < nprocs; i++)
        wait(NULL);

    // Step 4: Show final shared counter value
    printf("Parent end: counter = %d\n", *counter);

    return 0;
}