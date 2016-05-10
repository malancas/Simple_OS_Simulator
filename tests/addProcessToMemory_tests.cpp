#include "../Cpu.h"
#include "../Memory.h"
#include <cassert>

using namespace std;

int main(){
  Memory m;
  Cpu c;

  m.pageSize = 20;
  m.totalMemorySize = 1000;
  m.freeMemory = 200;
  m.frameTable.resize(50);
  for (auto i :m.frameTable){
    i = {-1,-1};
  }
  c.freeFrameList.resize(30);
  c.freeFrameList.push_back(1);
  for (int i = 5; i < 10; ++i){
    c.freeFrameList.push_back(i);
  }

  for (int j = 15; j < 39; ++j){
    c.freeFrameList.push_back(j);
  }

  for (int z = 0; z < 30; ++z){
    cout << c.freeFrameList[z] << " ";
  }
  cout << '\n';
  
  
  m.processes = {{0,Process(0,5,54)},{1,Process(1,5,5)},{2,Process(2,5,5)},{3,Process(3,5,4)},{4,Process(4,5,5)}};
  m.processes[0].size = 20;
  m.processes[1].size = 46;
  c.processes[4].size = 100;
  m.processes[3].size = 50;
  c.processes[2].size = 50;

  for (int i = 0; i < 5; ++i){
    c.jobPool.push_back(i);
  }

  cout << c.freeFrameList[0] << '\n';
  c.addAsManyJobsAsPossibleToMemory();
  assert(c.readyDeque.size() == 3);
  assert(m.jobPool.size() == 2);

  assert(m.jobPool[0] == 1);
  assert(c.jobPool[1] == 0);
  assert(m.jobPool[1] == 0);

  assert(m.readyDeque[0] == 4);
  assert(m.readyDeque[1] == 2);
  assert(m.readyDeque[2] == 3);

  assert(m.processes[2].pageTable.size() == 3);
  c.restoreFrameTableAndFreeFrameList(2);
  m.freeMemory += m.processes[2].size;
  assert(m.freeFrameList.size() == 33);
  
  cout << "ADD PROCESS TO MEMORY TESTS PASSED" << '\n';
  return 0;
}
