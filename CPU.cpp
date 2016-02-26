#include <iostream>
#include <string>
using namespace std;

struct CPU : public Memory {
  //VARIABLES
  int currProcess;
  bool emptyCPU;

  //CONTRUCTORS
  CPU() : currProcess(-1), emptyCPU(true) {}

  //FUNCTIONS
  //main run function
  void run(){
    waitForInput();
  }

  void waitForInput(){
    string input = "";
    while (input != "t"){
      cin >> input;
      if (emptyCPU && input != "A"){
        cout << "No processes running. Add processes and try again" << '\n';
      }
      else if (emptyCPU && input == "A"){
        processes.insert(make_pair(pidCounter,Process(pidCounter, pcbCounter));
        currProcess = pidCounter;
        ++pidCounter; ++pcbCounter;
        emptyCPU = false;
      }
      else if (!emptyCPU && input == "A"){
        processes.insert(make_pair(pidCounter,Process(pidCounter, pcbCounter));
        readyQueue.push(pidCounter);
        ++pidCounter; ++pcbCounter;
      }
    }
  }

};
