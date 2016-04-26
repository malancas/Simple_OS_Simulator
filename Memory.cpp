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

  void Memory::addProcessToDiskQueue(const int& pid, const int& queueNum){
    if (firstDiskSystemCall){
      addProcessToWaitingQueue(pid,queueNum,true);
      firstDiskSystemCall = false;
    }
    else {
      if (scanDiskQueuesStatus[queueNum] == 1){
      	addProcessToWaitingQueue(pid,queueNum,true);
      }
      else {
      	addProcessToWaitingQueue(pid,queueNum,false);
      }
    }
  }

  void Memory::checkForSystemCallinDiskSet(const int& callNum, const bool& zeroIsWaiting){
    if (zeroIsWaiting){
    	if (diskSets1.empty()){
    	    cerr << "There are no queues of this type. No processes exist in these queues." << '\n';
      		cerr << "Please enter a new command and try again." << '\n' << '\n';
      		return;      	
    	}
    	else if (diskSets1[callNum].empty()){
        	cerr << "No system calls are currently in the chosen queue " << callNum << '\n' << '\n';
    		return;
    	}
    	else {
    		int finishedProcess = (diskSets1[callNum].begin())->pid;
    		diskSets1[callNum].erase(diskSets1[callNum].begin());
    		if (diskSets1[callNum].empty()){
    			scanDiskQueuesStatus[callNum] = !scanDiskQueuesStatus[callNum];
    		}

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
    	if (diskSets0.empty()){
    	    cerr << "There are no queues of this type. No processes exist in these queues." << '\n';
      		cerr << "Please enter a new command and try again." << '\n' << '\n';
      		return;      	
    	}
    	else if (diskSets0[callNum].empty()){
        	cerr << "No system calls are currently in the chosen queue " << callNum << '\n' << '\n';
    		return;
    	}
    	else {
    		int finishedProcess = (diskSets0[callNum].begin())->pid;
    		diskSets0[callNum].erase(diskSets0[callNum].begin());
    		if (diskSets0[callNum].empty()){
    			scanDiskQueuesStatus[callNum] = !scanDiskQueuesStatus[callNum];
    		}

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

  void Memory::terminateProcess(){
    if (m.emptyCPU){
      cerr << "The CPU is unoccupied, no process present to be terminated" << '\n' << '\n';
    }
    else {
      unordered_map<int,Process>::iterator it = m.processes.find(m.currProcess);

      cout << "Process terminated" << '\n';
      cout << "PID " << setw(10) << "Total CPU Time " << setw(10) << "Average Burst Time " << '\n';
      cout << m.currProcess << setw(10) << it->second.totalCPUTime << setw(10) << 
      (it->second.totalCPUTime / it->second.cpuUsageCount) << '\n' << '\n';

      m.processes.erase(m.currProcess);

      if (!m.readyQueue.empty()){
        m.currProcess = m.readyQueue.front();
        m.readyQueue.pop_front();
        m.emptyCPU = false;
        cout << "A new process has been added to the CPU." << '\n';
      }
      else {
        m.emptyCPU = true;
      }
      cout << '\n';
    } 
  }