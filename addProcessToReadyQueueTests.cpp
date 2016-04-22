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
#include "CPU.h"
#include "Memory.h"

int main (){
	//Memory m;
	CPU c;
	c.processes = {{0,Process(0,5)},{1,Process(1,5)},{2,Process(2,5)},{3,Process(3,5)},{4,Process(4,5)}};
	c.processes[0].remainingBurst = 0;
	c.processes[1].remainingBurst = -4;
	c.processes[2].remainingBurst = 98;
	c.processes[3].remainingBurst = 4;
	c.processes[4].remainingBurst = 4;

	for (int i = 0; i < c.processes.size(); ++i){
		c.addProcessToReadyQueue(i);
	}

	assert(c.readyQueue[0] == 1);
	assert(c.readyQueue[1] == 0);
	assert(c.readyQueue[2] == 3);
	assert(c.readyQueue[3] == 4);
	assert(c.readyQueue[4] == 2);

	return 0;
}