#include "buffer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <unistd.h>
using namespace std;

struct threadArgs
{
    pthread_t tid;
    pthread_mutex_t *mutex;
    sem_t *full;
    sem_t *empty;
    vector<string> *words;
};

void *produce(void *args)
{
    string wordString;
    bufferitem randWord;

    // Infinite loop while thread runs
    while (true)
    {
        // Sleep for a random time between 0 and 5 seconds
        sleep(rand() % 5);

        // Insert a random word from words vector inside threadArgs struct
        wordString = (*((threadArgs *)args)->words)[rand() % (*((threadArgs *)args)->words).size()];
        printf("Producer %lu is inserting %s\n", ((threadArgs *)args)->tid, wordString.c_str());
        randWord = atoi(wordString.c_str());
        insert_item(randWord);
    }
}

void *consume(void *args)
{
    bufferitem randWord;

    // Infinite loop while thread runs
    while (true)
    {
        // Sleep for a random time between 0 and 5 seconds
        sleep(rand() % 5);

        //Print the removed word and thread id
        printf("Consumer %lu removed %d\n", ((threadArgs *)args)->tid, remove_item(&randWord));
        
        // Remove a random word from buffer inside threadArgs struct
        remove_item(&randWord);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <sleep length> <num producer threads> <num consumer threads>" << endl;
        return 1;
    }

    pthread_attr_t attr;
    pthread_mutex_t mutex;
    sem_t full, empty;
    pthread_mutex_t *mutexPtr = &mutex;
    sem_t *fullPtr = &full;
    sem_t *emptyPtr = &empty;
    string filename = "wordsEn.txt";
    ifstream file(filename);
    vector<string> *words = new vector<string>();

    srand(time(NULL));

    // Read the file line by line into the words vector
    string line;
    while (getline(file, line))
    {
        words->push_back(line);
    }

    // Grab the arguments from the command line
    int sleepLength = atoi(argv[1]);
    int numProducerThreads = atoi(argv[2]);
    int numConsumerThreads = atoi(argv[3]);

    // Initialize the buffer
    bufferitem buffer[BUFFER_SIZE];

    // Initialize locks
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Create producer threads
    threadArgs *producerArgs = new threadArgs[numProducerThreads];
    for (pthread_t i = 0; i < numProducerThreads; i++)
    {
        producerArgs[i].tid = i;
        producerArgs[i].mutex = mutexPtr;
        producerArgs[i].full = fullPtr;
        producerArgs[i].empty = emptyPtr;
        producerArgs[i].words = words;
        pthread_create(&producerArgs[i].tid, &attr, produce, (void *)&producerArgs[i]);
        printf("Producer %lu created\n", producerArgs[i].tid);
    }

    // Create consumer threads
    threadArgs *consumerArgs = new threadArgs[numConsumerThreads];
    for (pthread_t i = 0; i < numConsumerThreads; i++)
    {
        consumerArgs[i].tid = i;
        consumerArgs[i].mutex = mutexPtr;
        consumerArgs[i].full = fullPtr;
        consumerArgs[i].empty = emptyPtr;
        consumerArgs[i].words = words;
        pthread_create(&consumerArgs[i].tid, &attr, consume, (void *)&consumerArgs[i]);
        printf("Consumer %lu created\n", consumerArgs[i].tid);
    }

    // Sleep the main thread for the specified length in seconds
    sleep(sleepLength);

    // Forcefully terminate all threads
    for (int i = 0; i < numProducerThreads; i++)
    {
        pthread_cancel(producerArgs[i].tid);
    }

    for (int i = 0; i < numConsumerThreads; i++)
    {
        pthread_cancel(consumerArgs[i].tid);
    }

    // Terminate main thread
    exit(0);
}