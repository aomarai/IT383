#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#define BUFFER_SIZE 5

struct threadArgs
{
    pthread_t tid;
    int *counter;
    int *buffer;
    int in;
    int out;
};

void *produce(void *args)
{
    int nextProduced = 1;
    bool producing = true;
    threadArgs threadData = *(threadArgs *)args;

    // If nextProduced is over one million, stop producing
    while (nextProduced < 1000000)
    {
        // While the buffer is full, do nothing
        while (*(threadData.counter) == BUFFER_SIZE)
        {
            ; // Do nothing
        }
        threadData.buffer[threadData.in] = nextProduced;
        threadData.in = (threadData.in + 1) % BUFFER_SIZE;
        threadData.counter++;
        nextProduced++;
    }

    // All values have been inserted, so exit thread
    pthread_exit(NULL);
}

void *consume(void *args)
{
    int nextConsumed;
    threadArgs threadData = *(threadArgs *)args;
    while (true)
    {
        while (*(threadData.counter) == 0)
        {
            ; // Do nothing
        }
        // Read the next item to be consumed
        nextConsumed = threadData.buffer[threadData.out];
        // Check if nextConsumed's previous value is the integer - 1 and nextConsumed's next is + 1
        if (nextConsumed == *(threadData.counter) - 1 && threadData.buffer[(threadData.out + 1) % BUFFER_SIZE] == nextConsumed + 1)
        {
            threadData.out = (threadData.out + 1) % BUFFER_SIZE;
            threadData.counter--;
        }
        // If not sequential, then print the next number in the buffer and print percentage of the sequence completed
        else
        {
            printf("%d\n", nextConsumed);
            printf("%f\n", (float)(*(threadData.counter) - 1) / (float)nextConsumed * 100);
        }
    }
}

int main()
{
    int *counter = 0;
    int in = 0;
    int out = 0;
    int *circBuffer = new int[BUFFER_SIZE];
    pthread_t tid;
    pthread_attr_t attr;

    // Create a producer thread
    pthread_attr_init(&attr);
    threadArgs *producerArgs = new threadArgs;
    producerArgs->counter = counter;
    producerArgs->buffer = circBuffer;
    producerArgs->in = in;
    producerArgs->out = out;
    pthread_create(&tid, &attr, produce, (void *)producerArgs);
}
