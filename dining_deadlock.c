#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5  // Number of philosophers

pthread_mutex_t chopstick[N]; // one mutex per chopstick

void* philosopher(void* num) {
    int id = *(int*)num;

    while (1) {
        printf("Philosopher %d is thinking \n", id);
        sleep(1);

        printf("Philosopher %d is hungry \n", id);

        // Pick up left chopstick
        pthread_mutex_lock(&chopstick[id]);
        printf("Philosopher %d picked up LEFT chopstick \n", id);

        // Simulate small delay — increases chance of deadlock
        sleep(1);

        // Pick up right chopstick
        pthread_mutex_lock(&chopstick[(id + 1) % N]);
        printf("Philosopher %d picked up RIGHT chopstick  and is eating \n", id);

        sleep(2); // eating time

        // Put down both chopsticks
        pthread_mutex_unlock(&chopstick[(id + 1) % N]);
        pthread_mutex_unlock(&chopstick[id]);
        printf("Philosopher %d finished eating and put down chopsticks\n", id);
    }
}

int main() {
    pthread_t thread_id[N];
    int philosopher_ids[N];

    // Initialize mutexes (chopsticks)
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&chopstick[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        philosopher_ids[i] = i;
        pthread_create(&thread_id[i], NULL, philosopher, &philosopher_ids[i]);
        printf("Philosopher %d joined the table\n", i);
    }

    // Join threads (never actually ends)
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Destroy mutexes
}