#include "../CPU.h"
#include "../Memory.h"
#include <cassert>

using namespace std;

int main(){
  Memory m;
  CPU c;

  m.processes = {{0,Process(0,5,54)},{1,Process(1,5,5)},{2,Process(2,5,5)},{3,Process(3,5,4)},{4,Process(4,5,5)}};

  m.processes[0].size = 0;
  m.processes[1].size = 2;
  c.processes[4].size = 1;
  m.processes[3].size = 4;
  c.processes[2].size = 3;

  assert(m.processes[0].size == 0);
  assert(m.processes[1].size == 2);
  assert(m.processes[4].size == 1);
  assert(m.processes[3].size == 4);
  assert(m.processes[2].size == 3);

  c.jobPool.push_back(1);
  m.jobPool.push_back(0);
  m.jobPool.push_back(4);
  m.jobPool.push_back(3);
  c.jobPool.push_back(2);
  assert(m.jobPool.size() == 5);
  
  c.snapshotAux_JobPool();

  cout << c.os.str() << '\n';

  assert(m.jobPool[0] == 3);
  assert(m.jobPool[1] == 2);
  assert(m.jobPool[2] == 1);
  assert(m.jobPool[3] == 4);
  assert(m.jobPool[4] == 0);

  cout << "Snapshot j tests passed!" << '\n';
  
  return 0;
}
