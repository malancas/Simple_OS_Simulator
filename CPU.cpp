#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <deque>
#include <iomanip>
#include <algorithm>
#include <set>
#include "CPU.h"
#include "Memory.h"
using namespace std;

  CPU::CPU() {}

  //FUNCTIONS
  void CPU::waitForInput(){
    string input = "";

    //If input == q, the function and program will close
    while (input != "q"){
      cout << "Enter a new command: ";
      cin >> input;
      cout << '\n';

      //If the CPU is empty and the user issues an 'A',
      //a new process is created and added to the CPU
      if (input == "A"){
        if (getProcessSize()){
          cout << "New process made!" << '\n';
          processes.insert(make_pair(pidCounter,Process(pidCounter,initialBurstEstimate,intResult)));
          //if (intResult <= freeMemory){
          if (processes[pidCounter].size <= freeMemory){
            //totalMemorySize -= intResult;
            totalMemorySize -= processes[pidCounter].size;

            if (emptyCPU){
              currProcess = pidCounter;
              emptyCPU = false;          
              processes[pidCounter].locationCode = "cpu";
              cout << "The CPU is now occupied!" << '\n' << '\n';
            }
            //If the CPU isn't empty and the user issues an 'A',
            //a process is created and added to the ready queue
            else {
              handleInterruptandSystemCall();

              //The current process is readded to the ready queue
              addProcessToReadyQueue(currProcess);
              //Its location code is changed
              processes[currProcess].locationCode = "r";

              //The new process is added to the ready queue
              addProcessToReadyQueue(pidCounter);
              //Its location code is changed to signify
              //its in the ready deque
              processes[pidCounter].locationCode = "r";

              currProcess = readyQueue.front();
              processes[currProcess].locationCode = "cpu";
              readyQueue.pop_front();
              emptyCPU = false;
            }
          }
          else {
            jobPool.push_back(pidCounter);
            processes[pidCounter].locationCode = "j";
          }
          ++(pidCounter);
        }
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
          if ((input[0]=='p' && !printerQueues.empty()) || (input[0]=='d' && !diskDeques0.empty())
           || (input[0]=='c' && !cdQueues.empty())){
            int num = 0;

            //If the function determines the user's input is valid,
            if(isSystemCallInputValid(input,num)){
              //The current process' burstEstimate and remainingBurst are updated
              //before it's added to a device queue
              handleInterruptandSystemCall();

              
              //If the input is valid, the user will be prompted
              //for system call paramters and the call will be
              //added to the appropriate device queue 
              //setSystemCallVariables(print, input[0], num);
              setSystemCallVariables(input[0]=='p', input[0], num);
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
        if (isSystemCallInputValid(input,num)){
          if (input[0]=='P'){
            checkForSystemCallinQueue(printerQueues, num);
          }
          else if (input[0]=='D'){
            if (scanDiskQueuesStatus[num-1] == 1){
              sort(diskDeques1[num-1].begin(),diskDeques1[num-1].end(),sortByHighestTrackFirst);
              checkForSystemCallinQueue(diskDeques1, num);
            }
            else {
              sort(diskDeques0[num-1].begin(),diskDeques0[num-1].end(),sortByLowestTrackFirst);
              checkForSystemCallinQueue(diskDeques0, num); 
            }
            //checkForSystemCallinDiskSet(num-1,scanDiskQueuesStatus[num-1]);
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
            cout << os.str();
            os.str("");
            os.clear();
            //SORT DISK QUEUES
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

            cout << os.str();
            os.str("");
            os.clear();

            snapshotAux_SystemInformation();
          }
        }
      }
      /*
        If the input's format is K<num>, where <num> is the
        pid of a process to be killed, the killProcess function
        will locate the process' pid in any of the possible deque
        locations, remove it, and erase the process from the processes map
      */
      else if (input[0] == 'K'){
        /*
        try {
          string pid_str = input.substr(1);
          int pidToErase = atoi(pid_str);
          killProcess(pidToErase);
        }
        catch(...){
          cerr << "The characters following 'K' don't form a valid integer." << '\n';
          cerr << "Enter a new command and try again" << '\n';
        }
        */
        int pidToErase;
        if (isSystemCallInputValid(input, pidToErase)){
          killProcess(pidToErase);
        }
      }
      else {
        cerr << "The characters entered are not a valid command." << '\n';
        cerr << "Please enter a new command and try again." << '\n' << '\n';
      }
    }
    return;
  }

  /*
    Prints a header for the snapshot function indicating what each value
     being printed signifies
  */
  void CPU::snapshotHeader(){
    os << "PID " << setw(10) << "Filename " << setw(10) << "Memstart " << setw(10) << "R/W " << setw(10) << "File Length " << 
    setw(10) << "Total CPU Time " << setw(10) << "Average Burst Time " << '\n';
  }

  /*
    Prints the data of each process in the chosen device queue
    Each process gets its own line
  */
  template<typename T>
  void CPU::snapshotPrint(T& itB, T& itE){
    while (itB != itE){
      string ty = processes[*itB].type;
      os << *itB << setw(10) << processes[*itB].name << setw(10) << processes[*itB].memStart
        << setw(10) << ty; 
      if (ty == "w"){
        os << setw(10) << processes[*itB].length;
      }
      else {
        os << setw(20);
      }
      os << processes[*itB].totalCPUTime << setw(10) << processes[*itB].totalCPUTime / processes[*itB].cpuUsageCount;
      os << '\n';
      ++itB;
    }
  }

  void CPU::snapshotAux(const string& input){
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
        os << "----" << input << count << '\n';
        snapshotPrint(itB, itE);
        ++itV;
        ++count;
        itB = itV->begin();
        itE = itV->end();
      }
      os << '\n' << '\n';
  }

  /*
    Will check if the system call input, which will have
    already been verified to begin with either a 'p', 'd',
    or 'c' is valid. The number following the chosen character
    will be checked to insure it is not negative and falls within
    the limits of the number of specific device queues present
  */
  bool CPU::isSystemCallInputValid(string& input, int& num){
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
          if (scanDiskQueuesStatus[num-1] == 1){
            return checkIfsysCallNumLargerThanDevQueue(diskDeques1,num);
          }
          return checkIfsysCallNumLargerThanDevQueue(diskDeques0,num);

          //return checkIfsysCallNumLargerThanSet(num);
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

  /*
    Prompts the user for system call parameters. If the system call is for
    a printer device (signified by the print bool), the function will not
    ask for certain parameters
  */
  void CPU::setSystemCallVariables(const bool& print, const char& ch, int& num){
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
      processes[currProcess].locationCode = "p" + to_string(num-1);
      (printerQueues[num-1]).push_back(currProcess);
    }
    else if (ch == 'd'){
      addProcessToDiskDeque(currProcess,num-1);
    }
    else { //ch == 'c'
      processes[currProcess].locationCode = "c" + to_string(num-1);
      cdQueues[num-1].push_back(currProcess);
    }
  }

  bool CPU::typeErrorChecking(string& typeIn){
    if (typeIn != "w" && typeIn != "r"){
      cerr << "The character entered were not 'w' or 'r'." << '\n';
      cerr << "Please enter a new command and try again." << '\n';
      return false;
    }
    else {
      return true;
    }
  }

  void CPU::checkForSystemCallinQueue(vector<deque<int>>& devQueues, const int& callNum){
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
          processes[finishedProcess].locationCode = "cpu";
        }
        else {
          addProcessToReadyQueue(finishedProcess);
          processes[finishedProcess].locationCode = "r";
        }
        cout << "A system call has completed" << '\n' << '\n';
      }
    }
    else {
      cerr << "There are no queues of this type. No processes exist in these queues." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';              
    }
  }

  /*
    Checks whether the 
  */
  bool CPU::checkIfsysCallNumLargerThanDevQueue(const vector<deque<int>>& devQueues, const int& callNum){
    if (callNum > static_cast<int>(devQueues.size())){
      cerr << "Number entered is larger than current number of chosen device queues." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
      return false;
    }
    else {
      return true;
    }
  }

