#include "Memory.h"

struct SortBySize{
	Memory* mPtr;

	SortBySize(Memory m) : mPtr(&m) {}

  	bool operator() (const int& x, const int& y) { return mPtr->processes[x].size > mPtr->processes[y].size; }
};