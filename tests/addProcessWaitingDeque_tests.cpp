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
	Checks whether the addProcessToWaitingQueue function
	does insert pids in sorted order according to their
	track number. The vectors diskSets0 and diskSets1 
	will be sorted from lowest track number to highest 
	and highest track to lowest, respectively
*/

int main (){
	Memory m;
	CPU c;

	//lowest to highest track number sorting tests
	m.diskSets0.resize(2);

	m.processes = {{0,Process(0,5)},{1,Process(1,5)},{2,Process(2,5)},{3,Process(3,5)},{4,Process(4,5)}};
	m.processes[0].track = 0;
	m.processes[1].track = 99;
	m.processes[2].track = 98;
	m.processes[3].track = 4;
	m.processes[4].track = 4;

	m.addProcessToWaitingQueue(0,0,true);
	m.addProcessToWaitingQueue(1,0,true);
	m.addProcessToWaitingQueue(2,0,true);
	m.addProcessToWaitingQueue(3,0,true);
	m.addProcessToWaitingQueue(4,0,true);

	multiset<Process>::iterator it = m.diskSets0[0].begin();
	multiset<Process>::iterator itEnd = m.diskSets0[0].end();
	while (it != itEnd){
		assert(it->pid == 0);
		assert(it->track == 0); ++it;
		
		assert(it->pid == 3); 
		assert(it->track == 4); ++it;
		
		assert(it->pid == 4);
		assert(it->track == 4); ++it;

		assert(it->pid == 2);
		assert(it->track == 98); ++it;
		
		assert(it->pid == 1);
		assert(it->track == 99); ++it;
	}

	//highest to lowest track number sorting tests
	m.diskSets1.resize(2);

	m.addProcessToWaitingQueue(0,0,false);
	m.addProcessToWaitingQueue(1,0,false);
	m.addProcessToWaitingQueue(2,0,false);
	m.addProcessToWaitingQueue(3,0,false);
	m.addProcessToWaitingQueue(4,0,false);

	it = m.diskSets1[0].begin();
	itEnd = m.diskSets1[0].end();
	while (it != itEnd){
		assert(it->pid == 1);
		assert(it->track == 99); ++it;
		
		assert(it->pid == 2); 
		assert(it->track == 98); ++it;
		
		assert(it->pid == 3);
		assert(it->track == 4); ++it;

		assert(it->pid == 4);
		assert(it->track == 4); ++it;
		
		assert(it->pid == 0);
		assert(it->track == 0); ++it;
	}

	cout << "TESTS PASSED!" << '\n';		

	return 0;
}