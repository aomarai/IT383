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

int calculateMedian(vector<int> &dataHeap);
int calculateMode(vector<int> &dataHeap);
void *sortSubarray(void *args);

struct thread_data 
{
    pthread_t tid;
    vector<int> threadHeap;
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
    pthread_t threads[numThreads];
    pthread_attr_t attr;
    string inputFile = argv[2];
    ifstream inputStream(inputFile);
    vector<int> dataHeap;
    
    if (inputStream.is_open())
    {
        getline(inputStream, line);

        //Grab the number of integers
        numInts = stoi(line.substr(2));
        

        //Parse the remaining integers in the file and insert into vector
        while (getline(inputStream, line))
        {
            dataHeap.push_back(stoi(line));
        }
    }

    //Close the input stream for reading
    inputStream.close();

    //Split the vector into subarrays by the number of threads
    thread_data threadData[numThreads];
    localN = numInts / numThreads;
    localBegin = 0;
    localEnd = localBegin + localN;

    //Initialize and set thread attributes
    pthread_attr_init(&attr);

    //Create threads from numThreads
    for (int i = 0; i < numThreads; i++)
    {
        cout << "Creating thread " << i << endl;

    }

    //Join threads after completion
    for (int i = 0; i < numThreads; i++)
    {
        cout << "Joining thread " << i << endl;
    }



    //Test prints
    cout << "\nDEBUG STATS\n--------------------------------\nHeap size: " << dataHeap.size() << endl;
    for (int i = 0; i < 100; i++)
    {
        cout << dataHeap.at(i) << " ";
    }
    cout << "\nHeap median: " << calculateMedian(dataHeap) << endl;
    cout << "Heap mode: " << calculateMode(dataHeap) << endl;
    cout << "# Threads: " << numThreads << endl;
}

int calculateMedian(vector<int> &dataHeap)
{
    size_t heapSize = dataHeap.size();

    //If the vector is empty, do nothing
    if (heapSize == 0)
        return 0;
    else
    {   
        //If the vector is an even size
        if (heapSize % 2 == 0)
        {
            //Return the calculated median
            return (dataHeap[heapSize / 2 - 1] + dataHeap[heapSize / 2]) / 2;
        }
        else
        //Vector is an odd size, so just return the element at size / 2
        {
            return dataHeap[heapSize / 2];
        }
    }
}

int calculateMode(vector<int> &dataHeap)
{
    int curNum = dataHeap[0];
    int mode = curNum;
    int count = 1;
    int countMode = 1;

    for(int i = 1; i < dataHeap.size(); i++)
    //Counts occurences of the current number
    {
        if (dataHeap[i] == curNum)
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
            curNum = dataHeap[i];
        }
    }
    return mode;    
}

//Each thread will call this function to sort their own subarray with heapsort
void *sortSubarray(void *args)
{
    thread_data *threadData;

    //Sort the thread's subarray
    sort_heap(threadData->threadSubBegin, threadData->threadSubEnd);
}