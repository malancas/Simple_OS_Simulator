#include <unordered_map>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Memory.h"
using namespace std;

  //VARIABLES
  unordered_map<int, Process> Memory::processes;
  int Memory::pidCounter = 0;
  deque<int> Memory::readyQueue;
  float Memory::historyParameter = 0;
  float Memory::initialBurstEstimate = 0;
  vector<deque<int>*> Memory::scanQueuesPtrVector;
  vector<deque<int>*> Memory::waitingQueuesPtrVector;
  bool Memory::firstDiskSystemCall = true;
  bool Memory::isScanQueues = true;
  bool Memory::scanGoesUp = true;
  float Memory::systemTotalCPUTime = 0;
  int Memory::systemTotalcpuUsageCount = 0;
  vector<int> Memory::cylinderCount;
  vector<deque<int>> Memory::printerQueues;
  //vector<deque<int>> Memory::diskQueues0;
  //vector<deque<int>> Memory::diskQueues1;
  vector<deque<int>> Memory::diskQueues;
  vector<deque<int>> Memory::cdQueues;
