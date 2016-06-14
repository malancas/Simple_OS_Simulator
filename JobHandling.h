#ifndef JOBHANDLING_H
#define JOBHANDLING_H

#include "Memory.h"

using namespace std;

struct JobHandling {
	Memory* mPtr;

	JobHandling(Memory* ptr);

	int searchForAndEraseJobThatFitsInMemory();

 	bool checkForJobThatFitsInMemory();

	void addJobToMemory(const int& pid);

	void addAsManyJobsAsPossibleToMemory();
};

#endif