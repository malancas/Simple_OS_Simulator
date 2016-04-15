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
  static unordered_map<int, Process> processes;

  /*
    The current value of the counter is assigned
    to new processes by incrementing to insure
    each has a unique PID
  */
  static int pidCounter;

  /*
    Also represented with α, used in the SJF
    approximation calculation
  */
  static float historyParameter;

  /*
    The initial burst estimate
  */
  static float initialBurstEstimate;

  //Used to determine which disk queue
  //vector represents the scan queues
  static vector<deque<int>*> scanQueuesPtrVector;

  static vector<deque<int>*> waitingQueuesPtrVector;

  static bool firstDiskSystemCall;

  static bool isScanQueues;

  /*
    Number of cylinders in the hard drive
  */
  static vector<int> cylinderCount;
  
  /*
    The ready queue contains process PIDS
    while each vector contains the number of
    device queues specified during the sys gen
    phase. Each of these queues contains PIDS
  */
  static deque<int> readyQueue;
  static vector<deque<int>> printerQueues;
  static vector<deque<int>> diskQueues0;
  static vector<deque<int>> diskQueues1;
  static vector<deque<int>> cdQueues;
};

#endif
