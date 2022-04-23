#include "buffer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
using namespace std;

void* produce(void* args)
{

}

void* consume(void* args)
{

}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <sleep length> <num producer threads> <num consumer threads>" << endl;
        return 1;
    }

    //Grab the arguments from the command line
    int sleepLength = atoi(argv[1]);
    int numProducerThreads = atoi(argv[2]);
    int numConsumerThreads = atoi(argv[3]);

    //Initialize the buffer
    bufferitem buffer[BUFFER_SIZE];
}