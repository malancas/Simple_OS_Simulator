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