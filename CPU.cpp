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
#include "Memory.h"
#include "CPU.h"
using namespace std;
  //Helps give CPU functions access
  //to Memory class variables
  Memory m;

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
        cout << "New process made!" << '\n';
        m.processes.insert(make_pair(m.pidCounter,Process(m.pidCounter,m.initialBurstEstimate)));
        if (m.emptyCPU){
          m.currProcess = m.pidCounter;
          m.emptyCPU = false;          
          cout << "The CPU is now occupied!" << '\n' << '\n';
          //If the CPU is already occupied, the
          //process is added to the ready queue
        }
        //If the CPU isn't empty and the user issues an 'A',
        //a process is created and added to the ready queue
        else {
          m.handleInterruptandSystemCall();

          //The current process is readded to the ready queue
          m.addProcessToReadyQueue(m.currProcess);

          //The new process is added to the ready queue
          m.addProcessToReadyQueue(m.pidCounter);

          m.currProcess = m.readyQueue.front();
          m.readyQueue.pop_front();
          m.emptyCPU = false;
        }
        ++(m.pidCounter);
      }
      
      //If the user tries to terminate a process
      //while the CPU is empty
      else if (input == "t"){
        m.terminateProcess();
      }

      //If the user issues a system call in the form of either
      //p<number>, d<number>, or c<number>
      else if (input[0]=='p' || input[0]=='d' || input[0]=='c'){
        if (m.emptyCPU){
          cerr << "The CPU is empty, system calls cannot be made." <<'\n';
          cerr << "Add a process with the A command before issuing a system call." << '\n' << '\n';
        }

        else {
          if (m.checkForQueues(input)){
            int num = 0;

            //If the function determines the user's input is valid,
            if(systemCallInputChecking(input,num)){
              //The current process' burstEstimate and remainingBurst are updated
              //before it's added to a device queue
              m.handleInterruptandSystemCall();

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
              if (!m.readyQueue.empty()){
                m.currProcess = m.readyQueue.front();
                m.readyQueue.pop_front();
                m.emptyCPU = false;
              }
              else {
                m.emptyCPU = true;
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

        m.handleInterruptandSystemCall();
        m.addProcessToReadyQueue(m.currProcess);
        m.currProcess = m.readyQueue.front();
        m.readyQueue.pop_front();

        //If the user's input is determined to be valid
        if (systemCallInputChecking(input,num)){
          if (input[0]=='P'){
            checkForSystemCallinQueue(m.printerQueues, num);
          }
          else if (input[0]=='D'){
            m.checkForSystemCallinDiskSet(num-1,m.scanDiskQueuesStatus[num-1]);
          }
          else if (input[0]=='C') {
            checkForSystemCallinQueue(m.cdQueues, num);
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
        m.handleInterruptandSystemCall();
        m.addProcessToReadyQueue(m.currProcess);
        m.currProcess = m.readyQueue.front();
        m.readyQueue.pop_front();

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
            m.snapshotAux_Disk();
            m.snapshotAux_SystemInformation();
          }
          else if (input == "r"){
            m.snapshotAux_ReadyDeque();
            m.snapshotAux_SystemInformation();
          }
          else {

            snapshotHeader();
            snapshotAux(input);

            cout << os.str();
            os.str("");
            os.clear();

            m.snapshotAux_SystemInformation();
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
      string ty = m.processes[*itB].type;
      os << *itB << setw(10) << m.processes[*itB].name << setw(10) << m.processes[*itB].memStart
        << setw(10) << ty; 
      if (ty == "w"){
        os << setw(10) << m.processes[*itB].length;
      }
      else {
        os << setw(20);
      }
      os << m.processes[*itB].totalCPUTime << setw(10) << m.processes[*itB].totalCPUTime / m.processes[*itB].cpuUsageCount;
      os << '\n';
      ++itB;
    }
  }

  void CPU::snapshotAux(const string& input){
    vector<deque<int>>::iterator itV, itVe;
    deque<int>::iterator itB, itE;
      if (input == "c"){
        if (!m.cdQueues.empty()){
          itV = m.cdQueues.begin(); itVe = m.cdQueues.end();
          itB = itV->begin(); itE = itV->end();
        }
      }
      else { //input == "p"
        if (!m.printerQueues.empty()){
          itV = m.printerQueues.begin(); itVe = m.printerQueues.end();
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
  bool CPU::systemCallInputChecking(string& input, int& num){
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
          return checkIfsysCallNumLargerThanDevQueue(m.printerQueues, num);

        }
        else if (input[0] == 'd' || input[0] == 'D'){
          return checkIfsysCallNumLargerThanSet(num);
        }
        else { //input[0] == 'c' || input[0] == 'C'
          return checkIfsysCallNumLargerThanDevQueue(m.cdQueues, num);          
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
  void CPU::systemCallParameters(const bool& print, const char& ch, int& num){
    string name = "";
    cout << "Enter the file name: ";
    cin >> name;
    cout << '\n';
    if (name.length() > 20){
      name.resize(20);
    }
    m.processes[m.currProcess].name = name;

    string memStart = "";
    int n = 0;

    cout << "Enter the starting location in memory: ";
    cin >> memStart;
    while (!m.intOrFloatErrorCheck(memStart, true, true)){
      cin >> memStart;
    }
    m.processes[m.currProcess].memStart = intResult;

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
      m.processes[m.currProcess].type = typeIn;
    }
    else {
      m.processes[m.currProcess].type = "w";
    }

    /*
      If the system is a write action, then
      the user will be prompted to enter the
      length of the file. The answer will be
      verified as a valid interger.
    */
    if (strcmp(m.processes[m.currProcess].type.c_str(),"w")==0){
      string input;

      cout << '\n' << "Enter the length of the file: ";
      cin >> input;
      while (!m.intOrFloatErrorCheck(input, true, false)){
        cin >> input;
      }
      m.processes[m.currProcess].length = intResult;
    }

    /*
      When the user has been prompted for all
      system call parameters, then the process
      will be added to the appropiate device queue
    */
    if (ch == 'p'){
      (m.printerQueues[num-1]).push_back(m.currProcess);
    }
    else if (ch == 'd'){
      getCylinderChoice(num-1);
      m.addProcessToDiskQueue(m.currProcess,num-1);
    }
    else { //ch == 'c'
      m.cdQueues[num-1].push_back(m.currProcess);
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
        if (m.emptyCPU){
          m.currProcess = finishedProcess;
          m.emptyCPU = false;
        }
        else {
          m.addProcessToReadyQueue(finishedProcess);
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

  bool CPU::checkIfsysCallNumLargerThanSet(const int& callNum){
    if (callNum > static_cast<int>(m.diskSets0.size())){
      cerr << "Number entered is larger than current number of chosen device queues." << '\n';
      cerr << "Please enter a new command and try again." << '\n' << '\n';
      return false;
    }
    else {
      return true;
    }
  }

  void CPU::getCylinderChoice(const int& dequeNum){
    string in;
    cout << "Enter the cylinder that the file exists on: ";
    cin >> in;
    while (!m.intOrFloatErrorCheck(in, true, true) || !isCylinderChoiceValid(intResult,dequeNum)){
      cout << "The chosen cylinder is invalid. Please enter a new value and try again: ";
      cin >> in;
    }
  }

  bool CPU::isCylinderChoiceValid(const int& cylinderNum, const int& dequeNum){
    return cylinderNum < m.cylinderCount[dequeNum];
  }