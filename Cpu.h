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
struct Snapshot;

struct Cpu {
	//VARIABLES

	//Used to store integer results of
	//intOrFloatErrorCheck
	int intResult;

	//Used to store float results of
	//intOrFloatErrorCheck
	float floatResult;

	//Used to print output from Snapshot function
	ostringstream os;

	//Used to access and modify variables in the Memory object
	Memory* mPtr;

	//CONTRUCTORS
	Cpu();
	Cpu(Memory m);

	//FUNCTIONS
	void waitForInput();

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
	void setSystemCallVariables(const char& ch, int& num);

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

	void addProcessToWaitingDeque(const int& pid, const int& dequeNum, const bool& zeroIsWaiting);

	void getCylinderChoice(const int& dequeNum);

	inline bool isCylinderChoiceValid(const int& cylinderNum, const int& dequeNum);

	void handleInterruptandSystemCall(const bool& burstIsComplete);

	void terminateProcess();

	bool getProcessSize();

	void killProcess(const int& pid);

	bool isStringValidHexNumber(const string& hex_str);

	bool isLogicalAddressInRange(const int& pid, const string& hex_str);

	void computePhysicalAddress(const int& pid, const string& hex_str);

	void restoreFrameTableAndFreeFrameList(const int& pid);

	void printProcessInfo(const int& pid);

	bool chosenTrackFitsOnDisk(const int& track, const int& diskNum);

	template <typename T>
	void checkForAndRemoveSystemCallinSet(const T& it);
};

#endif
