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

  static bool isScanQueues;

  static bool scanGoesUp;

  static float systemTotalCPUTime;

  static int systemTotalcpuUsageCount;

  static bool emptyCPU;

  static int currProcess;

  static int intResult;

  static float floatResult;

  static int num;
  
  static float floatNum;

  /*
    Number of cylinders in the hard drive
  */
  static vector<int> cylinderCount;

  //Holds boolean values, each corresponding
  //to the disk vector that is acting as the
  //scan vector.
  static vector<bool> scanDiskQueuesStatus;
  
  //CONTRUCTORS
  Memory() : systemTotalCPUTime(0), systemTotalcpuUsageCount(0),
   firstDiskSystemCall(true), emptyCPU(true), currProcess(-1) {}

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
  void snapshotAux_Disk();
  void snapshotAux_Disk2(multiset<Process>::iterator scanIt, multiset<Process>::iterator scanItEnd);
  void handleInterruptandSystemCall();
  bool intOrFloatErrorCheck(string in, const bool& checkingInt, const bool& zeroValuesOK);
  float sjwAlgorithm();
  void snapshotAux_ReadyDeque();
  void snapshotAux_SystemInformation();
  void getInstallerInput();
  void getInstallerInput_aux(const string& userMessage, const char& variableCode);
  bool checkInputForErrors(const char& variableCode);
  bool isHistoryParameterInRange();

};

#endif