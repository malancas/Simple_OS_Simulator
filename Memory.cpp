#include <unordered_map>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Process.cpp"
#include "SystemCall.cpp"
using namespace std;

struct Memory {
  //VARIABLES
  unordered_map<int, Process> processes;
  int pidCounter;
  int systemCallCounter;
  queue<int> readyQueue;
  vector<queue<int>> printerQueues;
  vector<queue<int>> diskQueues;
  vector<queue<int>> cdQueues;

  //CONSTRUCTORS

  //FUNCTIONS
  void addProcesstoQueue(){
    processes.insert (make_pair(pidCounter, Process(pidCounter)));
    readyQueue.push(pidCounter);
    ++pidCounter;
  }

  void makeQueues(const int& inP, const int& inD, const int& inC){
    printerQueues.resize(inP);
    diskQueues.resize(inD);
    cdQueues.resize(inC);
  }
};