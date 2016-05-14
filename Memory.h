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
  //VARIABLES

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

  static bool firstDiskSystemCall;

  static float systemTotalCpuTime;

  static int systemTotalcpuUsageCount;

  //Signals whether the CPU is empty
  static bool emptyCpu;

  //Contains the pid of the process
  //currently using the CPU
  static int currProcess;

  static int intResult;

  static float floatResult;

  static int totalMemorySize;

  static int freeMemory;

  static int maximumProcessSize;

  static int pageSize;

  /*
    Number of cylinders in the hard drive
  */
  static vector<int> cylinderCount;

  //Holds boolean values, each corresponding
  //to the disk vector that is acting as the
  //scan vector.
  static vector<bool> scanDiskDequesStatus;

  //CONTRUCTORS
  Memory();

  /*
    The ready deque contains process PIDS
    while each vector contains the number of
    device deques specified during the sys gen
    phase. Each of these deques contains PIDS
  */
  static deque<int> readyDeque;
  static vector<deque<int>> printerDeques;
  static vector<deque<int>> cdDeques;
  static vector<deque<int>> diskDeques0;
  static vector<deque<int>> diskDeques1;
  static vector<vector<int>> frameTable;
  static deque<int> freeFrameList;

  //void addProcessToWaitingDeque(const int& pid, const int& dequeNum, const bool& zeroIsWaiting);
  void addProcessToDiskDeque(const int& pid, const int& dequeNum);
  void checkForSystemCallinDiskDeque(const int& callNum, const bool& zeroIsWaiting);
  //void checkForSystemCallinDiskSet(const int& callNum, const bool& zeroIsWaiting);
  float sjwAlgorithm();
  void checkForSystemCallinDeque(vector<deque<int>>& devDeques, const int& callNum);
  static bool sortByLowestTrackFirst(const int& lhs, const int& rhs);
  static bool sortByHighestTrackFirst(const int& lhs, const int& rhs);
  static bool sortByLargestSizeFirst(const int& lhs, const int& rhs);

  struct SortByLowCmp{
		bool operator() (const int& x, const int& y) { return processes[x].track < processes[y].track; }
  };

  struct SortByHighCmp{
		bool operator() (const int& x, const int& y) { return processes[x].track > processes[y].track; }
  };

  struct SortByLowBurst{
  	bool operator() (const int& x, const int& y) { return processes[x].remainingBurst < processes[y].remainingBurst; }
  };

  struct SortBySize{
  	bool operator() (const int& x, const int& y) { return processes[x].size > processes[y].size; }
  };

/*
  auto f = [](const int& x, const int& y)
  {return processes[x].size < processes[y].size;};
*/

  static vector<multiset<int,SortByLowCmp>> diskSets0;
  static vector<multiset<int,SortByHighCmp>> diskSets1;
  static set<int,SortByLowBurst> readySet;
  static set<int,SortBySize> jobPool;
};

#endif
