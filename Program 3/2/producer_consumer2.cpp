#include "buffer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

struct threadArgs
{
    pthread_t tid;
    pthread_mutex_t *mutex;
    sem_t *full;
    sem_t *empty;
};

void *produce(void *args)
{
}

void *consume(void *args)
{
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <sleep length> <num producer threads> <num consumer threads>" << endl;
        return 1;
    }

    pthread_mutex_t mutex;
    sem_t full, empty;
    pthread_mutex_t *mutexPtr = &mutex;
    sem_t *fullPtr = &full;
    sem_t *emptyPtr = &empty;

    // Grab the arguments from the command line
    int sleepLength = atoi(argv[1]);
    int numProducerThreads = atoi(argv[2]);
    int numConsumerThreads = atoi(argv[3]);

    // Initialize the buffer
    bufferitem buffer[BUFFER_SIZE];

    // Initialize locks
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
}