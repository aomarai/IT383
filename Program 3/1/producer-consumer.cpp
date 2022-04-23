#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <atomic>

#define BUFFER_SIZE 5
#define SEQ_MAX 1000000
bool terminationDetected = false;

struct threadArgs
{
    pthread_t tid;
    int *counter;
    int *buffer;
};

void *produce(void *args)
{
    // printf("Producer thread %lu\n", ((threadArgs *)args)->tid);
    int nextProduced = 1;
    int in = 0;
    int *counter = ((threadArgs *)args)->counter;
    int *buffer = ((threadArgs *)args)->buffer;

    while (nextProduced <= SEQ_MAX && !terminationDetected)
    {
        while (*counter == BUFFER_SIZE && !terminationDetected)
        {
            // Wait for consumer to consume by doing nothing
            ;
        }
        //printf("Producer is producing %d\n", nextProduced);
        buffer[in] = nextProduced;
        in = (in + 1) % BUFFER_SIZE;
        (*counter)++;
        nextProduced++;
    }
    //printf("Producer thread successfully produced %d items\n", nextProduced - 1);
    pthread_exit(NULL);
}

void *consume(void *args)
{
    // printf("Consumer thread %lu\n", ((threadArgs *)args)->tid);
    int nextConsumed = 0;
    int out = 0;
    int *buffer = ((threadArgs *)args)->buffer;
    int *counter = ((threadArgs *)args)->counter;

    while (nextConsumed <= SEQ_MAX - 1 && !terminationDetected)
    {
        //printf("Consumer counter is %d\n", *counter);
        while (*counter == 0)
        {
            // Do nothing
            ;
        }

        // Check if the next item in the buffer is nextConsumed + 1
        if (nextConsumed + 1 != buffer[out])
        {
            printf("The consumer received up to %d, but the next incorrect item in the buffer is %d\n", nextConsumed, buffer[out]);
            terminationDetected = true;
            pthread_exit(NULL);
        }

        //printf("Consumed %d\n", buffer[out]);
        nextConsumed = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        (*counter)--;
    }
    printf("Consumer thread successfully received all %d integers\n", SEQ_MAX);
    pthread_exit(NULL);
}

int main()
{
    int counter = 0;
    int *ctPtr = &counter;
    int *circBuffer = new int[BUFFER_SIZE];
    pthread_t tid = 0;
    pthread_attr_t attr;

    // Create producer and consumer threads
    pthread_attr_init(&attr);
    threadArgs *producerArgs = new threadArgs;
    threadArgs *consumerArgs = new threadArgs;
    producerArgs->counter = consumerArgs->counter = ctPtr;
    producerArgs->buffer = consumerArgs->buffer = circBuffer;
    producerArgs->tid = tid;
    consumerArgs->tid = tid++;

    // Create producer thread
    pthread_create(&producerArgs->tid, &attr, produce, (void *)producerArgs);
    pthread_create(&consumerArgs->tid, &attr, consume, (void *)consumerArgs);

    // Wait for producer and consumer threads to finish
    pthread_join(producerArgs->tid, NULL);
    pthread_join(consumerArgs->tid, NULL);
    //printf("Producer and consumer threads finished successfully.\n");

    return 0;
}
