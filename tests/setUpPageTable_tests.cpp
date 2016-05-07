#include "../Sysgen.cpp"
#include "../Memory.h"
#include <cassert>

using namespace std;

int main(){
  Memory m;
  Sysgen s;

  m.totalMemorySize = 16;
  m.maximumProcessSize = 8;
  m.pageSize = 2;
  m.pageTable.resize(4);
  
  s.setUpPageTable();

  for (auto i: m.pageTable){
    cout << i << '\n';
    assert(i < 16);
  }
  cout << '\n';
  
  for (auto i: s.pageTable){
    cout << i << '\n';
    assert(i < 16);
  }
  cout << '\n';

  cout << "setUpPageTable tests passed" << '\n';
  return 0;
}
