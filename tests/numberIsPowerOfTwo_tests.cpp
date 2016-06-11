#include "../Sysgen.cpp"
#include "../Memory.h"
#include <cassert>

using namespace std;

int main(){
  Memory m;
  Sysgen s;

  assert(s.numberIsPowerOfTwo(7) == false);
  assert(s.numberIsPowerOfTwo(8) == true);

  return 0;
}