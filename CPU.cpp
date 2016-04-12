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
#include "Memory.h"
using namespace std;

struct CPU : public Memory {
  //VARIABLES

  //stores the pid of the current process
  int currProcess;

  //Used to determine whether a process is
  //occupying the CPU
  bool emptyCPU;

  //Used to print output from Snapshot function
  ostringstream os;

  //CONTRUCTORS
  CPU() : currProcess(-1), emptyCPU(true) {}

  //FUNCTIONS
  void waitForInput(){
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
	//A new process is created and added to the processes unordered map
	processes.insert(make_pair(pidCounter,Process(pidCounter, initialBurstEstimate)));
	//If the CPU is empty, the newly created process is added to the CPU
	if (emptyCPU){
          currProcess = pidCounter;
          emptyCPU = false;          
          cout << "The CPU is now occupied!" << '\n' << '\n';
        }
        //If the CPU is already occupied, the
        //process is added to the ready queue
        else {
	  askTimer();
	  //The processes new estimated burst is calculated
	  int timeInCPU = askTimer();
	  processes[currProcess].totalBurst -= timeInCPU;
	  //The process' cpuCount increments
	  ++processes[currProcess].cpuUsageCount;
	  //The average burst variable is updated with timer's burst time
	  processes[currProcess].averageBurst = timeInCPU;
	  //Recalculating the current process' place in the
	  //ready queue given its new burst
	  addProcessToReadyQueue(currProcess);
	  //Calculating the new process' place in the ready queue
          addProcessToReadyQueue(pidCounter);
	  //The process with the shortest burst time in the ready
	  //queue enters the CPU
	  currProcess = readyQueue.front();
	  //The shortest burst process is removed from the ready queue
	  readyQueue.pop_front();
        }
        ++pidCounter;
      }
      
