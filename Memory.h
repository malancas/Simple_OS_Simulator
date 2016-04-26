#ifndef MEMORY_H
#define MEMORY_H

#include <unordered_map>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <set>
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

  bool emptyCPU;

  int currProcess;

  /*
    Number of cylinders in the hard drive
  */
  vector<int> cylinderCount;

  //Holds boolean values, each corresponding
  //to the disk vector that is acting as the
  //scan vector.
  vector<bool> scanDiskQueuesStatus;
  
  //CONTRUCTORS
  Memory() : systemTotalCPUTime(0), systemTotalcpuUsageCount(0), firstDiskSystemCall(true), emptyCPU(true), currProcess(-1) {}

  /*
    The ready queue contains process PIDS
    while each vector contains the number of
    device queues specified during the sys gen
    phase. Each of these queues contains PIDS
  */
  deque<int> readyQueue;
  vector<deque<int>> printerQueues;
  vector<deque<int>> diskQueues0;
  vector<deque<int>> diskQueues1;
  //vector<deque<int>> diskQueues;
  vector<deque<int>> cdQueues;

  struct lowest_Track_First {
    bool operator() (const Process& lhs, const Process& rhs) const{
      return lhs.track < rhs.track;
    }
  };

  struct highest_Track_First {
    bool operator() (const Process& lhs, const Process& rhs) const{
      return lhs.track > rhs.track;
    }
  };

  vector<multiset<Process, lowest_Track_First>> diskSets0;
  vector<multiset<Process, highest_Track_First>> diskSets1;

  void addProcessToWaitingQueue(const int& pid, const int& dequeNum, const bool& zeroIsWaiting);
  void addProcessToDiskQueue(const int& pid, const int& queueNum);
  void checkForSystemCallinDiskSet(const int& callNum, const bool& zeroIsWaiting);
  void addProcessToReadyQueue(const int& pid);
  void terminateProcess();

};

#endif