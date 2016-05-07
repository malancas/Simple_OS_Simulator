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
	m.processes = {{0,Process(0,5)},{1,Process(1,5)},{2,Process(2,5)},{3,Process(3,5)},{4,Process(4,5)}};
	m.processes[0].remainingBurst = 0;
	m.processes[1].remainingBurst = -4;
	m.processes[2].remainingBurst = 98;
	m.processes[3].remainingBurst = 4;
	m.processes[4].remainingBurst = 4;

	for (int i = 0; i < m.processes.size(); ++i){
		m.addProcessToReadyQueue(i);
	}

	assert(m.readyQueue[0] == 1);
	assert(m.readyQueue[1] == 0);
	assert(m.readyQueue[2] == 3);
	assert(m.readyQueue[3] == 4);
	assert(m.readyQueue[4] == 2);
	cout << "TESTS PASSED!" << '\n';		

	return 0;
}