#include "../Cpu.h"
#include "../Memory.h"
#include <cassert>

using namespace std;

int main(){
  Memory m;
  Cpu c;

  m.processes = {{0,Process(0,5,54)},{1,Process(1,5,5)},{2,Process(2,5,5)},{3,Process(3,5,4)},{4,Process(4,5,5)}};


  return 0;
}