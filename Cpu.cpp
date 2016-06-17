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
#include <ctype.h>
#include "Cpu.h"
#include "Memory.h"
#include "JobHandling.h"
#include "Snapshot.h"

using namespace std;

	Cpu::Cpu(Memory* ptr) : mPtr(ptr), s(ptr), j(ptr) {}

	//FUNCTIONS
	void Cpu::waitForInput(){
		mPtr->emptyCpu = true;
		string input = "";

		//If input == q, the function and program will close
		while (input != "q"){
			cout << "Enter a new command: ";
			cin >> input;
			cout << '\n';

			//If the Cpu is empty and the user issues an 'A',
			//a new process is created and added to the Cpu
			if (input == "A"){
				if (!mPtr->emptyCpu){
					handleInterruptandSystemCall(false);
					//The current process is readded to the ready deque
					mPtr->readySet.insert(mPtr->currProcess);
					//Its location code is changed
					mPtr->processes[mPtr->currProcess].locationCode = "r";
					mPtr->emptyCpu = true;
				}
				if (getProcessSize()){
					cout << "New process made!" << '\n';
					mPtr->processes.insert(make_pair(mPtr->pidCounter,Process(mPtr->pidCounter, mPtr->initialBurstEstimate, intResult)));
					mPtr->processes[mPtr->pidCounter].locationCode = "j";
					mPtr->jobPool.insert(mPtr->pidCounter);

					++(mPtr->pidCounter);

					//Looks in the job pool for jobs that
					//potentially fit in memory
					j.addAsManyJobsAsPossibleToMemory();
				}
				//Choose a process to put in the Cpu
				if (mPtr->readyDeque.size()){
					mPtr->currProcess = mPtr->readyDeque.front();
					mPtr->processes[mPtr->currProcess].locationCode = "cpu";
					mPtr->readyDeque.pop_front();
					mPtr->emptyCpu = false;
					cout << "The Cpu is now occupied!" << '\n' << '\n';
				}
			}

			//If the user tries to terminate a process
			//while the Cpu is empty
			else if (input == "t"){
				terminateProcess();

				j.addAsManyJobsAsPossibleToMemory();

				if (!mPtr->readyDeque.empty()){
					mPtr->currProcess = mPtr->readyDeque.front();
					mPtr->readyDeque.pop_front();
					mPtr->emptyCpu = false;
					cout << "A new process has been added to the Cpu." << '\n';
				}
				else {
					mPtr->emptyCpu = true;
				}
			}

			//If the user issues a system call in the form of either
			//p<number>, d<number>, or c<number>
			else if (input[0]=='p' || input[0]=='d' || input[0]=='c'){
				if (mPtr->emptyCpu){
					cerr << "The Cpu is empty, system calls cannot be made." <<'\n';
					cerr << "Add a process with the A command before issuing a system call." << '\n' << '\n';
				}

				else {
					if ((input[0]=='p' && !mPtr->printerDeques.empty()) || (input[0]=='d' && !mPtr->diskDeques0.empty())
					 || (input[0]=='c' && !mPtr->cdDeques.empty())){
						int num = 0;

						//If the function determines the user's input is valid,
						if(isSystemCallInputValid(input,num)){
							//The current process' burstEstimate and remainingBurst are updated
							//before it's added to a device deque
							handleInterruptandSystemCall(true);


							//If the input is valid, the user will be prompted
							//for system call paramters and the call will be
							//added to the appropriate device deque
							setSystemCallVariables(input[0], num);
							cout << "System call added!" << '\n' << '\n';
							if (!mPtr->readyDeque.empty()){
								mPtr->currProcess = mPtr->readyDeque.front();
								mPtr->readyDeque.pop_front();
								mPtr->emptyCpu = false;
							}
							else {
								mPtr->emptyCpu = true;
							}
						}
					}
					else {
						cerr << "There are no deques available of the chosen device." << '\n';
						cerr << "Please enter a new command and try again." << '\n' << '\n';
					}
				}
			}

			//If the user issues system call interrupt signal
			else if (input[0]=='P' || input[0] == 'D' || input[0] == 'C'){
				int num = 0;

				handleInterruptandSystemCall(false);
				mPtr->readySet.insert(mPtr->currProcess);
				mPtr->currProcess = mPtr->readyDeque.front();
				mPtr->readyDeque.pop_front();

				//If the user's input is determined to be valid
				if (isSystemCallInputValid(input,num)){
					if (input[0]=='P'){
						checkForSystemCallinDeque(mPtr->printerDeques, num);
					}
					else if (input[0]=='D'){
						int n = num-1;
						if (mPtr->scanDiskDequesStatus[n] == 1){
							checkForDiskSets(mPtr->diskSets1, n, true);
						}
						else {
							checkForDiskSets(mPtr->diskSets0, n, false);
						}
					}
					else if (input[0]=='C') {
						checkForSystemCallinDeque(mPtr->cdDeques, num);
					}
				}
			}

			/*If the user types 'S' for the snapshot function,
				the user will prompted for whether to print the contents
				of the ready, printer, cd, or disk deques. The contents of
				the chosen deque will read into an ostream object and
				printed to the terminal
			*/
			else if (input == "S"){
				if (!mPtr->emptyCpu){
				handleInterruptandSystemCall(false);
				mPtr->readySet.insert(mPtr->currProcess);
				mPtr->currProcess = mPtr->readyDeque.front();
				mPtr->readyDeque.pop_front();
				}

				s.handleInputChoice();
			}
			/*
				If the input's format is K<num>, where <num> is the
				pid of a process to be killed, the killProcess function
				will locate the process' pid in any of the possible deque
				locations, remove it, and erase the process from the processes map
			*/
			else if (input == "K"){
				string in;
				cout << "Enter the pid of the process to kill: ";
				cin >> in;
				while (!intOrFloatErrorCheck(in, true, true) && intResult < mPtr->pidCounter){
					cin >> in;
				}
				if (intResult == mPtr->currProcess){
					handleInterruptandSystemCall(true);
				}
				else if (!mPtr->emptyCpu){
					handleInterruptandSystemCall(false);
				}

				killProcess(intResult);
				j.addAsManyJobsAsPossibleToMemory();
			}
			else {
				cerr << "The characters entered are not a valid command." << '\n';
				cerr << "Please enter a new command and try again." << '\n' << '\n';
			}
		}
		return;
	}


	/*
		Will check if the system call input, which will have
		already been verified to begin with either a 'p', 'd',
		or 'c' is valid. The number following the chosen character
		will be checked to insure it is not negative and falls within
		the limits of the number of specific device deques present
	*/
	bool Cpu::isSystemCallInputValid(string& input, int& num){
		//Represents the string following either p,d, or c
		string dequeNum = input.substr(1);

		/*Contents of quenum are fed in num to insure it can
			be read as an integer. An error message is printed
			if this can't happen
		*/
		istringstream iss{dequeNum};
		if (iss >> num && (iss.eof() || isspace(iss.peek()))){
			if (num <= 0){
				cerr << "Chosen number is zero or negative. Please try again." << '\n' << '\n';
				return false;
			}
			else {
				/*checkIfsys
					Each statement will check whether num is larger than the number
					of chosen device deques present
				*/
				if (input[0] == 'p' || input[0] == 'P'){
					return checkIfsysCallNumLargerThanDevDeque(mPtr->printerDeques, num);

				}
				else if (input[0] == 'd' || input[0] == 'D'){
					if (mPtr->scanDiskDequesStatus[num-1] == 1){
						return checkIfsysCallNumLargerThanDevDeque(mPtr->diskDeques1, num);
					}
					return checkIfsysCallNumLargerThanDevDeque(mPtr->diskDeques0, num);

					//return checkIfsysCallNumLargerThanSet(num);
				}
				else { //input[0] == 'c' || input[0] == 'C'
					return checkIfsysCallNumLargerThanDevDeque(mPtr->cdDeques, num);
				}
			}
		}
		/*
			If dequenum could not be read as integer into num
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
	void Cpu::setSystemCallVariables(const char& ch, int& num){
		string name = "";
		cout << "Enter the file name: ";
		cin >> name;
		cout << '\n';
		if (name.length() > 20){
			name.resize(20);
		}
		mPtr->processes[mPtr->currProcess].name = name;

		string memStart = "";
		int n = 0;

		cout << "Enter the starting location in memory: ";
		cin >> memStart;
		while (!isStringValidHexNumber(memStart) || !isLogicalAddressInRange(mPtr->currProcess, memStart)){
			cin >> memStart;
		}
		mPtr->processes[mPtr->currProcess].logicalMemoryAddress = memStart;
		computePhysicalAddress(mPtr->currProcess, memStart);
		cout << "Physical address: " << hex << "0x" << mPtr->processes[mPtr->currProcess].physicalAddress << '\n' << '\n';

		/*
			If the system call is not for a printing device,
			then the user will promted to confirm whether the action
			is a read or a write. Error checking will be done to insure
			that the user's input is either 'r' or 'w'. If the system call
			is for printer devices, then the type is automatically 'w'
		*/
		string typeIn = "";
		if (!(ch=='p')){
			cout << "Enter r if your action is a read or enter w if your action is a write: ";
			cin >> typeIn;
			while (!typeErrorChecking(typeIn)){
				cin >> typeIn;
			}
			mPtr->processes[mPtr->currProcess].type = typeIn;
		}
		else {
			mPtr->processes[mPtr->currProcess].type = "w";
		}

		/*
			If the system is a write action, then
			the user will be prompted to enter the
			length of the file. The answer will be
			verified as a valid interger.
		*/
		if (strcmp(mPtr->processes[mPtr->currProcess].type.c_str(),"w")==0){
			string input;

			cout << '\n' << "Enter the length of the file: ";
			cin >> input;
			while (!intOrFloatErrorCheck(input, true, false)){
				cin >> input;
			}
			mPtr->processes[mPtr->currProcess].length = intResult;
		}

		if (ch == 'd'){
			string input;
			cout << '\n' << "Enter the track the process exists on: ";
			cin >> input;
			while (!intOrFloatErrorCheck(input, true, true)|| !chosenTrackFitsOnDisk(intResult, num-1)){
		cin >> input;
			}
			mPtr->processes[mPtr->currProcess].track = intResult;
		}

		/*
			When the user has been prompted for all
			system call parameters, then the process
			will be added to the appropiate device deque
		*/
		if (ch == 'p'){
			mPtr->processes[mPtr->currProcess].locationCode = "p" + to_string(num-1);
			(mPtr->printerDeques[num-1]).push_back(mPtr->currProcess);
		}
		else if (ch == 'd'){
			addProcessToDiskDeque(mPtr->currProcess, num-1);
		}
		else { //ch == 'c'
			mPtr->processes[mPtr->currProcess].locationCode = "c" + to_string(num-1);
			mPtr->cdDeques[num-1].push_back(mPtr->currProcess);
		}
	}

	bool Cpu::typeErrorChecking(string& typeIn){
		if (typeIn != "w" && typeIn != "r"){
			cerr << "The character entered were not 'w' or 'r'." << '\n';
			cerr << "Please enter a new command and try again." << '\n';
			return false;
		}
		else {
			return true;
		}
	}

	void Cpu::checkForSystemCallinDeque(vector<deque<int>>& devDeques, const int& callNum){
		if (!devDeques.empty()){
			if (devDeques[callNum-1].empty()){
				cerr << "No system calls are currently in the chosen deque " << callNum << '\n' << '\n';
			}

			//The system call at the front of the deque is removed
			else {
				int finishedProcess = devDeques[callNum-1].front();
				devDeques[callNum-1].pop_front();
				if (mPtr->emptyCpu){
					mPtr->currProcess = finishedProcess;
					mPtr->emptyCpu = false;
					mPtr->processes[finishedProcess].locationCode = "cpu";
				}
				else {
					mPtr->readySet.insert(finishedProcess);
					mPtr->processes[finishedProcess].locationCode = "r";
				}
				cout << "A system call has completed" << '\n' << '\n';
			}
		}
		else {
			cerr << "There are no deques of this type. No processes exist in these deques." << '\n';
			cerr << "Please enter a new command and try again." << '\n' << '\n';
		}
	}


	bool Cpu::checkIfsysCallNumLargerThanDevDeque(const vector<deque<int>>& devDeques, const int& callNum){
		if (callNum > static_cast<int>(devDeques.size())){
			cerr << "Number entered is larger than current number of chosen device deques." << '\n';
			cerr << "Please enter a new command and try again." << '\n' << '\n';
			return false;
		}
		else {
			return true;
		}
	}

	void Cpu::getCylinderChoice(const int& dequeNum){
		string in;
		cout << "Enter the cylinder that the file exists on: ";
		cin >> in;
		while (!intOrFloatErrorCheck(in, true, true) || !isCylinderChoiceValid(intResult,dequeNum)){
			cout << "The chosen cylinder is invalid. Please enter a new value and try again: ";
			cin >> in;
		}
	}

	bool Cpu::isCylinderChoiceValid(const int& cylinderNum, const int& dequeNum){
		return cylinderNum < mPtr->cylinders[dequeNum];
	}


	/*
		Used for the running phase input, the function will check
		if the user input entered to represent a integer can actually
		be represented an integer and if the integer is negative or not
	*/
	bool Cpu::intOrFloatErrorCheck(string in, const bool& checkingInt, const bool& zeroValuesOK){
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
	void Cpu::handleInterruptandSystemCall(const bool& burstIsComplete){
		//Asking timer how long the current process has used the Cpu
		string in;
		cout << "How long has the current process been using the Cpu? ";
		cin >> in;
		cout << '\n';

		//Keeps asking for valid input until it is received
		while (!intOrFloatErrorCheck(in, false, true)){
			cin >> in;
		}

		//The current process' remaining burst and burst estimate are updated
		unordered_map<int,Process>::iterator it = mPtr->processes.find(mPtr->currProcess);
		it->second.totalCpuTime += floatResult;

		if (!burstIsComplete){
			it->second.remainingBurst = it->second.remainingBurst - floatResult;
		}
		else {
			++(it->second.cpuUsageCount);
			it->second.burstEstimate = sjwAlgorithm();
			it->second.remainingBurst = it->second.burstEstimate;
		}
	}

	void Cpu::terminateProcess(){
		if (mPtr->emptyCpu){
			cerr << "The Cpu is unoccupied, no process present to be terminated" << '\n' << '\n';
		}
		else {
			//Ask for time in Cpu. Update burst time etc.
			handleInterruptandSystemCall(true);

			unordered_map<int,Process>::iterator it = mPtr->processes.find(mPtr->currProcess);

			//Memory used by the process is returned to the freeMemory counter
			mPtr->freeMemory += it->second.size;

			os << "Process terminated" << '\n';
			os << "------------------" << '\n';
			os << "PID " << setw(10) << "Total Cpu Time " << setw(10) << "Average Burst Time " << '\n';
			os << mPtr->currProcess << setw(10) << it->second.totalCpuTime << setw(20);
			if (it->second.cpuUsageCount > 0){
				os << (it->second.totalCpuTime / it->second.cpuUsageCount);
			}
			else {
				os << "0";
			}
			os << '\n' << '\n';

			//The system's total cpu time and cpu usage count variables are updated with the
			//terminated process' corresponding variables. This updates the system's average
			//total Cpu time
			if (it->second.cpuUsageCount > 0){
				mPtr->systemTotalCpuTime += it->second.totalCpuTime;
				mPtr->systemTotalcpuUsageCount += it->second.cpuUsageCount;
			}

			mPtr->processes.erase(mPtr->currProcess);

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
	bool Cpu::getProcessSize(){
		string input;
		cout << "Enter size (in words) of the process: ";
		cin >> input;
		while (!intOrFloatErrorCheck(input, true, false)){
			cin >> input;
		}
		if (intResult > mPtr->totalMemorySize){
			cerr << "Process rejected" << '\n';
			cerr << "The number entered is larger than total memory size of " << mPtr->totalMemorySize << '\n';
			cerr << "Please announce the arrival of a new process with a different number and and try again." << '\n' << '\n';
			return false;
		}
		if (intResult > mPtr->maximumProcessSize){
			cerr << "Process rejected" << '\n';
			cerr << "The number entered is larger than the largest process size specified (" <<
				mPtr->maximumProcessSize << ")" << '\n';
			cerr << "Please announce the arrival of a new process with a different number and try again." << '\n' << '\n';
			return false;
		}
		return true;
	}

	/*
		Finds process with the pid used as the parameter and erases it from
		the processes map and whatever deque it belongs to (job pool, ready deque etc)
	*/
	void Cpu::killProcess(const int& pid){
		restoreFrameTableAndFreeFrameList(pid);
		mPtr->freeMemory += mPtr->processes[pid].size;

		string locationCode = mPtr->processes[pid].locationCode;
		if (locationCode == "cpu"){
			if (mPtr->readyDeque.size()){
				mPtr->currProcess = mPtr->readyDeque.front();
				mPtr->readyDeque.pop_front();
			}
			mPtr->currProcess = -1;
			mPtr->emptyCpu = true;
		}
		//Since every pid is unique, lower_bound or upper_bound can be used below
		//to find the pid in question
		else if (locationCode == "r"){
			set<int, Memory::SortByLowBurst>::iterator it = lower_bound(mPtr->readySet.begin(), mPtr->readySet.end(), pid);
			mPtr->readySet.erase(it);
		}
		else if (locationCode == "j"){
			set<int, Memory::SortBySize>::iterator it = lower_bound(mPtr->jobPool.begin(), mPtr->jobPool.end(), pid);
			mPtr->jobPool.erase(it);
		}
		else if (locationCode[0] == 'c'){
			string dequeNum_str = locationCode.substr(1);
			int dequeNum = atoi(dequeNum_str.c_str());

			deque<int>::iterator it = lower_bound(mPtr->cdDeques[dequeNum].begin(), mPtr->cdDeques[dequeNum].end(), pid);
			mPtr->cdDeques[dequeNum].erase(it);
		}
		else if (locationCode[0] == 'p'){
			string dequeNum_str = locationCode.substr(1);
			int dequeNum = atoi(dequeNum_str.c_str());

			deque<int>::iterator it = lower_bound(mPtr->printerDeques[dequeNum].begin(), mPtr->printerDeques[dequeNum].end(), pid);
			mPtr->printerDeques[dequeNum].erase(it);
		}
		else { //locationCode[0] == 'd'
			string dequeNum_str = locationCode.substr(2);
			int dequeNum = atoi(dequeNum_str.c_str());

			if (locationCode[1] == '1'){
				set<int,Memory::SortByHighTrack>::iterator it = lower_bound(mPtr->diskSets1[dequeNum].begin(), mPtr->diskSets1[dequeNum].end(), pid);
				mPtr->diskSets1[dequeNum].erase(it);
			}
			else { // if locationCode[1] == '0'
				set<int,Memory::SortByLowTrack>::iterator it = lower_bound(mPtr->diskSets0[dequeNum].begin(), mPtr->diskSets0[dequeNum].end(), pid);
				mPtr->diskSets0[dequeNum].erase(it);
			}
		}
		printProcessInfo(intResult);
		mPtr->processes.erase(pid);
	}


	//Checks if a string is a valid hexadecimal number
	bool Cpu::isStringValidHexNumber(const string& hex_str){
		string sub = "";
		if (hex_str[0] == '0' && hex_str[1] == 'x'){
			sub = hex_str.substr(2);
		}
		else {
			sub = hex_str;
		}
		for (auto i: sub){
			if (!isxdigit(i)){
				cerr << "The memory location entered is not a valid hexadecimal number." << '\n';
				cerr << "Enter a new hexadecimal value and try again." << '\n';
				return false;
			}
		}
		return true;
	}


	//Checks if the chosen logical address is valid for the process in question
	bool Cpu::isLogicalAddressInRange(const int& pid, const string& hex_str){
		string sub = "";
		if (hex_str[0] == '0' && hex_str[1] == 'x'){
			sub = hex_str.substr(2);
		}
		else {
			sub = hex_str;
		}
		int converted = (int)strtol(sub.c_str(),nullptr,16);//convert to decimal
		if (converted >= 0 && converted <= mPtr->pageSize * mPtr->processes[pid].pageTable.size()){
			return true;
		}
		cerr << "The logical address entered isn't in range of the process' size" << '\n';
		cerr << "Enter a new hex value and try again." << '\n';
		return false;
	}


	//Uses the logical hex memory to computer a corresponding physical address
	//in decimal
	void Cpu::computePhysicalAddress(const int& pid, const string& hex_str){
		//Use 0 instead of 16 for third parameter if the hex
		//string begins with 0x
		unordered_map<int,Process>::iterator it = mPtr->processes.find(pid);
		int decimalValue;
		string subbed = "";
		if (hex_str[0] == '0' && hex_str[1] == 'x'){
			subbed = hex_str.substr(2);
		}
		else {
			subbed = hex_str;
		}
		decimalValue = (int)strtol(subbed.c_str(),nullptr,16);

		int offset = decimalValue % mPtr->pageSize;
		int pageCount = it->second.pageTable.size();
		int correspondingPage;
		int i = 1;
		bool found = false;
		while (i < pageCount+1 && !found){
			if (decimalValue < i * mPtr->pageSize){
				correspondingPage = i-1;
				found = true;
			}
			++i;
		}
		int physicalPage = it->second.pageTable[correspondingPage];
		mPtr->processes[pid].physicalAddress = (physicalPage+1 * mPtr->pageSize) + offset;
	}


	void Cpu::restoreFrameTableAndFreeFrameList(const int& pid){
		vector<int>::iterator it = mPtr->processes[pid].pageTable.begin();
		vector<int>::iterator itEnd = mPtr->processes[pid].pageTable.end();

		while (it != itEnd){
			//Reset the pid and page vector slots
			//to -1
			mPtr->frameTable[*it] = {-1,-1};

			//Add the missing frames back to the
			//freeFrameList
			mPtr->freeFrameList.push_back(*it);
			++it;
		}
	}


	void Cpu::printProcessInfo(const int& pid){
		unordered_map<int,Process>::iterator it = mPtr->processes.find(pid);

		//Memory used by the process is returned to the freeMemory counter
		mPtr->freeMemory += it->second.size;

		os << "Process killed" << '\n';
		os << "------------------" << '\n';
		os << "PID " << setw(10) << "Total Cpu Time " << setw(10) << "Average Burst Time " << '\n';
		os << pid << setw(10) << it->second.totalCpuTime << setw(20);
		if (it->second.cpuUsageCount > 0){
			os << (it->second.totalCpuTime / it->second.cpuUsageCount);
		}
		else {
			os << "0";
		}
		os << '\n' << '\n';

		//The system's total cpu time and cpu usage count variables are updated with the
		//terminated process' corresponding variables. This updates the system's average
		//total Cpu time
		if (it->second.cpuUsageCount > 0){
			mPtr->systemTotalCpuTime += it->second.totalCpuTime;
			mPtr->systemTotalcpuUsageCount += it->second.cpuUsageCount;
		}
		cout << os.str();
		os.str("");
		os.clear();
	}

 	bool Cpu::chosenTrackFitsOnDisk(const int& track, const int& diskNum){
		if (track < mPtr->cylinders[diskNum]){return true;}
	 	cerr << "The track entered is higher than the number of tracks on the disk" << '\n';
	 	cerr << "Enter a new track number and try again." << '\n';
	 	return false;
 	}
 	

	template <typename T>
	void Cpu::checkForAndRemoveSystemCallinSet(const T& it){
		if (it->empty()){
			cerr << "No system calls are currently in the chosen deque" << '\n' << '\n';
			return;
		}
		//Else the system call at the front of the set is removed
		int finishedProcess = *(it->begin());
		it->erase(it->begin());
		if (mPtr->emptyCpu){
			mPtr->currProcess = finishedProcess;
			mPtr->emptyCpu = false;
			mPtr->processes[finishedProcess].locationCode = "cpu";
		}
		else {
			mPtr->readySet.insert(finishedProcess);
			mPtr->processes[finishedProcess].locationCode = "r";
		}
		cout << "A system call has completed" << '\n' << '\n';
	}


	//Returns the result of the algorithm based on the current process' values
	float Cpu::sjwAlgorithm(){
    	return (1 - mPtr->historyParameter) * mPtr->processes[mPtr->currProcess].burstEstimate + mPtr->historyParameter * floatResult;
  	}


  	void Cpu::addProcessToDiskDeque(const int& pid, const int& dequeNum){
    	if (mPtr->firstDiskSystemCall){
      		mPtr->diskSets0[dequeNum].insert(pid);
      		mPtr->firstDiskSystemCall = false;
    	}
    	else {
      		if (mPtr->scanDiskDequesStatus[dequeNum] == 1){
        		mPtr->processes[pid].locationCode = "d0" + to_string(dequeNum);
       			mPtr->diskSets0[dequeNum].insert(pid);
      		}
      		else {
        		mPtr->processes[pid].locationCode = "d1" + to_string(dequeNum);
        		mPtr->diskSets1[dequeNum].insert(pid);
      		}
    	}
  	}


	template <typename T>
	void Cpu::checkForDiskSets(const vector<multiset<int,T>>& diskSets, const int& num, const bool& isSet1){
  		if (diskSets.empty()){
			cerr << "There are no queues of this type available" << '\n';
			cerr << "Please enter a new command and try again" << '\n';
		}
		else {
			if (isSet1){
				vector<multiset<int,Memory::SortByHighTrack>>::iterator it = mPtr->diskSets1.begin() + (num);
				checkForAndRemoveSystemCallinSet(it);	
			}
			else {
				vector<multiset<int,Memory::SortByLowTrack>>::iterator it = mPtr->diskSets0.begin() + (num);
				checkForAndRemoveSystemCallinSet(it);	
			}
		}
	}