#include "../Cpu.h"
#include "../Memory.h"
#include <cassert>

using namespace std;

int main(){
  Memory m;
  Cpu c;

  m.pageSize = 20;
  m.totalMemorySize = 1000;

  m.processes = {{0,Process(0,5,54)},{1,Process(1,5,5)},{2,Process(2,5,5)},{3,Process(3,5,4)},{4,Process(4,5,5)}};
  m.processes[0].size = 20;
  m.processes[1].size = 46;
  c.processes[4].size = 100;
  m.processes[3].size = 50;
  c.processes[2].size = 50;

  m.frameTable.resize(50);

  m.frameTable[1] = {0,0};
  m.frameTable[2] = {1,0};
  m.frameTable[3] = {1,1};
  m.frameTable[4] = {1,2};
  m.frameTable[11] = {4,0};
  m.frameTable[12] = {4,1};
  m.frameTable[13] = {4,2};
  m.frameTable[14] = {4,3};
  m.frameTable[15] = {4,4};
  m.frameTable[17] = {3,0};
  m.frameTable[18] = {3,1};
  m.frameTable[19] = {3,2};
  m.frameTable[25] = {2,0};
  m.frameTable[26] = {2,1};
  m.frameTable[27] = {2,2};

  cout << "m size: " << m.frameTable[1].size() << '\n';
  cout << "c size: " << c.frameTable[1].size() << '\n'; 

  m.freeFrameList.push_back(0);
  m.freeFrameList.push_back(5);
  m.freeFrameList.push_back(6);
  m.freeFrameList.push_back(7);
  m.freeFrameList.push_back(8);
  m.freeFrameList.push_back(9);
  m.freeFrameList.push_back(10);
  m.freeFrameList.push_back(16);
  m.freeFrameList.push_back(20);
  m.freeFrameList.push_back(21);
  m.freeFrameList.push_back(22);
  m.freeFrameList.push_back(23);
  m.freeFrameList.push_back(24);
  m.freeFrameList.push_back(28);
  m.freeFrameList.push_back(29);
  m.freeFrameList.push_back(30);
  m.freeFrameList.push_back(31);
  m.freeFrameList.push_back(32);
  m.freeFrameList.push_back(33);
  m.freeFrameList.push_back(34);
  m.freeFrameList.push_back(35);
  m.freeFrameList.push_back(36);
  m.freeFrameList.push_back(37);
  m.freeFrameList.push_back(38);
  m.freeFrameList.push_back(39);
  m.freeFrameList.push_back(40);
  m.freeFrameList.push_back(41);
  m.freeFrameList.push_back(42);
  m.freeFrameList.push_back(43);
  m.freeFrameList.push_back(44);
  m.freeFrameList.push_back(45);
  m.freeFrameList.push_back(46);
  m.freeFrameList.push_back(47);
  m.freeFrameList.push_back(48);
  m.freeFrameList.push_back(49);

  c.snapshotAux_memoryInformation();
  cout << c.os.str();

  return 0;
}