#include <iomanip>
#include <set>
#include "Snapshot.h"
#include "Memory.h"

using namespace std;

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
			string ty = processes[*itB].type;
			os << *itB << setw(10) << processes[*itB].name << setw(10) <<
	"0x" << hex << processes[*itB].physicalAddress << setw(10) << ty;
			if (ty == "w"){
				os << setw(10) << processes[*itB].length;
			}
			else {
				os << setw(20);
			}
			os << processes[*itB].totalCpuTime << setw(10) << processes[*itB].totalCpuTime / processes[*itB].cpuUsageCount;
			os << '\n' << '\n';

			vector<int>::iterator itPt = processes[*itB].pageTable.begin();
			vector<int>::iterator itPtEnd = processes[*itB].pageTable.end();
			int size = processes[*itB].pageTable.size() - 1;
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
				if (!cdDeques.empty()){
					itV = cdDeques.begin(); itVe = cdDeques.end();
					itB = itV->begin(); itE = itV->end();
				}
			}
			else { //input == "p"
				if (!printerDeques.empty()){
					itV = printerDeques.begin(); itVe = printerDeques.end();
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
		if (systemTotalcpuUsageCount > 0){
			os << systemTotalCpuTime / systemTotalcpuUsageCount;
		}
		else {
			os << "0";
		}
		os << '\n' << '\n';
	}

	void Snapshot::snapshotAux_ReadyDeque(){
		deque<int>::iterator itB = readyDeque.begin();
		deque<int>::iterator itE = readyDeque.end();

		os << "PID " << setw(10) << "Total Cpu Time " << setw(10) << "Avg. Burst "
			 << setw(15) << " Page Table" << '\n';
		os << "----r" << '\n';
		while (itB != itE){
			os << *itB << setw(10) << processes[*itB].totalCpuTime << setw(10);
			if (processes[*itB].cpuUsageCount > 0){
				cout << (processes[*itB].totalCpuTime / processes[*itB].cpuUsageCount);
			}
			else {
				os << "0";
			}
			os << setw(15) << " -- ";

			vector<int>::iterator itPg = processes[*itB].pageTable.begin();
			vector<int>::iterator itPgEnd = processes[*itB].pageTable.end();
			int size = processes[*itB].pageTable.size()-1;
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
	multiset<int>::iterator it = jobPool.begin();
	multiset<int>::iterator itE = jobPool.end();

	os << "PID  " << setw(10) << "  Process Size" << '\n';
	os << "----j" << '\n';
	while (it != itE){
		os << *it << setw(10) << processes[*it].size << '\n';
		++it;
	}
	os << '\n';
}

	void Snapshot::snapshotAux_Disk(){
		for (int i = 0; i < scanDiskDequesStatus.size(); ++i){
			os << "----" << "Scan Deque " << i+1 << '\n';
			if (scanDiskDequesStatus[i] == 1){
				multiset<int,sortByHighCmp>::iterator it = diskSets1[i].begin();
				multiset<int,sortByHighCmp>::iterator itEnd = diskSets1[i].end());
				snapshotAux_Disk2(it, itEnd);

				os << '\n';
				os << "----" << "Waiting Deque " << i+1 << '\n';
				snapshotAux_Disk2(diskSets0[i].begin(), diskSets0[i].end());
			}
			else {
				snapshotAux_Disk2(diskSets0[i].begin(), diskSets0[i].end());
				os << '\n';
				os << "----" << "Waiting Deque " << i+1 << '\n';
				snapshotAux_Disk2(diskSets1[i].begin(), diskSets1[i].end());
			}
			os << '\n' << '\n';
		}
	}


	template <typename T>
	void Snapshot::snapshotAux_Disk2(T& scanIt, const T& scanItEnd){
		while (scanIt != scanItEnd){
			os << *scanIt << setw(10) << processes[*scanIt].name << setw(10) << "0x"
	 << hex << processes[*scanIt].physicalAddress
	 << setw(10) << processes[*scanIt].type << setw(10);
			if (processes[*scanIt].type == "w"){
				os << processes[*scanIt].length << setw(10);
			}
			else {
				os << setw(20);
			}
			os << processes[*scanIt].totalCpuTime << setw(10) <<
			(processes[*scanIt].totalCpuTime / processes[*scanIt].cpuUsageCount) << '\n';

			vector<int>::iterator itPg = processes[*scanIt].pageTable.begin();
			vector<int>::iterator itPgEnd = processes[*scanIt].pageTable.end();
			int size = processes[*scanIt].pageTable.size()-1;
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
	int size = freeFrameList.size();

	os << "Free Frame List-----------" << '\n';
	for (int i = 0; i < size; ++i){
		os << freeFrameList[i]+1;
		if (i < size-1){
			os << ", ";
		}
		if (!(i % 15) && i != 0){
			os << '\n';
		}
	}

	os << '\n' << '\n';


	vector<vector<int>>::iterator it = frameTable.begin();
	vector<vector<int>>::iterator itEnd = frameTable.end();

	os << "Frame Table---------------" << '\n';
	os << "Total number of frames: " << frameTable.size() << '\n';
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
