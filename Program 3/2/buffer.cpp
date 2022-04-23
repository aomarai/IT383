#include "buffer.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

//The buffer
bufferitem buffer[BUFFER_SIZE];
sem_t full, empty;
int insertIndex = 0;
pthread_mutex_t mutex;


int insert_item(bufferitem item) 
{
    //Acquire an empty semaphore
    sem_wait(&empty);

    //Acquire the lock
    pthread_mutex_lock(&mutex);

    //Insert the item at insertIndex and increment insertIndex
    buffer[insertIndex] = item;
    insertIndex = (insertIndex + 1) % BUFFER_SIZE;

    //Release the lock
    pthread_mutex_unlock(&mutex);

    //Signal the full semaphore
    sem_post(&full);
}

int remove_item(bufferitem *item) 
{
    //Acquire a full semaphore
    sem_wait(&full);

    //Acquire the lock to prevent race conditions
    pthread_mutex_lock(&mutex);

    //Remove the item and increment the removal index
    *item = buffer[insertIndex];
    buffer[insertIndex] = 0;
    insertIndex = (insertIndex - 1) % BUFFER_SIZE;

    //Release the lock
    pthread_mutex_unlock(&mutex);

    //Signal the empty semaphore
    sem_post(&empty);
}