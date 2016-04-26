#include <unordered_map>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Memory.h"
using namespace std;

  void Memory::addProcessToWaitingQueue(const int& pid, const int&setNum, const bool& zeroIsWaiting){
    if (zeroIsWaiting){
    	diskSets0[setNum].insert(processes[pid]);
    }
    else {
    	diskSets1[setNum].insert(processes[pid]);
    }
  }