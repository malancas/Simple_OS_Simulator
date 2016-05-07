#include "../CPU.h"
#include "../Memory.h"
#include <cassert>
#include <iostream>
#include <string>
using namespace std;

int main()
{
	Memory m;
	CPU c;

	m.totalMemorySize = 128;
	m.pageSize = 2;

	m.processes = {{0,Process(0,5,54)},{1,Process(1,5,5)},{2,Process(2,5,5)},{3,Process(3,5,4)},{4,Process(4,5,5)}};
	m.processes[0].locationCode = "d13";
	m.processes[0].size = 2;
	m.processes[0].pageTable.push_back(0);
	
	m.processes[1].locationCode = "p0";
	m.processes[1].size = 4;
	m.processes[1].pageTable.push_back(1);
	m.processes[1].pageTable.push_back(2);
		
	m.processes[2].locationCode = "p0";
	m.processes[2].size = 5;
	for (int i = 0; i < 3; ++i)
	  {m.processes[2].pageTable.push_back(3+i);}
	//frames 3,4,5
	
	m.processes[3].locationCode = "c1";
	m.processes[3].size = 10;
	for (int i = 0; i < 5; ++i)
	  {m.processes[3].pageTable.push_back(6+i);}
	//frames 6,7,8,9,10
	
	m.processes[4].locationCode = "j";
	m.processes[4].size = 4;
	for (int i = 0; i < 2; ++i)
	  {m.processes[4].pageTable.push_back(11+i);}
	//frames 11,12

	c.freeMemory = 103;
	for (int i = 0; i < 51; ++i){
	  m.freeFrameList.push_back(15+i);
	}
	assert(m.freeFrameList.size() == 51);
	
	for (int i = 0; i < 64; ++i)
	  {m.frameTable.push_back(vector<int>(2,-1));}
	
	m.printerDeques.resize(1);
	m.cdDeques.resize(2);
	m.diskDeques1.resize(4);

	for (int i = 0; i < 10; ++i){
		m.printerDeques[0].push_back(i);
		m.cdDeques[1].push_back(i);
		m.diskDeques1[3].push_back(i);
		m.jobPool.push_back(i);
	}

	//process size = 2
	c.killProcess(0);
	assert(m.freeFrameList.size() == 52);
	assert(m.freeMemory == 105);
	assert(m.processes.find(0) == m.processes.end());
	for (auto j: m.diskDeques1[3]){
		assert(j != 0);
	}

	//process size = 4
	c.killProcess(1);
	assert(m.freeFrameList.size() == 54);
	assert(m.freeMemory == 109);
	assert(m.processes.find(1) == m.processes.end());
	for (auto j: m.printerDeques[0]){
		assert(j != 1);
	}

       	//process size = 5
	c.killProcess(2);
	assert(m.freeFrameList.size() == 57);
	assert(m.freeMemory == 114);
	assert(m.processes.find(2) == m.processes.end());
	for (auto j: m.printerDeques[3]){
		assert(j != 2);
	}

	//process size = 10
	c.killProcess(3);
	assert(m.freeFrameList.size() == 62);
	assert(m.freeMemory == 124);
	assert(m.processes.find(3) == m.processes.end());
	for (auto j: m.cdDeques[1]){
		assert(j != 3);
	}

	//process size = 4
	c.killProcess(4);
	assert(m.freeFrameList.size() == 64);
	assert(m.freeMemory == 128);
	assert(m.processes.find(4) == m.processes.end());
	for (auto j: m.jobPool){
		assert(j != 4);
	}

	cout << "Kill process tests passed!" << '\n';

	return 0;
}
