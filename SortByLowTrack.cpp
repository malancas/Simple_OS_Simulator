#include "Memory.h"

using namespace std;

struct SortByLowTrack{
	Memory* mPtr;

	SortByLowTrack(Memory m) : mPtr(&m) {}

	bool operator() (const int& x, const int& y) { return mPtr->processes[x].track < mPtr->processes[y].track; }
};