      //If the user tries to terminate a process
      //while the CPU is empty
      else if (input == "t"){
        if (emptyCPU){
          cerr << "The CPU is unoccupied, no process present to be terminated" << '\n' << '\n';
        }
        else {
          processes.erase(currProcess);
          cout << "Process terminated" << '\n';
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

      //If the user issues a system call in the form of either
      //p<number>, d<number>, or c<number>
      else if (input[0]=='p' || input[0]=='d' || input[0]=='c'){
        if (emptyCPU){
          cerr << "The CPU is empty, system calls cannot be made." <<'\n';
          cerr << "Add a process with the A command before issuing a system call." << '\n' << '\n';
        }

        else {
          if ((input[0] == 'p' && !printerQueues.empty()) || (input[0] == 'c' && !cdQueues.empty()) ||
            (input[0] == 'd' && !diskQueues.empty())){
            int num = 0;

            //If the function determines the user's input is valid,
            if(systemCallInputChecking(input,num)){
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
	      askTimer();
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

        //If the user's input is determined to be valid
        if (systemCallInputChecking(input,num)){
	  /*
	    If there is a process occupying the CPU, the timer will
	    be prompted for the amount of time the process has
	    spent in the CPU. Its burst will then be recalculated
	    and it will be added to the ready queue. This allows the
	    ready queue to sort its contents based on shortest burst
	    time, then pushing the process at the front of the queue
	    in the ready queue into the CPU
	  */
	  if (!emptyCPU){
	    //Get amount of time process was in CPU
	    int timeInCPU = askTimer();
	    //Tao is recalculated by subtracting the amount of time spent in the CPU
	    processes[currProcess].totalBurst -= timeInCPU;
	    //To recalculate the average burst to account for the new CPU usage time,
	    //tempAvgBurst is used by remultiplying the average burst and old cpuUsageCount
	    double tempAvgBurst = processes[currProcess].averageBurst * processes[currProcess].cpuUsageCount;
	    //The new cpu usage time is added to tempAvgBurst
	    tempAvgBurst += timeInCPU;
	     //Increment the number of times the process was in the CPU
	    ++processes[currProcess].cpuUsageCount;
	    //The new average burst is calculated with the new cpuUsageCounta and tempAvgBurst
	    processes[currProcess].averageBurst = tempAvgBurst / processes[currProcess].cpuUsageCount;
	    addProcessToReadyQueue(currProcess);
	    currProcess = readyQueue.front();
	    readyQueue.pop_front();
	  }
          if (input[0]=='P'){
            checkForSystemCallinQueue(printerQueues, num);
          }
          else if (input[0]=='D'){
            checkForSystemCallinQueue(diskQueues, num);
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
        cout << "Enter r, p, c, or d: ";
        cin >> input; cout << '\n';
        if (input != "r" && input != "p" && input != "c" && input != "d"){
          cerr << "The characters entered are not supported by Snapshot." << '\n';
          cerr << "Enter a new command and try again." << '\n' << '\n';
        }

        else {
          if (input == "r"){
            deque<int>::iterator itB = readyQueue.begin();
            deque<int>::iterator itE = readyQueue.end();

            os << "PID" << '\n' << "----r" << '\n';
            while (itB != itE){
              os << *itB << '\n';
              ++itB;
            }
            os << '\n';
          }

          else {
            snapshotHeader();
            snapshotAux(input);
          }
          cout << os.str();

          os.str("");
          os.clear();
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
  void snapshotHeader(){
    os << "PID " << setw(10) << "Filename " << setw(10) << "Memstart " << setw(10) << "R/W " << setw(10) << "File Length" << '\n';
  }

  /*
    Prints the data of each process in the chosen device queue
    Each process gets its own line
  */
  template<typename T>
  void snapshotPrint(T& itB, T& itE){
    while (itB != itE){
      string ty = processes[*itB].type;
      os << *itB << setw(10) << processes[*itB].name << setw(10) << processes[*itB].memStart
        << setw(10) << ty; 
      if (ty == "w"){
        os << setw(10) << processes[*itB].length;
      }
      os << '\n';
      ++itB;
    }
  }

  /*
    Checks whether the device queues are empty and returns
    if so. Otherwise, the function will set up to loop through
    the chosen device queues and feed the system call data into
    the ostringstream object os to be printed as output
  */
  void snapshotAux(const string& input){
    vector<deque<int>>::iterator itV, itVe;
    deque<int>::iterator itB, itE;

    if (input == "c"){
      if (cdQueues.empty()){
        return;
      }
      itV = cdQueues.begin(); itVe = cdQueues.end();
      itB = itV->begin(); itE = itV->end();
    }
    else if (input == "d"){
      if (diskQueues.empty()){
        return;
      }
      itV = diskQueues.begin(); itVe = diskQueues.end();
      itB = itV->begin(); itE = itV->end(); 
    }
    else { //input == "p"
      if (printerQueues.empty()){
        return;
      }
      itV = printerQueues.begin(); itVe = printerQueues.end();
      itB = itV->begin(); itE = itV->end();        
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
    os << '\n';      
  }

  /*
    Will check if the system call input, which will have
    already been verified to begin with either a 'p', 'd',
    or 'c' is valid. The number following the chosen character
    will be checked to insure it is not negative and falls within
    the limits of the number of specific device queues present
  */
  bool systemCallInputChecking(string& input, int& num){
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
          return checkIfsysCallNumLargerThanDevQueue(diskQueues, num);
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
  void systemCallParameters(const bool& print, const char& ch, int& num){
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
    /*
      intErrorCheck will return false as long as the function
      determines that the input entered in memStart can't
      be converted into an integer
    */

    cout << "Enter the starting location in memory: ";
    cin >> memStart;
    
    while (!intErrorCheck(memStart, n, true)){
      cin >> memStart;
    }
    processes[currProcess].memStart = n;

    /*
      If the system call is not for a printing device,
      then the user will promted to confirm whether the action
      is a read or a write. Error checking will be done to insure
      that the user's input is either 'r' or 'w'. If the system call
      is for printer devices, then the type is automatically 'w'
    */
    string typeIn = "";
    if (!print){
      string typeIn;

      cout << "Enter r if your action is a read or enter w if your action is a write: ";
      cin >> typeIn;
      cout << '\n'; 
      
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
      int fileLength;

      cout << '\n' << "Enter the length of the file: ";
      cin >> input;
      cout << '\n';
      while (!intErrorCheck(input, fileLength, false)){
        cin >> input;
      }
      processes[currProcess].length = fileLength;
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
      diskQueues[num-1].push_back(currProcess);
    }
    else { //ch == 'c'
      cdQueues[num-1].push_back(currProcess);
    }
  }


  /*
    Used for the running phase input, the function will check
    if the user input entered to represent a integer can actually
    be represented an integer and if the integer is negative or not
  */
  bool intErrorCheck(string in, int& num, const bool& memLoc){
    istringstream iss{in};
    //Checks if the input can be converted to an int
    if (iss >> num && (iss.eof() || isspace(iss.peek()))) {
      if (!memLoc && num <= 0){
        cerr << "Zero or a negative number was entered. Please try again." << '\n';
        return false;
      }
      else if (memLoc && num < 0){
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

  //Checks whether the type input is either of the accepted
  //values: 'r' or 'w'. If not, the function call will return
  //false and the system call request is rejected
  bool typeErrorChecking(string& typeIn){
    if (typeIn != "w" && typeIn != "r"){
      cerr << "The character entered were not 'w' or 'r'." << '\n';
      cerr << "Please enter a new command and try again." << '\n';
      return false;
    }
    return true;
  }

  /*
    Checks whether there are system calls in device queues to respond to a system call
    completed interrupt. If not, an error message in printed. Otherwise, the first system
    call in the device queue is moved in the ready queue if the CPU is occupied. If the CPU
    is empty, then the process will be moved straight to the CPU
  */
  void checkForSystemCallinQueue(vector<deque<int>>& devQueues, const int& callNum){
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
          readyQueue.push_back(finishedProcess);
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
    Checks whether the system call made by the user tries to issue a process to a numbered queue
    that is bigger than the current number of specified device queues
  */
  bool checkIfsysCallNumLargerThanDevQueue(const vector<deque<int>>& devQueues, const int& callNum){
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
    Asks the user, who acts as the timer, how long a process
    has used the CPU for. This function is called whenever an
    interrupt occurs or a process enters the CPU
  */
  int askTimer(){
    string timeInput = "";
    int time = 0;

    cout << "Enter the amount of time this process has used the CPU: ";
    cin >> timeInput;
    while (!intErrorCheck(timeInput, time, -1)){
      cin >> timeInput;
    }
    return time;
  }

  /*
    Prompts the user for the number of the cylinder that has to be
    accessed when disk read or write system call is made. Used by
    the disk queue algorithm
  */
  void askForCylinder(){
    string cynInput = "";
    int cynlinderChoice = 0;

    cout << "Enter the number of the proper cylinder for access: ";
    cin >> cynInput;
    
    while (!intErrorCheck(cynInput, cynlinderChoice, 1)){
      cin >> cynInput;
    }
  }

  /*
    Used to estimate a process' future burst time 
  */
  double sjfApproximationAlgorithm(Process& proc){
    return (1-historyParameter) * initialBurstEstimate + historyParameter * proc.totalBurst; 
  }

  void addProcessToReadyQueue(const int& num){
    int insertBurstAmount = processes[num].totalBurst;
    deque<int>::iterator insertLimit = upper_bound(readyQueue.begin(), readyQueue.end(), insertBurstAmount);
    readyQueue.insert(insertLimit, num);
  }
};
