#include <iterator>
#include <algorithm>
#include <deque>
#include "JobHandling.h"

using namespace std;

JobHandling::JobHandling(Memory* ptr) : mPtr(ptr) {}

//Returns the pid of the largest process in the job pool that will fit
//in the remaining free memory available
int JobHandling::searchForAndEraseJobThatFitsInMemory(){
  multiset<int, Memory::SortBySize>::iterator it = mPtr->jobPool.begin();
  multiset<int, Memory::SortBySize>::iterator itEnd = mPtr->jobPool.end();
  while (it != itEnd){
    if (mPtr->processes[*it].size <= mPtr->freeMemory){
      int chosenPID = *it;
      mPtr->jobPool.erase(it);
      return chosenPID;
    }
    ++it;
  }
  return -1;
}

bool JobHandling::checkForJobThatFitsInMemory(){
  int chosenPID = searchForAndEraseJobThatFitsInMemory();
  if (chosenPID > -1){
    mPtr->freeMemory -= mPtr->processes[chosenPID].size;
    addJobToMemory(chosenPID);
    mPtr->processes[chosenPID].locationCode = "r";
    mPtr->readySet.insert(chosenPID);
    return true;
  }
  return false;
}

void JobHandling::addJobToMemory(const int& pid){
  int processSize = mPtr->processes[pid].size;
  int pagesNeeded = processSize / mPtr->pageSize;
  if (processSize % mPtr->pageSize){
    ++pagesNeeded;
  }

  mPtr->processes[pid].pageTable.resize(pagesNeeded);
  for (int i = 0; i < pagesNeeded; ++i){
    //frameTable[freeFrameTable[i]] represents the frame
    //that freeFrameTable[i] stores


    //i represents the process' page currently being stored
    //in relation to the chosen frame
    mPtr->frameTable[mPtr->freeFrameList[i]] = {pid,i};
    mPtr->processes[pid].pageTable[i] = mPtr->freeFrameList[i];
  }
  for (int j = 0; j < pagesNeeded; ++j){
    mPtr->freeFrameList.pop_front();
  }
}

void JobHandling::addAsManyJobsAsPossibleToMemory(){
  bool jobAdded = true;
  while (jobAdded){
    jobAdded = checkForJobThatFitsInMemory();
  }
}
