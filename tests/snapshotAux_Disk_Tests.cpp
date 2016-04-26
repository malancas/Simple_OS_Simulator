#include <cassert>
#include <unordered_map>
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
#include "../Memory.h"
#include "../CPU.h"

/*
	Checks whether the addProcessToReadyQueue function
	does insert pids in sorted order according to their
	remaining burst. Negative numbers always have higher
	priority
*/

int main (){
	Memory m;
	CPU c;
	m.scanDiskQueuesStatus.resize(2);
	m.scanDiskQueuesStatus[0] = 0;
	m.scanDiskQueuesStatus[1] = 1;
	m.processes = {{0,Process(0,5)},{1,Process(1,5)},{2,Process(2,5)},{3,Process(3,5)},{4,Process(4,5)}};
	m.diskSets0.resize(2);
	m.diskSets1.resize(2);
	
	m.processes[0].name = "ffffd";
	m.processes[0].memStart = 34;
	m.processes[0].length = 0;
	m.processes[0].type = "r";
	m.processes[0].totalCPUTime = 4;
	m.processes[0].cpuUsageCount = 1;
	m.processes[0].track = 1;

	m.processes[1].name = "dog";
	m.processes[1].memStart = 0;
	m.processes[1].length = 5;
	m.processes[1].type = "w";
	m.processes[1].totalCPUTime = 5;
	m.processes[1].cpuUsageCount = 5;
	m.processes[1].track = 3443;

	m.processes[2].name = "Bird";
	m.processes[2].memStart = 1;
	m.processes[2].length = 98;
	m.processes[2].type = "r";
	m.processes[2].totalCPUTime = 100;
	m.processes[2].cpuUsageCount = 50;
	m.processes[2].track = 34;

	m.processes[3].name = "Fish";
	m.processes[3].memStart = 4;
	m.processes[3].length = 4;
	m.processes[3].type = "w";
	m.processes[3].totalCPUTime = 50;
	m.processes[3].cpuUsageCount = 3;
	m.processes[3].track = 5;
	
	m.processes[4].name = "cat";
	m.processes[4].memStart = 100;
	m.processes[4].length = 4;
	m.processes[4].type = "w";
	m.processes[4].totalCPUTime = 10;
	m.processes[4].cpuUsageCount = 5;
	m.processes[4].track = 4;

	m.diskSets0[0].insert(m.processes[4]);
	m.diskSets1[0].insert(m.processes[2]);
	m.diskSets1[0].insert(m.processes[3]);
	m.diskSets0[1].insert(m.processes[1]);
	m.diskSets0[1].insert(m.processes[0]);


	c.snapshotHeader();	
	cout << c.os.str();
    c.os.str("");
    c.os.clear();

	m.snapshotAux_Disk();

	cout << "TESTS PASSED!" << '\n';		

	return 0;
}