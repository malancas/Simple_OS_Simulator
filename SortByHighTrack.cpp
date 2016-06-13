#include "Memory.h"

struct SortByHighTrack{
	Memory* mPtr;

	SortByHighTrack(Memory m) : mPtr(&m) {}

	bool operator() (const int& x, const int& y) { return mPtr->processes[x].track > mPtr->processes[y].track; }
};