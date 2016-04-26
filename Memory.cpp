#include <unordered_map>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "Memory.h"
using namespace std;

  unordered_map<int, Process> Memory::processes = {};

  int Memory::pidCounter = 0;

  float Memory::historyParameter;

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

  float Memory::historyParameter = 0;

    static deque<int> readyQueue;
  static vector<deque<int>> printerQueues;
  static vector<deque<int>> diskQueues0;
  static vector<deque<int>> diskQueues1;
  static vector<deque<int>> cdQueues;

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
    if (emptyCPU){
      cerr << "The CPU is unoccupied, no process present to be terminated" << '\n' << '\n';
    }
    else {
      unordered_map<int,Process>::iterator it = processes.find(currProcess);

      cout << "Process terminated" << '\n';
      cout << "------------------" << '\n';
      cout << "PID " << setw(10) << "Total CPU Time " << setw(10) << "Average Burst Time " << '\n';
      cout << currProcess << setw(10) << it->second.totalCPUTime << setw(20);
      if (it->second.cpuUsageCount > 0){
        cout << (it->second.totalCPUTime / it->second.cpuUsageCount);
      }
      else {
        cout << "0";
      }
      cout << '\n' << '\n';

      //The system's total cpu time and cpu usage count variables are updated with the
      //terminated process' corresponding variables. This updates the system's average
      //total CPU time 
      if (it->second.cpuUsageCount > 0){
        systemTotalCPUTime += it->second.totalCPUTime;
        systemTotalcpuUsageCount += it->second.cpuUsageCount;
      }

      processes.erase(currProcess);

      if (!readyQueue.empty()){
        currProcess = readyQueue.front();
        readyQueue.pop_front();
        emptyCPU = false;
        cout << "A new process has been added to the CPU." << '\n';
      }
      else {
        emptyCPU = true;
      }
      cout << '\n';
    } 
  }

  void Memory::snapshotAux_Disk(){
    multiset<Process>::iterator scanIt, scanItEnd, waitingIt, waitingItEnd;
    for (int i = 0; i < scanDiskQueuesStatus.size(); ++i){
      cout << "----" << "Scan Queue " << i+1 << '\n';
      if (scanDiskQueuesStatus[i] == 1){
        snapshotAux_Disk2(diskSets1[i].begin(), diskSets1[i].end());
        cout << '\n';
        cout << "----" << "Waiting Queue " << i+1 << '\n';
        snapshotAux_Disk2(diskSets0[i].begin(), diskSets0[i].end());
      }
      else {
        snapshotAux_Disk2(diskSets0[i].begin(), diskSets0[i].end());
        cout << '\n';
        cout << "----" << "Waiting Queue " << i+1 << '\n';
        snapshotAux_Disk2(diskSets1[i].begin(), diskSets1[i].end());
      }
      cout << '\n' << '\n';
    }
    cout << "System Average CPU Time" << '\n';
    cout << "-----------------------" << '\n';
    if (systemTotalcpuUsageCount > 0){
      cout << systemTotalCPUTime/systemTotalcpuUsageCount;
    }
    else {
      cout << "0"; 
    }
    cout << '\n' << '\n';
  }

  void Memory::snapshotAux_Disk2(multiset<Process>::iterator scanIt, multiset<Process>::iterator scanItEnd){
    while (scanIt != scanItEnd){
      cout << scanIt->pid << setw(10) << scanIt->name << setw(10) << scanIt->memStart 
        << setw(10) << scanIt->type << setw(10);
      if (scanIt->type == "w"){
        cout << scanIt->length << setw(10);
      }
      else {
        cout << setw(20);
      }
      cout << scanIt->totalCPUTime << setw(10) << (scanIt->totalCPUTime / scanIt->cpuUsageCount) << '\n';
      ++scanIt;
    }
  }

  //Updates the current process' burstEstimate and remaining burst variables
  void Memory::handleInterruptandSystemCall(){
    //Asking timer how long the current process has used the CPU
    string in;
    cout << "How long has the current process been using the CPU?" << '\n';
    cin >> in;

    //Keeps asking for valid input until it is received
    while (!intOrFloatErrorCheck(in, false, true)){
      cin >> in;
    }

    //The current process' remaining burst and burst estimate are updated
    processes[currProcess].remainingBurst = processes[currProcess].burstEstimate - floatResult;
    processes[currProcess].burstEstimate = sjwAlgorithm();

    //The current process' total cpu time and cpu usage count are updated
    processes[currProcess].totalCPUTime += floatResult;
    ++(processes[currProcess].cpuUsageCount);
  }

  /*
    Used for the running phase input, the function will check
    if the user input entered to represent a integer can actually
    be represented an integer and if the integer is negative or not
  */
  bool Memory::intOrFloatErrorCheck(string in, const bool& checkingInt, const bool& zeroValuesOK){
    istringstream iss{in};
    //Checks if the input can be converted to an int
    if (checkingInt){
      if (iss >> intResult && (iss.eof() || isspace(iss.peek()))) {
        if (!zeroValuesOK && intResult <= 0){
          cerr << "Zero or a negative number was entered. Please try again." << '\n';
          return false;
        }
        else if (zeroValuesOK && intResult < 0){
          cerr << "A negative number was entered. Please try again." << '\n';
          return false;
        }
        else{
          return true;
        }
      }
      else {
        cerr << "Non numeric characters enetered. Please try again" << '\n' << '\n';
        return false;
      }      
    }
    else {
      if (iss >> floatResult && (iss.eof() || isspace(iss.peek()))) {
        if (!zeroValuesOK && floatResult <= 0){
          cerr << "Zero or a negative number was entered. Please try again." << '\n';
          return false;
        }
        else if (zeroValuesOK && floatResult < 0){
          cerr << "A negative number was entered. Please try again." << '\n';
          return false;
        }
        else{
          return true;
        }
      }
      else {
        cerr << "Non numeric characters enetered. Please try again" << '\n' << '\n';
        return false;
      }  
    }
  }

  //Returns the result of the algorithm based on the current process' values
  float Memory::sjwAlgorithm(){
    cout << "history Parameter: " << historyParameter << '\n';
    cout << "float result: " << floatResult << '\n';

    return (1 - historyParameter) * processes[currProcess].burstEstimate + historyParameter * floatResult;
  }

  void Memory::snapshotAux_ReadyDeque(){
    deque<int>::iterator itB = readyQueue.begin();
    deque<int>::iterator itE = readyQueue.end();

    cout << "PID " << setw(10) << "Total CPU Time " << setw(10) << "Average Burst Time " << '\n';
    cout << "----r" << '\n';
    while (itB != itE){
      cout << *itB << setw(10) << processes[*itB].totalCPUTime << setw(10);
      if (processes[*itB].cpuUsageCount > 0){
        cout << (processes[*itB].totalCPUTime / processes[*itB].cpuUsageCount);
      }
      else {
        cout << "0";
      }
      cout << '\n';

      ++itB;
    }
    cout << '\n';
  }

  void Memory::snapshotAux_SystemInformation(){
    cout << "Total System Average CPU Time" << '\n';
    cout << "-----------------------------" << '\n';
    if (systemTotalcpuUsageCount > 0){
      cout << systemTotalCPUTime / systemTotalcpuUsageCount;
    }
    else {
      cout << "0";
    }
    cout << '\n' << '\n';
  }

  void Memory::getInstallerInput(){
    //Set the number of printer device queues
    getInstallerInput_aux("Enter the number of printer devices: ", 'o');
    printerQueues.resize(num);

    //Set the number of disk device queues
    getInstallerInput_aux("Enter the number of disk devices: ", 'o');
    diskQueues0.resize(num);
    diskQueues1.resize(num);
    cylinderCount.resize(num);
    scanDiskQueuesStatus.resize(num);

    //The elements are set to false, meaning that scanDiskQueues0 will
    //begin the program as representing the scan queues
    fill(scanDiskQueuesStatus.begin(),scanDiskQueuesStatus.end(),false);

    //Set the number of CD device queues
    getInstallerInput_aux("Enter the number of CD devices: ", 'o');
    cdQueues.resize(num);

    //Set the history parameter
    floatNum = 0;
    getInstallerInput_aux("Enter the history parameter: ", 'h');
    historyParameter = floatNum;

    //cout << "float: " << floatNum << '\n';
    //cout << "hist: " << historyParameter << '\n';

    //Set the inital burst estimate
    getInstallerInput_aux("Enter the initial burst estimate: ", 'i');
    initialBurstEstimate = floatNum;
    cout << "Initial burst estimate: " << initialBurstEstimate << '\n';

    //Set the number of cylinders in each disk device
    int n = cylinderCount.size();
    //The two strings are used to create the message used in the user prompt.
    //The message's content depends on which disk device it asking about.
    string messageBase = "Enter the number of cylinders in disk device ";
    string messageEnd = ": ";
    for (int i = 0; i < n; ++i){
      getInstallerInput_aux(messageBase+to_string(i+1)+messageEnd, 'o');
      cylinderCount[i] = num;
    }
  } 

  void Memory::getInstallerInput_aux(const string& userMessage, const char& variableCode){
    num = 0;
    cout << userMessage << '\n';
    while(!checkInputForErrors(variableCode)){
      cout << userMessage;
    }
    cout << '\n';
  }
  
  /*
    Prints userMessage to user before receiving input before
    parsing and checking for errors.
  */
  
  bool Memory::checkInputForErrors(const char& variableCode){
    string line;
    if (getline(cin, line)) {
      istringstream iss{line};
      //Checks if the input can be converted to an int
      if (variableCode == 'o'){
       if (iss >> num && (iss.eof() || isspace(iss.peek()))) {
          
         //If it was successfully converted, then checks if
         //num is negative
         if (num < 0){
           cerr << '\n' << "Negative number entered. Please try again" << '\n';
           return false;
         }
  
         /*
         If the function is being used to error check input for the history parameter,
         checkingHistoryParameter = true. If this is the case and the input entered
         is a valid integer, an additional function will be called to determine if the
         history parameter >= 0 and <= 1
         */
  
         //If num is not negative and if representing a history parameter
         //in the correct range, then the function will return true
         return true;            
       }
       else {
         cerr << '\n' << "Non numeric character entered. Please try again." << '\n';
         return false;
       }
      }
      else {
       if (iss >> floatNum && (iss.eof() || isspace(iss.peek()))) { 
         //If it was successfully converted, then checks if
         //num is negative
         if (floatNum < 0){
           cerr << '\n' << "Negative number entered. Please try again" << '\n';
           return false;
         }
  
         //If num is not negative and if representing a history parameter
         //in the correct range, then the function will return true
         if (variableCode == 'h'){
            if (!isHistoryParameterInRange()){
              cerr << "Input is not in correct range for the history parameter" << '\n';
              return false;
            }
            return true; 
         }
         return true;  
       }
       else {
         cerr << '\n' << "Non numeric character entered. Please try again." << '\n';
         return false;
       }
      }
    }
    return false;
  }

  /*
    Checks whether num is >= 0 and <= 1,
    which is necessary to be valid history parameter
  */
  bool Memory::isHistoryParameterInRange(){
    return (floatNum >= 0 && floatNum <= 1);
  }