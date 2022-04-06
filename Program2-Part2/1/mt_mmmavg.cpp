#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

int calculateMedian(vector<int> *dataHeap);
int calculateMode(vector<int> *dataHeap);
void *sortSubarray(void *args);
int calculateMean(vector<int> *dataHeap);
int comparator(const void *p1, const void *p2);

struct thread_data 
{
    pthread_t tid;
    vector<int> *threadHeap;
    int threadSubBegin, threadSubEnd;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: number_of_threads inputfile" << endl;
        exit(1);
    }

    int numInts;
    int numThreads = stoi(argv[1]);
    int localN, localBegin, localEnd;
    string line;
    pthread_t *threads = new pthread_t[numThreads];
    pthread_attr_t attr;
    pthread_t tid;
    string inputFile = argv[2];
    ifstream inputStream(inputFile);
    vector<int> *dataHeap;
    
    if (inputStream.is_open())
    {
        getline(inputStream, line);

        //Grab the number of integers
        numInts = stoi(line.substr(2));
        dataHeap = new vector<int>[numInts];

        //Parse the remaining integers in the file and insert into vector
        while (getline(inputStream, line))
        {
            dataHeap->push_back(stoi(line));
        }
    }

    //Close the input stream for reading
    inputStream.close();

    //Split the vector into subarrays by the number of threads
    thread_data *threadData = new thread_data[numThreads];
    localN = numInts / numThreads;
    localBegin = 0;
    localEnd = localBegin + localN;

    //Initialize and set thread attributes
    pthread_attr_init(&attr);

    //Create threads from numThreads
    for (int i = 0; i < numThreads; i++)
    {
        cout << "Creating thread " << i << endl;
        threadData[i].threadSubBegin = localBegin;
        threadData[i].threadSubEnd = localEnd;
        threadData[i].tid = i;
        threadData[i].threadHeap = dataHeap;
        pthread_create(&threads[i], &attr, sortSubarray, &threadData[i]);
        localBegin = localEnd;
        localEnd = (localEnd + localN < numInts ? localEnd + localN : numInts);
    }

    //Join threads after completion and grab required indexes
    vector<int> *returnIndices;
    void *temp = NULL;
    for (int i = 0; i < numThreads; i++)
    {
        cout << "Joining thread " << i << endl;
        pthread_join(threads[i],  NULL);
    }

    //Print out indices for testing
    // for (int i = 0; i < returnIndices->size(); i++)
    // {
    //     cout << returnIndices->at(i) << endl;
    // }

    //Merge the subarrays into a new array

    //Test prints
    cout << "\nDEBUG STATS\n--------------------------------\nHeap size: " << dataHeap->size() << endl;
    for (int i = 0; i < 100; i++)
    {
        cout << dataHeap->at(i) << " ";
    }
    cout << "\nHeap median: " << calculateMedian(dataHeap) << endl;
    cout << "Heap mean: " << calculateMean(dataHeap) << endl;
    cout << "Heap mode: " << calculateMode(dataHeap) << endl;
    cout << "# Threads: " << numThreads << endl;

    //Remove memory leaks
    delete[](threads);
    delete[](threadData);
    delete[](dataHeap);

    return 0;
}

int comparator(const void *p1, const void *p2)
{
    return(*(int*)p1 - *(int*)p2);
}

//Each thread will call this function to sort their own subarray with heapsort
void *sortSubarray (void *args)
{
    thread_data threadData = *(thread_data*) args;

    printf("\nThread ID: %lu\nThread subarray begin: %d\nThread subarray end: %d\n\n", threadData.tid, threadData.threadSubBegin, threadData.threadSubEnd);

    //Sort the thread's subarray
    sort_heap((threadData.threadHeap->begin() + threadData.threadSubBegin),
     (threadData.threadHeap->begin() + threadData.threadSubEnd));

    printf("Thread %lu has executed\n", threadData.tid);
    
    //Return the beginning index so we know how to merge arrays
    //return (void*) threadData.threadSubBegin;
}

int calculateMedian(vector<int> *dataHeap)
{
    size_t heapSize = dataHeap->size();

    //If the vector is empty, do nothing
    if (heapSize == 0)
        return 0;
    else
    {   
        //If the vector is an even size
        if (heapSize % 2 == 0)
        {
            //Return the calculated median
            return (dataHeap->at(heapSize / 2 - 1) + dataHeap->at(heapSize / 2)) / 2;
        }
        else
        //Vector is an odd size, so just return the element at size / 2
        {
            return dataHeap->at(heapSize / 2);
        }
    }
}

int calculateMode(vector<int> *dataHeap)
{
    int curNum = dataHeap->at(0);
    int mode = curNum;
    int count = 1;
    int countMode = 1;

    for(int i = 1; i < dataHeap->size(); i++)
    //Counts occurences of the current number
    {
        if (dataHeap->at(i) == curNum)
        {
            count++;
        }
       else
        //Number has changed into a different number
        {
            if (count > countMode)
            {
                countMode = count;
                mode = curNum;
            }
            //Reset count for occurences of new number
            count = 1;
            curNum = dataHeap->at(i);
        }
    }
    return mode;    
}

int calculateMean(vector<int> *dataHeap)
{
    return accumulate(dataHeap->begin(), dataHeap->end(), 0.0) / dataHeap->size();
}