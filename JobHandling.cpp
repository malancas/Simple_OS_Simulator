#include <iterator>
#include <algorithm>
#include <deque>
#include "JobHandling.h"
#include "Memory.h"

using namespace std;

//Returns the pid of the largest process in the job pool that will fit
//in the remaining free memory available
int JobHandling::searchForAndEraseJobThatFitsInMemory(){
  sort(jobPool.begin(), jobPool.end(), sortByLargestSizeFirst);
  deque<int>::iterator it = jobPool.begin();
  deque<int>::iterator itEnd = jobPool.end();
  while (it != itEnd){
    if (processes[*it].size <= freeMemory){
      int chosenPID = *it;
      jobPool.erase(it);
      return chosenPID;
    }
    ++it;
  }
  return -1;
}

bool JobHandling::checkForJobThatFitsInMemory(){
  int chosenPID = searchForAndEraseJobThatFitsInMemory();
  if (chosenPID > -1){
    freeMemory -= processes[chosenPID].size;
    addJobToMemory(chosenPID);
    processes[chosenPID].locationCode = "r";
    addProcessToReadyDeque(chosenPID);
    return true;
  }
  return false;
}

void JobHandling::addJobToMemory(const int& pid){
  int processSize = processes[pid].size;
  int pagesNeeded = processSize / pageSize;
  if (processSize % pageSize){
    ++pagesNeeded;
  }

  processes[pid].pageTable.resize(pagesNeeded);
  for (int i = 0; i < pagesNeeded; ++i){
    //frameTable[freeFrameTable[i]] represents the frame
    //that freeFrameTable[i] stores


    //i represents the process' page currently being stored
    //in relation to the chosen frame
    frameTable[freeFrameList[i]] = {pid,i};
    processes[pid].pageTable[i] = freeFrameList[i];
  }
  for (int j = 0; j < pagesNeeded; ++j){
    freeFrameList.pop_front();
  }
}

void JobHandling::addAsManyJobsAsPossibleToMemory(){
  bool jobAdded = true;
  while (jobAdded){
    jobAdded = checkForJobThatFitsInMemory();
  }
}
