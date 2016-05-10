#ifndef CPU_H
#define CPU_H

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Memory.h"
using namespace std;

struct Process;

struct Cpu : public Memory {
  //VARIABLES

  //Used to store integer results of
  //intOrFloatErrorCheck
  int intResult;

  //Used to store float results of
  //intOrFloatErrorCheck
  float floatResult;

  //Used to print output from Snapshot function
  ostringstream os;

  //CONTRUCTORS
  Cpu();

  //FUNCTIONS
  void waitForInput();

  void snapshotHeader();

  /*
    Prints the data of each process in the chosen device deque
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
    the limits of the number of specific device deques present
  */
  bool isSystemCallInputValid(string& input, int& num);

  /*
    Prompts the user for system call parameters. If the system call is for
    a printer device (signified by the print bool), the function will not
    ask for certain parameters
  */
  void setSystemCallVariables(const bool& print, const char& ch, int& num);

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

  void checkForSystemCallinDeque(vector<deque<int>>& devDeques, const int& callNum);

  bool checkIfsysCallNumLargerThanDevDeque(const vector<deque<int>>& devDeques, const int& callNum);

  bool checkIfsysCallNumLargerThanSet(const int& callNum);

  //void addProcessToDiskDeque(const int& pid, const int& dequeNum);

  void addProcessToWaitingDeque(const int& pid, const int& dequeNum, const bool& zeroIsWaiting);

  void getCylinderChoice(const int& dequeNum);

  inline bool isCylinderChoiceValid(const int& cylinderNum, const int& dequeNum);

  void snapshotAux_SystemInformation();

  void snapshotAux_ReadyDeque();

  void snapshotAux_JobPool();

  void handleInterruptandSystemCall(const bool& burstIsComplete);

  void terminateProcess();

  void snapshotAux_Disk();

  void snapshotAux_Disk2(deque<int>::iterator scanIt, deque<int>::iterator scanItEnd);

  void snapshotAux_memoryInformation();

  bool getProcessSize();

  void killProcess(const int& pid);

  void findProcessToKill(const int& pid, deque<int>& processLocation);

  bool isStringValidHexNumber(const string& hex_str);

  bool isLogicalAddressInRange(const int& pid, const string& hex_str);

  void computePhysicalAddress(const int& pid, const string& hex_str);
  
  int searchForAndEraseJobThatFitsInMemory();

  bool checkForJobThatFitsInMemory();

  void addJobToMemory(const int& pid);

  void restoreFrameTableAndFreeFrameList(const int& pid);

  void addAsManyJobsAsPossibleToMemory();

  void printProcessInfo(const int& pid);

  bool chosenTrackFitsOnDisk(const int& track, const int& diskNum);
};

#endif
