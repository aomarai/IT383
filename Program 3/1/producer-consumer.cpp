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
    int nextProduced;
    threadArgs threadData = *(threadArgs *)args;
    // Produce an item in nextProduced by adding 1 to the counter value
    nextProduced = *(threadData.counter) + 1;

    while (true)
    {
        // Compare the counter against the buffer size
        while (*(threadData.counter) == BUFFER_SIZE)
        {
            ; // Do nothing
        }
        threadData.buffer[threadData.in] = nextProduced;
        threadData.in = (threadData.in + 1) % BUFFER_SIZE;
        threadData.counter++;
    }
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
        nextConsumed = threadData.buffer[threadData.out];
        threadData.out = (threadData.out + 1) % BUFFER_SIZE;
        threadData.counter--;

        // Consume the item in nextConsumed
    }
}

int main()
{
    int *counter = 0;
    int in = 0;
    int out = 0;
    int nextProduced = 0;
    int *circBuffer = new int[BUFFER_SIZE];
    pthread_t tid;
    pthread_attr_t attr;
}
