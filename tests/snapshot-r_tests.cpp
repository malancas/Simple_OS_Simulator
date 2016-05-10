#include <cassert>
#include "../Cpu.h"
#include "../Memory.h"

int main(){
  Memory m;
  Cpu c;

  m.processes = {{0,Process(0,5,54)},{1,Process(1,5,5)},{2,Process(2,5,5)},{3,Process(3,5,4)},{4,Process(4,5,5)}};

  m.processes[0].size = 20;
  m.processes[1].size = 46;
  c.processes[4].size = 100;
  m.processes[3].size = 50;
  c.processes[2].size = 50;

  for (int i = 0; i < 5; ++i){
    c.readyDeque.push_back(i);
  }

  for (int i = 0; i < 5; ++i){
    m.processes[i].pageTable.resize(2);
    m.processes[i].pageTable = {i*2, i*2+1};
  }

  assert(m.readyDeque.size() == 5);

  c.snapshotAux_ReadyDeque();
  cout << c.os.str();

  return 0;
}
