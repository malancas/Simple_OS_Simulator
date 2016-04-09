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
  double Memory::historyParameter = 0;
  int Memory::initialBurstEstimate = 0;
vector<int> Memory::cylinderCount;
  vector<deque<int>> Memory::printerQueues;
  vector<deque<int>> Memory::diskQueues;
  vector<deque<int>> Memory::cdQueues;
