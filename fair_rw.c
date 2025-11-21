#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t rw_mutex, mutex, queue;
int readcount = 0;
int data = 0;

void* reader(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sem_wait(&queue);               // Wait in queue (fairness)
        sem_wait(&mutex);
        readcount++;
        if (readcount == 1)
            sem_wait(&rw_mutex);        // First reader locks writers
        sem_post(&mutex);
        sem_post(&queue);               // Allow next in queue

        printf("Reader %d: read data = %d\n", id, data);
        sleep(1);

        sem_wait(&mutex);
        readcount--;
        if (readcount == 0)
            sem_post(&rw_mutex);        // Last reader releases
        sem_post(&mutex);
        sleep(1);
    }
}

void* writer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sem_wait(&queue);               // Wait in queue (fairness)
        sem_wait(&rw_mutex);            // Lock exclusive access
        sem_post(&queue);               // Let next in queue proceed

        data++;
        printf("Writer %d: wrote data = %d\n", id, data);
        sleep(1);

        sem_post(&rw_mutex);
        sleep(1);
    }
}

int main() {
    pthread_t rtid[3], wtid[2];
    int r_id[3] = {1,2,3};
    int w_id[2] = {1,2};

    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    sem_init(&queue, 0, 1);  // Ensures FIFO order
    for (int i = 0; i < 3; i++)
        pthread_create(&rtid[i], NULL, reader, &r_id[i]);
    for (int i = 0; i < 2; i++)
        pthread_create(&wtid[i], NULL, writer, &w_id[i]);

    for (int i = 0; i < 3; i++)
        pthread_join(rtid[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(wtid[i], NULL);

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    sem_destroy(&queue);

    return 0;
}