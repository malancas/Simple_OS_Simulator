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

  float Memory::systemTotalCPUTime = 0;

  int Memory::systemTotalcpuUsageCount = 0;

  bool Memory::emptyCPU = true;

  int Memory::currProcess = -1;

  int Memory::intResult = 0;

  float Memory::floatResult = 0;

  int Memory::totalMemorySize = 0;

  int Memory::freeMemory = 0;

  int Memory::maximumProcessSize = 0;

  int Memory::pageSize = 0;

  vector<int> Memory::cylinderCount = {};

  vector<bool> Memory::scanDiskQueuesStatus = {};

  deque<int> Memory::readyQueue = {};
  vector<deque<int>> Memory::printerQueues = {};
  vector<deque<int>> Memory::cdQueues = {};
  vector<deque<int>> Memory::diskDeques0 = {};
  vector<deque<int>> Memory::diskDeques1 = {};
  deque<int> Memory::jobPool = {};
vector<tuple<int,int>> Memory::frameTable = {};
vector<int> Memory::freeFrameList = {};

  Memory::Memory() {};

  void Memory::addProcessToDiskDeque(const int& pid, const int& queueNum){
    if (firstDiskSystemCall){
      diskDeques0[queueNum].push_back(pid);
      firstDiskSystemCall = false;
    }
    else {
      if (scanDiskQueuesStatus[queueNum] == 1){
      	//addProcessToWaitingQueue(pid,queueNum,true);
        processes[pid].locationCode = "d0" + to_string(queueNum);
        diskDeques0[queueNum].push_back(pid);
      }
      else {
      	//addProcessToWaitingQueue(pid,queueNum,false);
        processes[pid].locationCode = "d1" + to_string(queueNum);
        diskDeques1[queueNum].push_back(pid);
      }
    }
  }

  void Memory::addProcessToReadyQueue(const int& pid){
    //Represents the remaining burst of process to be inserted
    float burstOfNewProcess = processes[pid].remainingBurst;
    /*
      The ready queue is traversed and the remaining burst
      member variable of each process in the ready queue is
      compared to the new process' remaining burst. If the 
      burst in the ready queue is larger than burstOfNewProcess, 
      the pid of the new process is inserted at the current place
      of the iterator and the function returns
    */
    deque<int>::iterator it = readyQueue.begin();
    while (it != readyQueue.end()){
      if (processes[*it].remainingBurst > burstOfNewProcess){
        readyQueue.insert(it, pid);
        return;
      }
      ++it;
    }
    /*
      If the new process' remaining burst is bigger than every other
      process' remaining burst in the readyQueue, it is pushed to the
      back of the queue
    */
    readyQueue.push_back(pid);
  }

  
  //Returns the result of the algorithm based on the current process' values
  float Memory::sjwAlgorithm(){
    return (1 - historyParameter) * processes[currProcess].burstEstimate + historyParameter * floatResult;
  }

  void Memory::checkForSystemCallinQueue(vector<deque<int>>& devQueues, const int& callNum){
    if (!devQueues.empty()){
      if (devQueues[callNum-1].empty()){
        cerr << "No system calls are currently in the chosen queue " << callNum << '\n' << '\n';
      }

      //The system call at the front of the queue is removed
      else {
        int finishedProcess = devQueues[callNum-1].front();
        devQueues[callNum-1].pop_front();
        if (emptyCPU){
          currProcess = finishedProcess;
          emptyCPU = false;
        }
        else {
          addProcessToReadyQueue(finishedProcess);
        }
        cout << "A system call has completed" << '\n' << '\n';
      }
    }
    else {
      cerr << "There are no queues of this type. No processes exist in these queues." << '\n';
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