/*
  bool CPU::checkIfsysCallNumLargerThanSet(const int& callNum){
    if (callNum > static_cast<int>(diskSets0.size())){
      cerr << "Number entered is larger than current number of chosen device queues." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
      return false;
    }
    else {
      return true;
    }
  }
*/

  void CPU::getCylinderChoice(const int& dequeNum){
    string in;
    cout << "Enter the cylinder that the file exists on: ";
    cin >> in;
    while (!intOrFloatErrorCheck(in, true, true) || !isCylinderChoiceValid(intResult,dequeNum)){
      cout << "The chosen cylinder is invalid. Please enter a new value and try again: ";
      cin >> in;
    }
  }

  bool CPU::isCylinderChoiceValid(const int& cylinderNum, const int& dequeNum){
    return cylinderNum < cylinderCount[dequeNum];
  }

  void CPU::snapshotAux_SystemInformation(){
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

  void CPU::snapshotAux_ReadyDeque(){
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


  void CPU::snapshotAux_Disk(){
    for (int i = 0; i < scanDiskQueuesStatus.size(); ++i){
      sort(diskDeques0[i].begin(),diskDeques1[i].end(),sortByHighestTrackFirst);
      sort(diskDeques0[i].begin(),diskDeques0[i].end(),sortByLowestTrackFirst);

      cout << "----" << "Scan Queue " << i+1 << '\n';
      if (scanDiskQueuesStatus[i] == 1){
        snapshotAux_Disk2(diskDeques1[i].begin(), diskDeques1[i].end());
        cout << '\n';
        cout << "----" << "Waiting Queue " << i+1 << '\n';
        snapshotAux_Disk2(diskDeques0[i].begin(), diskDeques0[i].end());
      }
      else {
        snapshotAux_Disk2(diskDeques0[i].begin(), diskDeques0[i].end());
        cout << '\n';
        cout << "----" << "Waiting Queue " << i+1 << '\n';
        snapshotAux_Disk2(diskDeques1[i].begin(), diskDeques1[i].end());
      }
      cout << '\n' << '\n';
    }
  }

  void CPU::snapshotAux_Disk2(deque<int>::iterator scanIt, deque<int>::iterator scanItEnd){
    while (scanIt != scanItEnd){
      cout << *scanIt << setw(10) << processes[*scanIt].name << setw(10) << processes[*scanIt].memStart 
        << setw(10) << processes[*scanIt].type << setw(10);
      if (processes[*scanIt].type == "w"){
        cout << processes[*scanIt].length << setw(10);
      }
      else {
        cout << setw(20);
      }
      cout << processes[*scanIt].totalCPUTime << setw(10) << 
      (processes[*scanIt].totalCPUTime / processes[*scanIt].cpuUsageCount) << '\n';
      ++scanIt;
    }
  }


  /*
    Used for the running phase input, the function will check
    if the user input entered to represent a integer can actually
    be represented an integer and if the integer is negative or not
  */
  bool CPU::intOrFloatErrorCheck(string in, const bool& checkingInt, const bool& zeroValuesOK){
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

  //Updates the current process' burstEstimate and remaining burst variables
  void CPU::handleInterruptandSystemCall(){
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
    unordered_map<int,Process>::iterator it = processes.find(currProcess);
    it->second.remainingBurst = it->second.burstEstimate - floatResult;
    it->second.burstEstimate = sjwAlgorithm();

    it->second.totalCPUTime += floatResult;
    ++(it->second.cpuUsageCount);

    //processes[currProcess].remainingBurst = processes[currProcess].burstEstimate - floatResult;
    //processes[currProcess].burstEstimate = sjwAlgorithm();

    //(processes[currProcess].totalCPUTime) += floatResult;
    //++(processes[currProcess].cpuUsageCount);
  }

  void CPU::terminateProcess(){
    if (emptyCPU){
      cerr << "The CPU is unoccupied, no process present to be terminated" << '\n' << '\n';
    }
    else {
      //Ask for time in CPU. Update burst time etc.

      unordered_map<int,Process>::iterator it = processes.find(currProcess);

      os << "Process terminated" << '\n';
      os << "------------------" << '\n';
      os << "PID " << setw(10) << "Total CPU Time " << setw(10) << "Average Burst Time " << '\n';
      os << currProcess << setw(10) << it->second.totalCPUTime << setw(20);
      if (it->second.cpuUsageCount > 0){
        os << (it->second.totalCPUTime / it->second.cpuUsageCount);
      }
      else {
        os << "0";
      }
      os << '\n' << '\n';

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
        os << "A new process has been added to the CPU." << '\n';
      }
      else {
        emptyCPU = true;
      }
      os << '\n';
    }
    cout << os.str();
    os.str("");
    os.clear();
  }

  /*
    When a new process arrives, the Long Term Scheduler is prompted for 
    its size. This size is error checked and then compared to the maximum
    allowable process size specified during Sys gen to insure it is valid
  */
  bool CPU::getProcessSize(){
    string input;
    cout << "Enter size (in words) of the process: ";
    cin >> input;
    while (!intOrFloatErrorCheck(input, true, false)){
      cin >> input;
    }
    if (intResult > totalMemorySize){
      cerr << "Process rejected" << '\n';
      cerr << "The number entered is larger than total memory size of " << totalMemorySize << '\n';
      cerr << "Please announce the arrival of a new process with a different number and and try again." << '\n';
      return false;
    }
    return true;
  }

  /*
    Finds process with the pid used as the parameter and erases it from
    the processes map and whatever deque it belongs to (job pool, ready deque etc)
  */
  void CPU::killProcess(const int& pid){
    string locationCode = processes[pid].locationCode;
    if (locationCode == "r"){
      findProcessToKill(pid, readyQueue);
    }
    else if (locationCode == "j"){
      findProcessToKill(pid, jobPool);
    }
    else if (locationCode[0] == 'c'){
      string dequeNum_str = locationCode.substr(1);
      int dequeNum = atoi(dequeNum_str.c_str()); 
      findProcessToKill(pid, cdQueues[dequeNum]);
    }
    else if (locationCode[0] == 'p'){
      string dequeNum_str = locationCode.substr(1);
      int dequeNum = atoi(dequeNum_str.c_str()); 
      findProcessToKill(pid, printerQueues[dequeNum]);
    }
    else { //locationCode[0] == 'd'
      string dequeNum_str = locationCode.substr(2);
      int dequeNum = atoi(dequeNum_str.c_str());
      if (locationCode[1] == '1'){
        findProcessToKill(pid, diskDeques1[dequeNum]);
      }
      else { // if locationCode[1] == '0'
        findProcessToKill(pid, diskDeques0[dequeNum]);
      }
    }
    processes.erase(pid);
  }

  /*
    Searches whatever deque the process' pid is in to remove it
  */
  void CPU::findProcessToKill(const int& pid, deque<int>& processLocation){
    deque<int>::iterator it = processLocation.begin();
    deque<int>::iterator itEnd = processLocation.end();
    while (it != itEnd){
      if (*it == pid){
        processLocation.erase(it);
        return;
      }
      ++it;
    }
    //Should never reach this point
    cout << "Process not found" << '\n';
  }