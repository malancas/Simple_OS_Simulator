#include <iostream>
#include <string>
#include <cassert>
#include "Sysgen.cpp"
#include "Cpu.h"
using namespace std;

int main(){
  Memory m;
  Memory* mPtr = &m;
  
  /*Sys gen
    This section of the program handles
    user input and the appropiate error
    checking. It will also use the input to
    initialize data structures like device queues
    to set up for the Running phase of the program
  */
  Sysgen s(mPtr);
  s.getInstallerInput();

  /*Running
    This part will take user input, parse it and error
    check it before using it to control the arrival and
    destruction of processes, the CPU, and device system
    calls. 
  */
  Cpu c(mPtr);
  c.waitForInput();

  return 0;
}
