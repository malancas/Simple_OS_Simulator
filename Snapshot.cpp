#include <iomanip>
#include <set>
#include "Snapshot.h"
#include "Memory.h"

using namespace std;

Snapshot::Snapshot(Memory* ptr) : mPtr(ptr) {}

/*
		Prints a header for the snapshot function indicating what each value
		 being printed signifies
	*/
	void Snapshot::snapshotHeader(){
		os << "PID " << setw(10) << "Filename " << setw(10) << "MemStart " << setw(10) << "R/W " << setw(10) << "Length " <<
			setw(10) << "Total Cpu Time " << setw(10) << "Ave Burst Time " << '\n';
	}

/*
		Prints the data of each process in the chosen device deque
		Each process gets its own line
	*/
	template<typename T>
	void Snapshot::snapshotPrint(T& itB, T& itE){
		while (itB != itE){
			string ty = mPtr->processes[*itB].type;
			os << *itB << setw(10) << mPtr->processes[*itB].name << setw(10) <<
	"0x" << hex << mPtr->processes[*itB].physicalAddress << setw(10) << ty;
			if (ty == "w"){
				os << setw(10) << mPtr->processes[*itB].length;
			}
			else {
				os << setw(20);
			}
			os << mPtr->processes[*itB].totalCpuTime << setw(10) << mPtr->processes[*itB].totalCpuTime / mPtr->processes[*itB].cpuUsageCount;
			os << '\n' << '\n';

			vector<int>::iterator itPt = mPtr->processes[*itB].pageTable.begin();
			vector<int>::iterator itPtEnd = mPtr->processes[*itB].pageTable.end();
			int size = mPtr->processes[*itB].pageTable.size() - 1;
			int count = 0;
			os << "Process #" << *itB << " page table: " << '\n';
			while (itPt != itPtEnd){
	os << *itPt;
	if (count < size){
		os << ", ";
	}
	if (!(count % 15) && count > 0){
		os << '\n';
	}
	++count;
	++itPt;
			}
			os << '\n' << '\n';

			++itB;
		}
	}

