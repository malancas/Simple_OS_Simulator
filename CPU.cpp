#include <iostream>
#include <string>
#include <iterator>
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

  //A, t, p<num>, P<num>, d<num>, D<num>, c<num>, C<num>
  //system calls: S -> s, d, c, r
  void waitForInput(){
    string input = "";
    while (input != "t"){
      cin >> input;
      if (emptyCPU && input != "A"){
        cout << "No processes running. Add processes and try again" << '\n';
      }
      else if (emptyCPU && input == "A"){
        processes.insert(make_pair(pidCounter,Process(pidCounter, pcbCounter)));
        if (readyQueue.empty()){
          currProcess = pidCounter;
          emptyCPU = false;          
        }
        else {
          currProcess = readyQueue.front();
          readyQueue.pop();
          readyQueue.push(pidCounter);
        }
        ++pidCounter; ++pcbCounter;
      }
      else if (!emptyCPU && input == "A"){
        processes.insert(make_pair(pidCounter,Process(pidCounter, pcbCounter)));
        readyQueue.push(pidCounter);
        ++pidCounter; ++pcbCounter;
      }
      else if (input == "t"){
        if (emptyCPU){
          cout << "Error: CPU is unoccupied, no process can be terminated" << '\n';
        }
        else {
          processes.erase(currProcess);
          currProcess = readyQueue.front();
          readyQueue.pop();
        }
      }
      else if (input == "S"){
        cout << "Enter one of the following: r,p,c,d" << '\n';
        cin >> input;
        if (input == "r"){
          cout << input;
        }
        else if (input == "p"){
          cout << input;
        }
        else if (input == "c"){
          cout << input;
        }
        //input == d
        else {
          cout << input;
        } 
      }
    }
  }
};
