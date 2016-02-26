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
  unordered_map<int, SystemCall> systemCalls;
  int pidCounter;
  int pcbCounter;
  int systemCallCounter;
  queue<int> readyQueue;
  vector<queue<int>> printerQueues;
  vector<queue<int>> diskQueues;
  vector<queue<int>> cdQueues;

  //PCB index?

  //CONSTRUCTORS

  //FUNCTIONS
  void addProcesstoQueue(){
    processes.insert (make_pair(pidCounter, Process(pidCounter, pcbCounter)));
    readyQueue.push(pidCounter);
    ++pidCounter;
    ++pcbCounter;
  }

  void makeQueues(string& inP, string& inD, string& inC){
    printerQueues.resize(stoi(inP));
    diskQueues.resize(stoi(inD));
    cdQueues.resize(stoi(inC));
  }
};