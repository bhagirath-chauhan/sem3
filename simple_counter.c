// simple_counter.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int counter = 0;  // normal variable in parent process
    int nprocs = 5;   // create 5 child processes
    int i;
    printf("Parent start: counter = %d\n", counter);

    for (i = 0; i < nprocs; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            // ---- Child process ----
            counter = counter + 1;  // increment local copy
            printf("Child %d (PID=%d): counter = %d\n", i + 1, getpid(), counter);
            _exit(0);  // terminate child immediately
        }
        // parent continues loop to create next child
    }

    // Wait for all child processes to finish
    for (i = 0; i < nprocs; i++)
        wait(NULL);

    printf("Parent end: counter = %d\n", counter);
    return 0;
}