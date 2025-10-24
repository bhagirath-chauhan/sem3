#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full, prod_lock;
pthread_mutex_t mutex;

void* producer(void* arg) {
    int id = *(int*)arg;
    int item = 0;

    while (1) {
        item++;
        sem_wait(&empty);
        sem_wait(&prod_lock);            // Lock consumers temporarily
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer %d produced item %d at position %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&prod_lock);            // Allow consumers again
        sem_post(&full);
        sleep(1);
    }
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    int item;

    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        printf("Consumer %d consumed item %d from position %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(2);
    }
}

int main() {
    pthread_t p[2], c[2];
    int p_id[2] = {1, 2};
    int c_id[2] = {1, 2};
    int i;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&prod_lock, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < 2; i++)
        pthread_create(&p[i], NULL, producer, &p_id[i]);
    for (i = 0; i < 2; i++)
        pthread_create(&c[i], NULL, consumer, &c_id[i]);

    for (i = 0; i < 2; i++)
        pthread_join(p[i], NULL);
    for (i = 0; i < 2; i++)
        pthread_join(c[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&prod_lock);
    pthread_mutex_destroy(&mutex);
    return 0;
}
