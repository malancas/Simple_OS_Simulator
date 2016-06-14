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
	vector<vector<int>> Memory::frameTable = {};
	deque<int> Memory::freeFrameList = {};

  vector<multiset<int,Memory::SortByLowTrack>> Memory::diskSets0 = {};
  vector<multiset<int,Memory::SortByHighTrack>> Memory::diskSets1 = {};
  set<int,Memory::SortByLowBurst> Memory::readySet = {};
  set<int,Memory::SortBySize> Memory::jobPool = {};


  Memory::Memory() {};


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
        	readySet.insert(finishedProcess);
        }
        cout << "A system call has completed" << '\n' << '\n';
      }
    }
    else {
      cerr << "There are no deques of this type. No processes exist in these deques." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
    }
  }


//int Memory::getPIDFromFrontOfSet()

/*
  bool Memory::sortByLowestTrackFirst(const int& lhs, const int& rhs){
    return processes[lhs].track < processes[rhs].track;
  }

  bool Memory::sortByHighestTrackFirst(const int& lhs, const int& rhs){
    return processes[lhs].track > processes[rhs].track;
  }

bool Memory::sortByLargestSizeFirst(const int& lhs, const int& rhs){
  return processes[lhs].size > processes[rhs].size;
}
*/