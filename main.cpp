#include <iostream>
#include <string>
#include <long_opts>
#include <unorderd_map>
#include "DeviceQueues"
using namespace std;

void parseInput(const string& in, DeviceQueues& d){
   int pNum = in[0];
   int dNUm = in[4];
   int cNum = in[8];

   d.makeQueues(pNum, dNum, cNum);
}

int main(){
  //sysgen
  string input = "";
  
  //Include try/catch for input, error catching
  //Ask for number of printer, disk, CD devices, respectively
  cout << "Enter devices in format '<num>P, <num>D, <num>C' " << '\n';
  cin >> input;

  DeviceQueues d;

  parseInput(input, d);

  //running
  
  /*
    Use hash for processes
    Must be accesible by ready queue, device queues, CPU
    Sits in main or should it exist as static in base class
    and can be inherited/accessed by all objects
  */


  return 0;
}
