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

	m.processes = {{0,Process(0,5,54)},{1,Process(1,5,5)},{2,Process(2,5,5)},{3,Process(3,5,4)},{4,Process(4,5,5)}};
	m.processes[0].locationCode = "d13";
	m.processes[1].locationCode = "p0";
	m.processes[2].locationCode = "p0";
	m.processes[3].locationCode = "c1";
	m.processes[4].locationCode = "j";

	m.printerQueues.resize(1);
	m.cdQueues.resize(2);
	m.diskDeques1.resize(4);

	for (int i = 0; i < 10; ++i){
		m.printerQueues[0].push_back(i);
		m.cdQueues[1].push_back(i);
		m.diskDeques1[3].push_back(i);
		m.jobPool.push_back(i);
	}

	c.killProcess(0);
	assert(m.processes.find(0) == m.processes.end());
	for (auto j: m.diskDeques1[3]){
		assert(j != 0);
	}

	c.killProcess(1);
	assert(m.processes.find(1) == m.processes.end());
	for (auto j: m.printerQueues[0]){
		assert(j != 1);
	}

	c.killProcess(2);
	assert(m.processes.find(2) == m.processes.end());
	for (auto j: m.printerQueues[3]){
		assert(j != 2);
	}

	c.killProcess(3);
	assert(m.processes.find(3) == m.processes.end());
	for (auto j: m.cdQueues[1]){
		assert(j != 3);
	}

	c.killProcess(4);
	assert(m.processes.find(4) == m.processes.end());
	for (auto j: m.jobPool){
		assert(j != 4);
	}

	cout << "Tests passed!" << '\n';

	return 0;
}