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
int calculateMean(vector<int> &dataHeap);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: number_of_threads inputfile" << endl;
        exit(1);
    }

    int numInts;
    int numThreads = stoi(argv[1]);
    string line;
    pthread_t threads[numThreads];
    string inputFile = argv[2];
    ifstream inputStream(inputFile);
    vector<int> dataHeap;
    
    if (inputStream.is_open())
    {
        getline(inputStream, line);

        //Grab the number of integers and store it for later use for parallelization
        numInts = stoi(line.substr(2));

        //Parse the remaining integers in the file and insert into vector
        while (getline(inputStream, line))
        {
            dataHeap.push_back(stoi(line));
        }
    }

    //Close the input stream for reading
    inputStream.close();

    //Heapsort the entire array for serial
    sort_heap(dataHeap.begin(), dataHeap.end());

    //Test prints
    cout << "DEBUG STATS\n--------------------------------\nHeap size: " << dataHeap.size() << endl;
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

int calculateMean(vector<int> &dataHeap)
{
    return accumulate(dataHeap.begin(), dataHeap.end(), 0.0) / dataHeap.size();
}