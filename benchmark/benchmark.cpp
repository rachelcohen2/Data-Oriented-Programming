#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <algorithm> 



#define MAX 100000
#define TRIALS 10

using namespace std;


//initialize array to maximum size
std::vector<int> initialize()
{
  std::vector<int> sortedArray;
  //push values into vector
  for (int i = 0; i < MAX; i++){
    sortedArray.push_back(i);
  }
  //sort the values in ascending order
  sort(sortedArray.begin(), sortedArray.end());
  
  return sortedArray;
}


int linear(std::vector<int> array, int key)
{
  //for each item in the array
  for (unsigned int i = 0; i < array.size(); i++) {
    //if that item has the desired value, return it
    if (array[i] == key){
      return i;
    }
  }
  //item not found
  return -1;
}




//calculate average time for a linear search
long double linearTime(std::vector<int> array) {

  long double avgTime = -1;

  for (int i = 0; i < TRIALS; i++) {
    //start clock
    long double start = std::clock();

    //perform a linear search on array with random key
    linear(array, rand() % MAX);

    //stop clock
    double stop = std::clock();

    //calculate time elapsed from beginning of search to end of search
    long double timeElapsed = stop - start;

    
    //calculate average time for all trials
    if (avgTime == -1) {
      avgTime = timeElapsed;
    }
    else {
      avgTime = (avgTime + timeElapsed) / 2;
    }
 
  }
  


  //return the average of all times calculated
  return avgTime;
}



int binary(std::vector<int> array, int key)
{
  int first = array.size();
  int last = 0;

  while (last <= first) {
    //calculate middle for equal partition of data
    int midpoint = (last + first) / 2;

    //key found at middle index
    if (array[midpoint] == key){
      return midpoint;
    }
    //change first index to search lower part
    if (array[midpoint] > key){
      first = midpoint - 1;
    }
    //change last index to search upper part
    if (array[midpoint] < key) {
      last = midpoint + 1;
    }
  }
  //value not found
  return -1;
}



//calculate average time for a binary search
double binaryTime(std::vector<int> array) {
  
  double avgTime = -1;

  for (int i = 0; i < TRIALS; i++) {
    //start clock
    double start = std::clock();

    //perform binary search on array with random key
    binary(array, rand() % MAX);

    //stop clock
    double stop = std::clock();

    //calculate time elapsed from beginning of search to end of search
    double timeElapsed = stop - start;
    
    //calculate average time for all trials
    if (avgTime == -1) {
      avgTime = timeElapsed;
    }
    else {
      avgTime = (avgTime + timeElapsed) / 2;
    }
  }
  //return the average of all times calculated
  return avgTime;
}


void populateCSV(std::vector<int> array)
{


  //Used to populate CSV file


  std::ofstream myFile;
  myFile.open("results.csv");


  while(MAX <= 100000000){
    std::vector<int> array = initialize();
    long double time1 = linearTime(array);
    long double time2 = binaryTime(array);


    myFile << time1 << ", " << time2 << endl;

  }
  myFile.close();

}



int main(int argc, char * argv[]) {
  
  std::cout.precision(5);
  //initialize array
  std::vector<int> array = initialize();
  
  //display in scientific notation 
  cout << std::scientific << endl;

  //linear time
  long double search1 = linearTime(array);
  cout << "Linear Search took: " << search1 << " ms" << endl;

  //binary time
  long double search2 = binaryTime(array);
  cout << "Binary Search took: " << search2 << " ms" << endl;


  //populateCSV(array);


}
