#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <mutex>
#include <iostream>
#include <chrono>
#include <math.h>
#include <stdexcept>
#define BYTE_CONST 1024
#define BYTES 4 //each int takes 4 bytes
#define INCREASE_SIZE 2 //increase by powers of 2 
#define INDEX_1 1
#define INDEX_2 2 
#define MIN 10 //start with this data size
#define MAX 20 //increase to this data size


//megabytes = bytes/1024/1024

using namespace std;

std::vector<int> dataVector;
int amtData; 
std::mutex mutexLock; 



void vectorWrite(int dataSize)
{

  try{

    //initialize data
    amtData = dataSize;

    //lock guard guarantees unlocking upon destruction
    std::lock_guard<std::mutex> lock(mutexLock);

    //add data to vector 
    for (int i = 0; i < amtData; i++)
      {
	dataVector.push_back(i);
      }
  }

  //catch error
  catch(std::logic_error&){
    cout << "exception caught" << endl;
  }
}



void vectorRead()
{

  //start the timing 
  auto startTimer = chrono::high_resolution_clock::now();

  //read data 
  for (int i = 0; i < amtData; i++)
    {
      dataVector[i];
      int var = dataVector[i];
    }

  //stop timer and calculate range   
  auto endTimer = chrono::high_resolution_clock::now();
  auto range = endTimer - startTimer;

  //print time range 
  cout << "timing: "<< chrono::duration <float, std::nano> (range).count();

}


int setAffinity(int id)
{

  //sysconf(_SC_NPROCESSORS_ONLN) returns num of processors currently online
  int numProcessors = sysconf(_SC_NPROCESSORS_ONLN);

  //if the core id is greater than the number of cores, return false
  if (id >= numProcessors)
    {
      return -1;
      cout << "ID cannot be greater than the number of processors" << endl;
    }

  //represents a set of cpus
  cpu_set_t cpuset;

  //clear set
  CPU_ZERO(&cpuset);

  //add id to cpu set 
  CPU_SET(id, &cpuset);

  //thread initialization
  pthread_attr_t pta; 
  pthread_t thread = pthread_self();    

  //sets cpu affinity
  return pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);

}


int main(int argc, char *argv[])
{

  
  std::thread threadCounter[INDEX_2];

  //initialize holder variables
  int totalBytes;
  int kbs;
  float mbs;

  //variable to hold sucessively increasing size
  int increaseSize;
  
  //repeat for successively larger data sizes
  for (int i = MIN; i < MAX; i++) {

    //increase by powers of 2
    increaseSize = pow(INCREASE_SIZE, i); 

    
    //set affinity for first thread run
    setAffinity(0);

    //write data for first thread run
    threadCounter[0] = std::thread(vectorWrite, increaseSize);
    threadCounter[0].join(); 

    //calculate total bytes
    totalBytes = BYTES * increaseSize;

    //divide by 1024 to get kbs
    kbs = totalBytes/BYTE_CONST;

    //display kbs
    cout << kbs << " kbs, ";

    //divide again by 1024 to get mbs
    mbs = kbs/BYTE_CONST;

    //display mbs
    cout << mbs << " mbs, ";
  
    //read data for first thread run
    threadCounter[0] = std::thread(vectorRead);
    threadCounter[0].join();

    cout << ", ";

  
    //set affinity for second thread run
    setAffinity(INDEX_1);

    //read data for second thread run
    threadCounter[INDEX_1] = std::thread(vectorRead);
    threadCounter[INDEX_1].join();
    cout << "\n";
  

  }

  return 0;

}
