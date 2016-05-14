#include "Cpu.h"
using namespace std;

struct JobHandling : public Cpu {
	int searchForAndEraseJobThatFitsInMemory();

 	bool checkForJobThatFitsInMemory();

	void addJobToMemory(const int& pid);

	void addAsManyJobsAsPossibleToMemory();
};
