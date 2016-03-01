#include <iostream>
#include <string>
#include "Memory.cpp"
#include "CPU.cpp"
#include <cassert>
using namespace std;

class NegativeError {};

int main(){
  //sysgen
  int inP, inD, inC;
  
  //Include try/catch for input, error catching
  //Ask for number of printer, disk, CD devices, respectively
  bool goodInput = false;
  while (!goodInput) {
    cout << "Enter number of printer devices: " << '\n';
    cin >> inP;
    if (inP < 0){
      cerr << "Negative number entered. Try again." << '\n' << '\n';
    }
    else {
      goodInput = true;
    }
  }

  goodInput = false;
  while (!goodInput){
    cout << "Enter number of disk devices: " << '\n';
    cin >> inD;
    if (inD < 0){
      cerr << "Negative number entered. Try again." << '\n' << '\n';
    }
    else {
      goodInput = true;
    }
  }

  goodInput = false;
  while (!goodInput){
    cout << "Enter number of CD devices: " << '\n';
    cin >> inC;
    if (inC < 0){
      cerr << "Negative number entered. Try again." << '\n' << '\n';
    }
    else {
      goodInput = true;
    }
  }

  Memory m;
  m.makeQueues(inP, inD, inC);

  
  //running
  CPU c;

  c.waitForInput();

  return 0;
}
