#ifndef MEMORY_H
#define MEMORY_H

#include <unordered_map>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Process.cpp"
using namespace std;

struct Memory {
  //VARIABLES
  
  /*
    Contains the Process objects, using their
    PIDS as keys. This allows the ready queue
    and device queues to deal with ints instead
    of objects
  */
  unordered_map<int, Process> processes;

  /*
    The current value of the counter is assigned
    to new processes by incrementing to insure
    each has a unique PID
  */
  int pidCounter;

  /*
    Also represented with α, used in the SJF
    approximation calculation
  */
  float historyParameter;

  /*
    The initial burst estimate
  */
  float initialBurstEstimate;

  //Used to determine which disk queue
  //vector represents the scan queues
  vector<deque<int>*> scanQueuesPtrVector;

  vector<deque<int>*> waitingQueuesPtrVector;

  bool firstDiskSystemCall;

  bool isScanQueues;

  bool scanGoesUp;

  float systemTotalCPUTime;

  int systemTotalcpuUsageCount;

  /*
    Number of cylinders in the hard drive
  */
  vector<int> cylinderCount;
  
  //CONTRUCTORS
  Memory() : systemTotalCPUTime(0), systemTotalcpuUsageCount(0) {}

  /*
    The ready queue contains process PIDS
    while each vector contains the number of
    device queues specified during the sys gen
    phase. Each of these queues contains PIDS
  */
  deque<int> readyQueue;
  vector<deque<int>> printerQueues;
  //vector<deque<int>> diskQueues0;
  //vector<deque<int>> diskQueues1;
  vector<deque<int>> diskQueues;
  vector<deque<int>> cdQueues;
};

#endif