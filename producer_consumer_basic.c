    #include <stdio.h>      // For printf()
    #include <pthread.h>    // For pthread functions (threads)
    #include <semaphore.h>  // For semaphore functions
    #include <unistd.h>     // For sleep() function

    // Define the max imum size of buffer (shared memory)
    #define BUFFER_SIZE 5

    // Shared buffer and indexes for circular queue
    int buffer[BUFFER_SIZE];
    int in = 0, out = 0;

    // Declare semaphores and mutex
    sem_t empty;   // Counts empty slots
    sem_t full;    // Counts filled slots
    pthread_mutex_t mutex;  // Ensures mutual exclusion

    // ---------------------- PRODUCER FUNCTION ----------------------
    void* producer(void* arg) {
        int id = *(int*)arg;   // Producer ID passed as argument
        int item = 0;          // Item to produce

        while (1) {
            item++;  // Produce a new item (simulated by incrementing a number)

            // ↓ Wait if there are no empty slots (i.e., buffer is full)
            sem_wait(&empty);

            // ↓ Lock the critical section (only one thread can access buffer)
            pthread_mutex_lock(&mutex);

            // Produce the item and place it into buffer
            buffer[in] = item;
            printf("Producer %d produced item %d at position %d\n", id, item, in);

            // Move the 'in' pointer forward (circularly)
            in = (in + 1) % BUFFER_SIZE;

            // ↓ Unlock the buffer (other threads can now access)
            pthread_mutex_unlock(&mutex);

            // ↓ Signal that there is one more full slot available
            sem_post(&full);

            // Sleep to simulate production time
            sleep(1);
        }
    }

    // ---------------------- CONSUMER FUNCTION ----------------------
    void* consumer(void* arg) {
        int id = *(int*)arg;   // Consumer ID passed as argument
        int item;              // Item to consume

        while (1) {
            // ↓ Wait if there are no full slots (i.e., buffer is empty)
            sem_wait(&full);

            // ↓ Lock the buffer for exclusive access
            pthread_mutex_lock(&mutex);

            // Consume the item from buffer
            item = buffer[out];
            printf("Consumer %d consumed item %d from position %d\n", id, item, out);

            // Move the 'out' pointer forward (circularly)
            out = (out + 1) % BUFFER_SIZE;

            // ↓ Unlock the buffer after consuming
            pthread_mutex_unlock(&mutex);

            // ↓ Signal that one more empty slot is now available
            sem_post(&empty);

            // Sleep to simulate consumption time
            sleep(2);
        }
    }

    // ---------------------- MAIN FUNCTION ----------------------
    int main() {
        pthread_t p[2], c[2];   // Thread handles for 2 producers and 2 consumers
        int p_id[2] = {1, 2};   // IDs for producers
        int c_id[2] = {1, 2};   // IDs for consumers
        int i;

        // Initialize semaphores
        // 'empty' starts with BUFFER_SIZE (all slots empty)
        sem_init(&empty, 0, BUFFER_SIZE);
        // 'full' starts with 0 (no items produced yet)
        sem_init(&full, 0, 0);

        // Initialize the mutex with default attributes
        pthread_mutex_init(&mutex, NULL);

        // Create producer threads
        for (i = 0; i < 2; i++) {
            pthread_create(&p[i], NULL, producer, &p_id[i]);
        }

        // Create consumer threads
        for (i = 0; i < 2; i++) {
            pthread_create(&c[i], NULL, consumer, &c_id[i]);
        }

        // Wait (join) for all threads to finish (infinite loop, so never ends)
        for (i = 0; i < 2; i++) {
            pthread_join(p[i], NULL);
        }
        for (i = 0; i < 2; i++) {
            pthread_join(c[i], NULL);
        }

        // Destroy semaphores and mutex (won’t reach here due to infinite loop)
        sem_destroy(&empty);
        sem_destroy(&full);
        pthread_mutex_destroy(&mutex);

        return 0;
    }