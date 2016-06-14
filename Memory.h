#ifndef MEMORY_H
#define MEMORY_H

#include <unordered_map>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <stdio.h>
#include "Process.cpp"

using namespace std;

struct Memory {
  //CONTRUCTORS
  Memory();

  //FUNCTORS
  //The functors below are used for certain sorted data structures that follow
  struct SortByLowTrack{
    bool operator() (const int& x, const int& y) { return processes[x].track < processes[y].track; }
  };

  struct SortByHighTrack{
    bool operator() (const int& x, const int& y) { return processes[x].track > processes[y].track; }
  };

  struct SortByLowBurst{
    bool operator() (const int& x, const int& y) { return processes[x].remainingBurst < processes[y].remainingBurst; }
  };

  struct SortBySize{
    bool operator() (const int& x, const int& y) { return processes[x].size > processes[y].size; }
  };

  //VARIABLES

  //Holds boolean values, each corresponding
  //to the disk vector that is acting as the
  //scan vector.
  vector<bool> scanDiskDequesStatus;

  /*
    Contains the Process objects, using their
    PIDS as keys. This allows the ready deque
    and device deques to deal with ints instead
    of objects
  */
  static unordered_map<int, Process> processes;

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

  float initialBurstEstimate;

  bool firstDiskSystemCall;

  float systemTotalCpuTime;

  int systemTotalcpuUsageCount;

  bool emptyCpu;

  //Contains the pid of the process
  //currently using the CPU
  int currProcess;

  int intResult;

  float floatResult;

  int totalMemorySize;

  int freeMemory;

  int maximumProcessSize;

  int pageSize;

  
  //Represents cylinders in the hard drive
  vector<int> cylinders;

  /*
    The ready deque contains process PIDS
    while each vector contains the number of
    device deques specified during the sys gen
    phase. Each of these deques contains PIDS
  */
  deque<int> readyDeque;

  vector<deque<int>> printerDeques;
  
  vector<deque<int>> cdDeques;
  
  vector<deque<int>> diskDeques0;
  
  vector<deque<int>> diskDeques1;
  
  vector<vector<int>> frameTable;
  
  deque<int> freeFrameList;
  
  void checkForSystemCallinDeque(vector<deque<int>>& devDeques, const int& callNum);
  
  vector<multiset<int,SortByLowTrack>> diskSets0;
  
  vector<multiset<int,SortByHighTrack>> diskSets1;
  
  set<int,SortByLowBurst> readySet;
  
  set<int,SortBySize> jobPool;
};

#endif
