#include "Memory.h"

using namespace std;

struct SortByLowBurst{
	Memory* mPtr;

	SortByLowBurst(Memory m) : mPtr(&m) {}

	bool operator() (const int& x, const int& y) { return mPtr->processes[x].remainingBurst < mPtr->processes[y].remainingBurst; }
};