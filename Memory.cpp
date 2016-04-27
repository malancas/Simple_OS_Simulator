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

  bool Memory::isScanQueues = true;

  float Memory::systemTotalCPUTime = 0;

  int Memory::systemTotalcpuUsageCount = 0;

  bool Memory::emptyCPU = true;

  int Memory::currProcess = -1;

  int Memory::intResult = 0;

  float Memory::floatResult = 0;

  int Memory::num = 0;
  
  float Memory::floatNum = 0;

  vector<int> Memory::cylinderCount = {};

  vector<bool> Memory::scanDiskQueuesStatus = {};

  deque<int> Memory::readyQueue = {};
  vector<deque<int>> Memory::printerQueues = {};
  vector<deque<int>> Memory::cdQueues = {};

  Memory::Memory() {};

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
          scanDiskQueuesStatus[callNum] = 0;
    		return;
    	}
    	else {
    		int finishedProcess = (diskSets1[callNum].begin())->pid;
    		diskSets1[callNum].erase(diskSets1[callNum].begin());
    		if (diskSets1[callNum].empty()){
    			scanDiskQueuesStatus[callNum] = 0;
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
          scanDiskQueuesStatus[callNum] = 1;
    		return;
    	}
    	else {
    		int finishedProcess = (diskSets0[callNum].begin())->pid;
    		diskSets0[callNum].erase(diskSets0[callNum].begin());
    		if (diskSets0[callNum].empty()){
    			scanDiskQueuesStatus[callNum] = 1;
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
    cout << "How long has the current process been using the CPU? ";
    cin >> in;
    cout << '\n';

    //Keeps asking for valid input until it is received
    while (!intOrFloatErrorCheck(in, false, true)){
      cin >> in;
    }

    //The current process' remaining burst and burst estimate are updated
    processes[currProcess].remainingBurst = processes[currProcess].burstEstimate - floatResult;
    processes[currProcess].burstEstimate = sjwAlgorithm();

    (processes[currProcess].totalCPUTime) += floatResult;
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
    diskSets0.resize(num);
    diskSets1.resize(num);
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

    //Set the inital burst estimate
    getInstallerInput_aux("Enter the initial burst estimate: ", 'i');
    initialBurstEstimate = floatNum;

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

  bool Memory::checkForQueues(const string& input){
    return (input[0] == 'p' && !printerQueues.empty()) || (input[0] == 'c' && !cdQueues.empty()) ||
            (input[0] == 'd' && !diskSets0.empty());
  }

  void Memory::waitForInput(){
    string input = "";

    //If input == q, the function and program will close
    while (input != "q"){
      cout << "Enter a new command: ";
      cin >> input;
      cout << '\n';

      //If the CPU is empty and the user issues an 'A',
      //a new process is created and added to the CPU
      if (input == "A"){
        cout << "New process made!" << '\n';
        processes.insert(make_pair(pidCounter,Process(pidCounter,initialBurstEstimate)));
        if (emptyCPU){
          currProcess = pidCounter;
          emptyCPU = false;          
          cout << "The CPU is now occupied!" << '\n' << '\n';
          //If the CPU is already occupied, the
          //process is added to the ready queue
        }
        //If the CPU isn't empty and the user issues an 'A',
        //a process is created and added to the ready queue
        else {
          handleInterruptandSystemCall();

          //The current process is readded to the ready queue
          addProcessToReadyQueue(currProcess);

          //The new process is added to the ready queue
          addProcessToReadyQueue(pidCounter);

          currProcess = readyQueue.front();
          readyQueue.pop_front();
          emptyCPU = false;
        }
        ++(pidCounter);
      }
      
      //If the user tries to terminate a process
      //while the CPU is empty
      else if (input == "t"){
        terminateProcess();
      }

      //If the user issues a system call in the form of either
      //p<number>, d<number>, or c<number>
      else if (input[0]=='p' || input[0]=='d' || input[0]=='c'){
        if (emptyCPU){
          cerr << "The CPU is empty, system calls cannot be made." <<'\n';
          cerr << "Add a process with the A command before issuing a system call." << '\n' << '\n';
        }

        else {
          if (checkForQueues(input)){
            int num = 0;

            //If the function determines the user's input is valid,
            if(systemCallInputChecking(input,num)){
              //The current process' burstEstimate and remainingBurst are updated
              //before it's added to a device queue
              handleInterruptandSystemCall();

              //Determining if the system call was for the printer
              //device is crucial for the systemCallParameters function
              bool print = false;
              if (input[0]=='p'){
                print = true;
              }
              //If the input is valid, the user will be prompted
              //for system call paramters and the call will be
              //added to the appropriate device queue 
              systemCallParameters(print, input[0], num);
              cout << "System call added!" << '\n' << '\n';
              if (!readyQueue.empty()){
                currProcess = readyQueue.front();
                readyQueue.pop_front();
                emptyCPU = false;
              }
              else {
                emptyCPU = true;
              }
            }            
          }
          else {
            cerr << "There are no queues available of the chosen device." << '\n';
            cerr << "Please enter a new command and try again." << '\n' << '\n';
          }
        }
      }

      //If the user issues system call interrupt signal
      else if (input[0]=='P' || input[0] == 'D' || input[0] == 'C'){
        int num = 0;

        handleInterruptandSystemCall();
        addProcessToReadyQueue(currProcess);
        currProcess = readyQueue.front();
        readyQueue.pop_front();

        //If the user's input is determined to be valid
        if (systemCallInputChecking(input,num)){
          if (input[0]=='P'){
            checkForSystemCallinQueue(printerQueues, num);
          }
          else if (input[0]=='D'){
            checkForSystemCallinDiskSet(num-1,scanDiskQueuesStatus[num-1]);
          }
          else if (input[0]=='C') {
            checkForSystemCallinQueue(cdQueues, num);
          }
        }     
      }

      /*If the user types 'S' for the snapshot function,
        the user will prompted for whether to print the contents
        of the ready, printer, cd, or disk queues. The contents of
        the chosen queue will read into an ostream object and 
        printed to the terminal
      */
      else if (input == "S"){
        handleInterruptandSystemCall();
        addProcessToReadyQueue(currProcess);
        currProcess = readyQueue.front();
        readyQueue.pop_front();

        cout << "Enter r, p, c, or d: ";
        cin >> input; cout << '\n';
        if (input != "r" && input != "p" && input != "c" && input != "d"){
          cerr << "The characters entered are not supported by Snapshot." << '\n';
          cerr << "Enter a new command and try again." << '\n' << '\n';
        }

        else {
          if (input == "d") {
            snapshotHeader();
            snapshotAux_Disk();
            snapshotAux_SystemInformation();
          }
          else if (input == "r"){
            snapshotAux_ReadyDeque();
            snapshotAux_SystemInformation();
          }
          else {

            snapshotHeader();
            snapshotAux(input);

            snapshotAux_SystemInformation();
          }
        }
      }
      else {
        cerr << "The characters entered are not a valid command." << '\n';
        cerr << "Please enter a new command and try again." << '\n' << '\n';
      }
    }
    return;
  }

  void Memory::snapshotHeader(){
    cout << "PID " << setw(10) << "Filename " << setw(10) << "Memstart " << setw(10) << "R/W " << setw(10) << "File Length " << 
    setw(10) << "Total CPU Time " << setw(10) << "Average Burst Time " << '\n';
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

  void Memory::systemCallParameters(const bool& print, const char& ch, int& num){
    string name = "";
    cout << "Enter the file name: ";
    cin >> name;
    cout << '\n';
    if (name.length() > 20){
      name.resize(20);
    }
    processes[currProcess].name = name;

    string memStart = "";
    int n = 0;

    cout << "Enter the starting location in memory: ";
    cin >> memStart;
    while (!intOrFloatErrorCheck(memStart, true, true)){
      cin >> memStart;
    }
    processes[currProcess].memStart = intResult;

    /*
      If the system call is not for a printing device,
      then the user will promted to confirm whether the action
      is a read or a write. Error checking will be done to insure
      that the user's input is either 'r' or 'w'. If the system call
      is for printer devices, then the type is automatically 'w'
    */
    string typeIn = "";
    if (!print){
      cout << "Enter r if your action is a read or enter w if your action is a write: ";
      cin >> typeIn;      
      while (!typeErrorChecking(typeIn)){
        cin >> typeIn;      
      }
      processes[currProcess].type = typeIn;
    }
    else {
      processes[currProcess].type = "w";
    }

    /*
      If the system is a write action, then
      the user will be prompted to enter the
      length of the file. The answer will be
      verified as a valid interger.
    */
    if (strcmp(processes[currProcess].type.c_str(),"w")==0){
      string input;

      cout << '\n' << "Enter the length of the file: ";
      cin >> input;
      while (!intOrFloatErrorCheck(input, true, false)){
        cin >> input;
      }
      processes[currProcess].length = intResult;
    }

    /*
      When the user has been prompted for all
      system call parameters, then the process
      will be added to the appropiate device queue
    */
    if (ch == 'p'){
      (printerQueues[num-1]).push_back(currProcess);
    }
    else if (ch == 'd'){
      getCylinderChoice(num-1);
      addProcessToDiskQueue(currProcess,num-1);
    }
    else { //ch == 'c'
      cdQueues[num-1].push_back(currProcess);
    }
  }

  void Memory::snapshotAux(const string& input){
    vector<deque<int>>::iterator itV, itVe;
    deque<int>::iterator itB, itE;
      if (input == "c"){
        if (!cdQueues.empty()){
          itV = cdQueues.begin(); itVe = cdQueues.end();
          itB = itV->begin(); itE = itV->end();
        }
      }
      else { //input == "p"
        if (!printerQueues.empty()){
          itV = printerQueues.begin(); itVe = printerQueues.end();
          itB = itV->begin(); itE = itV->end();        
        }
      }

      int count = 1;
      while (itV != itVe){
        cout << "----" << input << count << '\n';
        snapshotPrint(itB, itE);
        ++itV;
        ++count;
        itB = itV->begin();
        itE = itV->end();
      }
      cout << '\n' << '\n';
  }

  template<typename T>
  void Memory::snapshotPrint(T& itB, T& itE){
    while (itB != itE){
      string ty = processes[*itB].type;
      cout << *itB << setw(10) << processes[*itB].name << setw(10) << processes[*itB].memStart
        << setw(10) << ty; 
      if (ty == "w"){
        cout << setw(10) << processes[*itB].length;
      }
      else {
        cout << setw(20);
      }
      cout << processes[*itB].totalCPUTime << setw(10) << processes[*itB].totalCPUTime / processes[*itB].cpuUsageCount;
      cout << '\n';
      ++itB;
    }
  }

  bool Memory::typeErrorChecking(string& typeIn){
    if (typeIn != "w" && typeIn != "r"){
      cerr << "The character entered were not 'w' or 'r'." << '\n';
      cerr << "Please enter a new command and try again." << '\n';
      return false;
    }
    else {
      return true;
    }
  }

    /*
    Will check if the system call input, which will have
    already been verified to begin with either a 'p', 'd',
    or 'c' is valid. The number following the chosen character
    will be checked to insure it is not negative and falls within
    the limits of the number of specific device queues present
  */
  bool Memory::systemCallInputChecking(string& input, int& num){
    //Represents the string following either p,d, or c
    string queueNum = input.substr(1);

    /*Contents of quenum are fed in num to insure it can
      be read as an integer. An error message is printed 
      if this can't happen    
    */
    istringstream iss{queueNum};
    if (iss >> num && (iss.eof() || isspace(iss.peek()))){
      if (num <= 0){
        cerr << "Chosen number is zero or negative. Please try again." << '\n' << '\n';
        return false;
      }
      else {
        /*checkIfsys
          Each statement will check whether num is larger than the number
          of chosen device queues present
        */
        if (input[0] == 'p' || input[0] == 'P'){
          return checkIfsysCallNumLargerThanDevQueue(printerQueues, num);

        }
        else if (input[0] == 'd' || input[0] == 'D'){
          return checkIfsysCallNumLargerThanSet(num);
        }
        else { //input[0] == 'c' || input[0] == 'C'
          return checkIfsysCallNumLargerThanDevQueue(cdQueues, num);          
        }          
      }
    }
    /*
      If queuenum could not be read as integer into num
    */
    else {
      cerr << "The command you entered was invalid." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
      return false;
    }
  }

  void Memory::getCylinderChoice(const int& dequeNum){
    string in;
    cout << "Enter the cylinder that the file exists on: ";
    cin >> in;
    while (!intOrFloatErrorCheck(in, true, true) || !isCylinderChoiceValid(intResult,dequeNum)){
      cout << "The chosen cylinder is invalid. Please enter a new value and try again: ";
      cin >> in;
    }
  }

  bool Memory::isCylinderChoiceValid(const int& cylinderNum, const int& dequeNum){
    return cylinderNum < cylinderCount[dequeNum];
  }

  bool Memory::checkIfsysCallNumLargerThanDevQueue(const vector<deque<int>>& devQueues, const int& callNum){
    if (callNum > static_cast<int>(devQueues.size())){
      cerr << "Number entered is larger than current number of chosen device queues." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
      return false;
    }
    else {
      return true;
    }
  }

  bool Memory::checkIfsysCallNumLargerThanSet(const int& callNum){
    if (callNum > static_cast<int>(diskSets0.size())){
      cerr << "Number entered is larger than current number of chosen device queues." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
      return false;
    }
    else {
      return true;
    }
  }