void Snapshot::snapshotAux(const string& input){
		vector<deque<int>>::iterator itV, itVe;
		deque<int>::iterator itB, itE;
			if (input == "c"){
				if (!mPtr->cdDeques.empty()){
					itV = mPtr->cdDeques.begin(); itVe = mPtr->cdDeques.end();
					itB = itV->begin(); itE = itV->end();
				}
			}
			else { //input == "p"
				if (!mPtr->printerDeques.empty()){
					itV = mPtr->printerDeques.begin(); itVe = mPtr->printerDeques.end();
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

	void Snapshot::snapshotAux_SystemInformation(){
		os << "Total System Average Cpu Time" << '\n';
		os << "-----------------------------" << '\n';
		if (mPtr->systemTotalcpuUsageCount > 0){
			os << mPtr->systemTotalCpuTime / mPtr->systemTotalcpuUsageCount;
		}
		else {
			os << "0";
		}
		os << '\n' << '\n';
	}

	void Snapshot::snapshotAux_ReadyDeque(){
		deque<int>::iterator itB = mPtr->readyDeque.begin();
		deque<int>::iterator itE = mPtr->readyDeque.end();

		os << "PID " << setw(10) << "Total Cpu Time " << setw(10) << "Avg. Burst "
			 << setw(15) << " Page Table" << '\n';
		os << "----r" << '\n';
		while (itB != itE){
			os << *itB << setw(10) << mPtr->processes[*itB].totalCpuTime << setw(10);
			if (mPtr->processes[*itB].cpuUsageCount > 0){
				cout << (mPtr->processes[*itB].totalCpuTime / mPtr->processes[*itB].cpuUsageCount);
			}
			else {
				os << "0";
			}
			os << setw(15) << " -- ";

			vector<int>::iterator itPg = mPtr->processes[*itB].pageTable.begin();
			vector<int>::iterator itPgEnd = mPtr->processes[*itB].pageTable.end();
			int size = mPtr->processes[*itB].pageTable.size()-1;
			int count = 0;

			//os << "Process #" << *itB << " page table: ";
			while (itPg != itPgEnd){
				os << *itPg;
				if (count < size){
					os << ", ";
				}
				if (!(count % 15) && count != 0){
					os << '\n';
				}
				++itPg;
				++count;
			}

			os << '\n' << '\n';
			++itB;
		}
		os << '\n';
	}

void Snapshot::snapshotAux_JobPool(){
	multiset<int>::iterator it = mPtr->jobPool.begin();
	multiset<int>::iterator itE = mPtr->jobPool.end();

	os << "PID  " << setw(10) << "  Process Size" << '\n';
	os << "----j" << '\n';
	while (it != itE){
		os << *it << setw(10) << mPtr->processes[*it].size << '\n';
		++it;
	}
	os << '\n';
}

	void Snapshot::snapshotAux_Disk(){
		multiset<int, Memory::SortByHighTrack>::iterator itHigh;
		multiset<int, Memory::SortByHighTrack>::iterator itHighEnd;

		multiset<int, Memory::SortByLowTrack>::iterator itLow;
  		multiset<int, Memory::SortByLowTrack>::iterator itLowEnd;

		for (int i = 0; i < mPtr->scanDiskDequesStatus.size(); ++i){
			os << "----" << "Scan Deque " << i+1 << '\n';
			if (mPtr->scanDiskDequesStatus[i] == 1){
				itHigh = mPtr->diskSets1[i].begin();
				itHighEnd = mPtr->diskSets1[i].end();
				snapshotAux_Disk2(itHigh, itHighEnd);

				os << '\n';
				os << "----" << "Waiting Deque " << i+1 << '\n';

  				itLow = mPtr->diskSets0[i].begin();
  				itLowEnd = mPtr->diskSets0[i].end();

				snapshotAux_Disk2(itLow, itLowEnd);
			}
			else {
				itLow = mPtr->diskSets0[i].begin();
  				itLowEnd = mPtr->diskSets0[i].end();
				snapshotAux_Disk2(itLow, itLowEnd);

				os << '\n';
				os << "----" << "Waiting Deque " << i+1 << '\n';
				
				itHigh = mPtr->diskSets1[i].begin();
				itHighEnd = mPtr->diskSets1[i].end();
				snapshotAux_Disk2(itHigh, itHighEnd);
			}
			os << '\n' << '\n';
		}
	}


	template <typename T>
	void Snapshot::snapshotAux_Disk2(T& scanIt, const T& scanItEnd){
		while (scanIt != scanItEnd){
			os << *scanIt << setw(10) << mPtr->processes[*scanIt].name << setw(10) << "0x"
	 << hex << mPtr->processes[*scanIt].physicalAddress
	 << setw(10) << mPtr->processes[*scanIt].type << setw(10);
			if (mPtr->processes[*scanIt].type == "w"){
				os << mPtr->processes[*scanIt].length << setw(10);
			}
			else {
				os << setw(20);
			}
			os << mPtr->processes[*scanIt].totalCpuTime << setw(10) <<
			(mPtr->processes[*scanIt].totalCpuTime / mPtr->processes[*scanIt].cpuUsageCount) << '\n';

			vector<int>::iterator itPg = mPtr->processes[*scanIt].pageTable.begin();
			vector<int>::iterator itPgEnd = mPtr->processes[*scanIt].pageTable.end();
			int size = mPtr->processes[*scanIt].pageTable.size()-1;
			int count = 0;

			os << "Process #" << *scanIt << " page table: ";
			while (itPg != itPgEnd){
	os << *itPg;
	if (count < size){
		os << ", ";
	}
				if (!(count % 15) && count != 0){
					os << '\n';
				}
				++itPg;
				++count;
			}
			os << '\n' << '\n';

			++scanIt;
		}
	}

void Snapshot::snapshotAux_memoryInformation(){
	int size = mPtr->freeFrameList.size();

	os << "Free Frame List-----------" << '\n';
	for (int i = 0; i < size; ++i){
		os << mPtr->freeFrameList[i]+1;
		if (i < size-1){
			os << ", ";
		}
		if (!(i % 15) && i != 0){
			os << '\n';
		}
	}

	os << '\n' << '\n';


	vector<vector<int>>::iterator it = mPtr->frameTable.begin();
	vector<vector<int>>::iterator itEnd = mPtr->frameTable.end();

	os << "Frame Table---------------" << '\n';
	os << "Total number of frames: " << mPtr->frameTable.size() << '\n';
	while (it != itEnd){
		if (it->size() == 2 && it->at(0) > -1){
			os << "p" << it->at(0) << ", " << "pg " << it->at(1);
		}
		else {
			os << "F";
		}
		os << '\n';
		++it;
	}
	os << '\n' << '\n';
}
