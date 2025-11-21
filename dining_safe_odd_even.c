#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5  // Number of philosophers

pthread_mutex_t chopstick[N]; // one mutex per chopstick

void* philosopher(void* num) {
    int id = *(int*)num;

    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);

        printf("Philosopher %d is hungry\n", id);

        if (id % 2 == 0) {
            // Even philosophers pick RIGHT first, then LEFT
            pthread_mutex_lock(&chopstick[(id + 1) % N]);
            printf("Philosopher %d picked up RIGHT chopstick\n", id);

            pthread_mutex_lock(&chopstick[id]);
            printf("Philosopher %d picked up LEFT chopstick and is eating\n", id);
        } else {
            // Odd philosophers pick LEFT first, then RIGHT
            pthread_mutex_lock(&chopstick[id]);
            printf("Philosopher %d picked up LEFT chopstick\n", id);

            pthread_mutex_lock(&chopstick[(id + 1) % N]);
            printf("Philosopher %d picked up RIGHT chopstick and is eating\n", id);
        }

        sleep(2); // eating time

        // Put down both chopsticks
        pthread_mutex_unlock(&chopstick[id]);
        pthread_mutex_unlock(&chopstick[(id + 1) % N]);
        printf("Philosopher %d finished eating and put down chopsticks\n", id);
    }
}

int main() {
    pthread_t thread_id[N];
    int philosopher_ids[N];

    // Initialize mutexes
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&chopstick[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        philosopher_ids[i] = i;
        pthread_create(&thread_id[i], NULL, philosopher, &philosopher_ids[i]);
        printf("Philosopher %d joined the table\n", i);
    }

    // Join threads (program will keep running)
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&chopstick[i]);
    }

    return 0;
}