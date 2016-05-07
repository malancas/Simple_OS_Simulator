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

int main (){
	Memory m;
	CPU c;

	m.processes = {{0,Process(0,5)},{1,Process(1,5)},{2,Process(2,5)},{3,Process(3,5)},{4,Process(4,5)}};

	m.historyParameter = 0.5;
	m.currProcess = 1;
	m.processes[1].burstEstimate = 5;
	m.floatResult = 4;

	cout << m.sjwAlgorithm() << '\n';

	return 0;
}