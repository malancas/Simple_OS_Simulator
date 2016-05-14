#include <unordered_map>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <set>
#include <stdio.h>
#include <string.h>
#include "Memory.h"
using namespace std;

  unordered_map<int, Process> Memory::processes = {};

  int Memory::pidCounter = 0;

  float Memory::historyParameter = 0;

  float Memory::initialBurstEstimate = 0;

  bool Memory::firstDiskSystemCall = true;

  float Memory::systemTotalCpuTime = 0;

  int Memory::systemTotalcpuUsageCount = 0;

  bool Memory::emptyCpu = true;

  int Memory::currProcess = -1;

  int Memory::intResult = 0;

  float Memory::floatResult = 0;

  int Memory::totalMemorySize = 0;

  int Memory::freeMemory = 0;

  int Memory::maximumProcessSize = 0;

  int Memory::pageSize = 0;

  vector<int> Memory::cylinderCount = {};

  vector<bool> Memory::scanDiskDequesStatus = {};

  deque<int> Memory::readyDeque = {};
  vector<deque<int>> Memory::printerDeques = {};
  vector<deque<int>> Memory::cdDeques = {};
  vector<deque<int>> Memory::diskDeques0 = {};
  vector<deque<int>> Memory::diskDeques1 = {};
  deque<int> Memory::jobPool = {};
	vector<vector<int>> Memory::frameTable = {};
	deque<int> Memory::freeFrameList = {};

  vector<multiset<int,Memory::SortByLowCmp>> Memory::diskSets0 = {};
  vector<multiset<int,Memory::SortByHighCmp>> Memory::diskSets1 = {};

  Memory::Memory() {};

  void Memory::addProcessToDiskDeque(const int& pid, const int& dequeNum){
    if (firstDiskSystemCall){
      diskDeques0[dequeNum].push_back(pid);
      firstDiskSystemCall = false;
    }
    else {
      if (scanDiskDequesStatus[dequeNum] == 1){
      	//addProcessToWaitingDeque(pid,dequeNum,true);
        processes[pid].locationCode = "d0" + to_string(dequeNum);
        diskDeques0[dequeNum].push_back(pid);
      }
      else {
      	//addProcessToWaitingDeque(pid,dequeNum,false);
        processes[pid].locationCode = "d1" + to_string(dequeNum);
        diskDeques1[dequeNum].push_back(pid);
      }
    }
  }

  void Memory::addProcessToReadyDeque(const int& pid){
    //Represents the remaining burst of process to be inserted
    float burstOfNewProcess = processes[pid].remainingBurst;
    /*
      The ready deque is traversed and the remaining burst
      member variable of each process in the ready deque is
      compared to the new process' remaining burst. If the
      burst in the ready deque is larger than burstOfNewProcess,
      the pid of the new process is inserted at the current place
      of the iterator and the function returns
    */
    deque<int>::iterator it = readyDeque.begin();
    while (it != readyDeque.end()){
      if (processes[*it].remainingBurst > burstOfNewProcess){
        readyDeque.insert(it, pid);
        return;
      }
      ++it;
    }
    /*
      If the new process' remaining burst is bigger than every other
      process' remaining burst in the readyDeque, it is pushed to the
      back of the deque
    */
    readyDeque.push_back(pid);
  }


  //Returns the result of the algorithm based on the current process' values
  float Memory::sjwAlgorithm(){
    return (1 - historyParameter) * processes[currProcess].burstEstimate + historyParameter * floatResult;
  }

  void Memory::checkForSystemCallinDeque(vector<deque<int>>& devDeques, const int& callNum){
    if (!devDeques.empty()){
      if (devDeques[callNum-1].empty()){
        cerr << "No system calls are currently in the chosen deque " << callNum << '\n' << '\n';
      }

      //The system call at the front of the deque is removed
      else {
        int finishedProcess = devDeques[callNum-1].front();
        devDeques[callNum-1].pop_front();
        if (emptyCpu){
          currProcess = finishedProcess;
          emptyCpu = false;
        }
        else {
          addProcessToReadyDeque(finishedProcess);
        }
        cout << "A system call has completed" << '\n' << '\n';
      }
    }
    else {
      cerr << "There are no deques of this type. No processes exist in these deques." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
    }
  }

  bool Memory::sortByLowestTrackFirst(const int& lhs, const int& rhs){
    return processes[lhs].track < processes[rhs].track;
  }

  bool Memory::sortByHighestTrackFirst(const int& lhs, const int& rhs){
    return processes[lhs].track > processes[rhs].track;
  }

bool Memory::sortByLargestSizeFirst(const int& lhs, const int& rhs){
  return processes[lhs].size > processes[rhs].size;
}
