#include <iostream>
#include <string>
#include <cassert>
#include "Sysgen.cpp"
#include "CPU.cpp"
using namespace std;

//class NegativeError {};

int main(){
  //sys gen
  Sysgen s;

  s.getUserDeviceInput();

  //running
  CPU c;

  c.waitForInput();

  return 0;
}
