#include <iostream>
#include <string>
#include <cassert>
#include "Sysgen.cpp"
#include "CPU.h"
using namespace std;

int main(){
  /*Sys gen
    This section of the program handles
    user input and the appropiate error
    checking. It will also use the input to
    initialize data structures like device queues
    to set up for the Running phase of the program
  */
  Sysgen s;
  s.getInstallerInput();

  /*Running
    This part will take user input, parse it and error
    check it before using it to control the arrival and
    destruction of processes, the CPU, and device system
    calls. 
  */

  CPU c;
  c.waitForInput();

  return 0;
}