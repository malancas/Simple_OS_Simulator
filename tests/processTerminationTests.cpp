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
using namespace std;

/*
	Checks that the process termination
	function works properly by printing 
	relevant process and system information 
	in a readable and neat format
*/

int main(){
	Memory m;
	CPU c;

	//lowest to highest track number sorting tests
	m.diskSets0.resize(2);

	m.processes = {{0,Process(0,5)},{1,Process(1,5)},{2,Process(2,5)},{3,Process(3,5)},{4,Process(4,5)}};
	m.processes[0].totalCPUTime = 0;
	m.processes[0].cpuUsageCount = 0;
	
	m.processes[1].track = 99;
	m.processes[2].track = 98;
	m.processes[3].track = 4;
	m.processes[4].track = 4;	

	return 0;
}