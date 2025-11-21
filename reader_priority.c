#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt;           // Used by writers to get exclusive access
pthread_mutex_t mutex; 
int readcount = 0;   // Number of readers currently reading
int data = 0;        // Shared resource

void* reader(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);      // Entry section
        readcount++;
        if (readcount == 1)
            sem_wait(&wrt);             // First reader locks writers
        pthread_mutex_unlock(&mutex);

        // Critical section
        printf("Reader %d: read data = %d\n", id, data);
        sleep(1);

        // Exit section
        pthread_mutex_lock(&mutex);
        readcount--;
        if (readcount == 0)
            sem_post(&wrt);             // Last reader unlocks writers
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

void* writer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sem_wait(&wrt);                 // Lock exclusive access
        data++;                         // Writing (modifying shared data)
        printf("Writer %d: wrote data = %d\n", id, data);
        sleep(1);
        sem_post(&wrt);                 // Release access
        sleep(2);
    }
}

int main() {
    pthread_t rtid[3], wtid[2];
    int r_id[3] = {1,2,3};
    int w_id[2] = {1,2};

    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 3; i++)
        pthread_create(&rtid[i], NULL, reader, &r_id[i]);
    for (int i = 0; i < 2; i++)
        pthread_create(&wtid[i], NULL, writer, &w_id[i]);

    for (int i = 0; i < 3; i++)
        pthread_join(rtid[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(wtid[i], NULL);

    sem_destroy(&wrt);
    pthread_mutex_destroy(&mutex);

    return 0;
}
