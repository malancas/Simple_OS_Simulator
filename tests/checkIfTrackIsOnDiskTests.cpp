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
#include "../CPU.cpp"
#include "../Memory.h"
using namespace std;

int main(){
	Memory m;
	m.cylinderCount = {2,1,0,214};
	CPU c;

	//Valid track numbers for disk 1 are 0,1
	assert(c.checkIfTrackIsInDisk(2,1)==false);
	cout << '\n' << "cylinder 1: " << m.cylinderCount[0] << '\n';
	assert(c.checkIfTrackIsInDisk(1,1)==true);
	assert(c.checkIfTrackIsInDisk(0,1)==true);

	//Only valid track is 0
	assert(c.checkIfTrackIsInDisk(0,2)==true);
	assert(c.checkIfTrackIsInDisk(1,2)==false);
	assert(c.checkIfTrackIsInDisk(2,2)==false);

	//No valid tracks
	assert(c.checkIfTrackIsInDisk(0,3)==false);

	//Tracks 0-213 are valid
	assert(c.checkIfTrackIsInDisk(214,4)==false);
	assert(c.checkIfTrackIsInDisk(213,4)==true);
	assert(c.checkIfTrackIsInDisk(0,4)==true);
	assert(c.checkIfTrackIsInDisk(1,4)==true);
	assert(c.checkIfTrackIsInDisk(2,4)==true);

	return 0;
}