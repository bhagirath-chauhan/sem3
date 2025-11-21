#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt, readTry;      // wrt: exclusive writer lock, readTry: blocks new readers if writer waiting
pthread_mutex_t rmutex;
int readcount = 0;
int data = 0;

void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&readTry);             // Try to read only if no writer waiting
        pthread_mutex_lock(&rmutex);
        readcount++;
        if (readcount == 1)
            sem_wait(&wrt);             // First reader locks writers
        pthread_mutex_unlock(&rmutex);
        sem_post(&readTry);

        printf("Reader %d: read data = %d\n", id, data);
        sleep(1);

        pthread_mutex_lock(&rmutex);
        readcount--;
        if (readcount == 0)
            sem_post(&wrt);
        pthread_mutex_unlock(&rmutex);

        sleep(1);
    }
}

void* writer(void* arg) {
    int id = *(int*)arg;
     while (1) {
        sem_wait(&readTry);             // Prevent new readers from entering
        sem_wait(&wrt);                 // Lock exclusive access
        data++;
        printf("Writer %d: wrote data = %d\n", id, data);
        sleep(1);
        sem_post(&wrt);
        sem_post(&readTry);             // Allow readers again
        sleep(1);
    }
}

int main() {
    pthread_t rtid[3], wtid[2];
    int r_id[3] = {1,2,3};
    int w_id[2] = {1,2};

    sem_init(&wrt, 0, 1);
    sem_init(&readTry, 0, 1);
    pthread_mutex_init(&rmutex, NULL);

    for (int i = 0; i < 3; i++)
        pthread_create(&rtid[i], NULL, reader, &r_id[i]);
    for (int i = 0; i < 2; i++)
        pthread_create(&wtid[i], NULL, writer, &w_id[i]);

    for (int i = 0; i < 3; i++)
        pthread_join(rtid[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(wtid[i], NULL);

    sem_destroy(&wrt);
    sem_destroy(&readTry);
    pthread_mutex_destroy(&rmutex);

    return 0;
}
