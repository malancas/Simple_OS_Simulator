#include <iostream>
//long_opts
#include <string>
#include "Memory.cpp"
#include "CPU.cpp"
#include <cassert>
using namespace std;

int main(){
  //sysgen
  string inP, inD, inC;
  
  //Include try/catch for input, error catching
  //Ask for number of printer, disk, CD devices, respectively
  cout << "Enter devices in format '<number of p>, <number of d>, <number of c>' " << '\n';
  cin >> inP;
  cin >> inD;
  cin >> inC;

  Memory m;
  m.makeQueues(inP, inD, inC);
  
  //running
  CPU c;

  c.waitForInput();

  /*
    Use hash for processes
    Must be accesible by ready queue, device queues, CPU
    Sits in main or should it exist as static in base class
    and can be inherited/accessed by all objects
  */
  return 0;
}
