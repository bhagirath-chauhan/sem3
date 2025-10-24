// shared_counter_semaphore.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

int main() {
    int nprocs = 5; // number of child processes
    int i;

    // Step 1: Create shared memory for counter
    int *counter = mmap(NULL, sizeof(int),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS,
                        -1, 0);

    if (counter == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    *counter = 0; // initialize counter

    // Step 2: Create shared semaphore
    sem_t *sem = mmap(NULL, sizeof(sem_t),
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS,
                      -1, 0);

    if (sem == MAP_FAILED) {
        perror("mmap failed for semaphore");
        exit(1);
    }

    sem_init(sem, 1, 1); // 1 = shared between processes, initial value 1

    printf("Parent start: counter = %d\n", *counter);

    // Step 3: Create child processes
    for (i = 0; i < nprocs; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork failed"); exit(1); }

        if (pid == 0) { // child
            sem_wait(sem);      // enter critical section
            (*counter)++;       // safely increment shared counter
            printf("Child %d (PID=%d): counter = %d\n", i+1, getpid(), *counter);
            sem_post(sem);      // leave critical section
            _exit(0);
        }
    }

    // Step 4: Parent waits for children
    for (i = 0; i < nprocs; i++)
        wait(NULL);

    printf("Parent end: counter = %d\n", *counter);

    // Step 5: Destroy semaphore (optional for anonymous mmap)
    sem_destroy(sem);

    return 0;
}
