#ifndef CPU_H
#define CPU_H

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct CPU {
  //VARIABLES

  //Used to store integer results of
  //intOrFloatErrorCheck
  int intResult;

  //Used to store float results of
  //intOrFloatErrorCheck
  float floatResult;

  //stores the pid of the current process
  int currProcess;

  //Used to determine whether a process is
  //occupying the CPU
  bool emptyCPU;

  //Used to print output from Snapshot function
  ostringstream os;

  //CONTRUCTORS
  CPU();

  //FUNCTIONS
  void waitForInput();

  void snapshotHeader();

  /*
    Prints the data of each process in the chosen device queue
    Each process gets its own line
  */
  template<typename T>
  void snapshotPrint(T& itB, T& itE);

  void snapshotAux(const string& input);

  /*
    Will check if the system call input, which will have
    already been verified to begin with either a 'p', 'd',
    or 'c' is valid. The number following the chosen character
    will be checked to insure it is not negative and falls within
    the limits of the number of specific device queues present
  */
  bool systemCallInputChecking(string& input, int& num);

  /*
    Prompts the user for system call parameters. If the system call is for
    a printer device (signified by the print bool), the function will not
    ask for certain parameters
  */
  void systemCallParameters(const bool& print, const char& ch, int& num);

  /*
    Used for the running phase input, the function will check
    if the user input entered to represent a integer can actually
    be represented an integer and if the integer is negative or not
  */
  bool intOrFloatErrorCheck(string in, const bool& checkingInt, const bool& zeroValuesOK);

  //Checks whether the type input is either of the accepted
  //values: 'r' or 'w'. If not, the boo goodInput remains
  //false and the system call request is rejected
  bool typeErrorChecking(string& typeIn);

  void checkForSystemCallinQueue(vector<deque<int>>& devQueues, const int& callNum);

  /*
    Checks whether the 
  */
  bool checkIfsysCallNumLargerThanDevQueue(const vector<deque<int>>& devQueues, const int& callNum);

  void addProcessToReadyQueue(const int& pid);

  void handleInterruptandSystemCall();

  float sjwAlgorithm();

  void terminateProcess();
};

#endif