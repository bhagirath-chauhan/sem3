#include <stdio.h>      // For printf(), perror()
#include <stdlib.h>     // For general utilities
#include <pthread.h>    // For POSIX threads (pthread_* functions)

// -------------------------------
// CONSTANT DEFINITIONS
// -------------------------------

#define NTHREADS 10        // Total number of threads to create
#define NINCREMENTS 100000 // How many times each thread will increment the counter

// -------------------------------
// GLOBAL VARIABLES
// -------------------------------

int counter = 0;           // Shared variable accessed by all threads
pthread_mutex_t lock;      // Mutex variable to protect 'counter'

// -------------------------------
// THREAD FUNCTION
// -------------------------------

// This is the function that each thread will execute.
void* increment_counter(void* arg) {
    int i;

    for (i = 0; i < NINCREMENTS; i++) {
        // ---- CRITICAL SECTION START ----
        pthread_mutex_lock(&lock);   // Lock the mutex before updating shared variable
        counter++;                   // Safely increment shared counter
        pthread_mutex_unlock(&lock); // Unlock so other threads can continue
        // ---- CRITICAL SECTION END ----
    }

    return NULL;  // Return value not needed, so return NULL
}

// -------------------------------
// MAIN FUNCTION
// -------------------------------

int main() {
    pthread_t threads[NTHREADS];  // Array to store thread IDs (handles)
    int i;

    // Initialize the mutex before use (NULL = use default attributes)
    if (pthread_mutex_init(&lock, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    printf("Starting threads...\n");

    // -------------------------------
    // CREATE THREADS
    // -------------------------------
    for (i = 0; i < NTHREADS; i++) {

        // pthread_create() creates a new thread
        // ┌────────────────────────────────────────────────────────────────────────────┐
        // │ int pthread_create(                                                       │
        // │     pthread_t *thread,        → Pointer where new thread ID will be stored │
        // │     const pthread_attr_t *attr, → Thread attributes (NULL = default)       │
        // │     void *(*start_routine)(void *), → Function thread will run             │
        // │     void *arg                → Argument passed to that function (NULL here)│
        // │ );                                                                       │
        // └────────────────────────────────────────────────────────────────────────────┘

        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

    // -------------------------------
    // WAIT (JOIN) FOR THREADS TO FINISH
    // -------------------------------
    for (i = 0; i < NTHREADS; i++) {

        // pthread_join() waits for a specific thread to finish
        // ┌────────────────────────────────────────────────────────────┐
        // │ int pthread_join(                                         │
        // │     pthread_t thread,   → Thread ID to wait for           │
        // │     void **retval       → Location to store thread's return value │
        // │ );                                                         │
        // └────────────────────────────────────────────────────────────┘
        //
        // Here:
        // - threads[i] is the thread ID we want to wait for.
        // - NULL means we don’t care about the thread’s return value.
        //
        // This ensures all threads finish before continuing to print result.

        pthread_join(threads[i], NULL);
    }

    // All threads are done — print the result
    printf("Final counter value: %d\n", counter);

    // Destroy the mutex to free system resources
    pthread_mutex_destroy(&lock);

    return 0;
}