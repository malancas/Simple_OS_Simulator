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
  //A, t, p<num>, P<num>, d<num>, D<num>, c<num>, C<num>
  //system calls: S -> s, d, c, r
  void waitForInput(){
    cout << "CPU empty, waiting for input..." << '\n';
    string input = "";

    while (input != "q"){
      cin >> input;

      //If user tries to add processes to device queues or
      //use Snapshot feature when no processes exist in CPU
      if (emptyCPU && (input != "A")){
        cout << "No processes running. Add processes and try again" << '\n';
      }

      //If the CPU is empty and the user issues an 'A'
      //Creates new process, adds it to CPU
      else if (emptyCPU && input == "A"){
        processes.insert(make_pair(pidCounter,Process(pidCounter)));
        if (readyQueue.empty()){
          currProcess = pidCounter;
          emptyCPU = false;          
        }
        else {
          currProcess = readyQueue.front();
          readyQueue.pop();
          readyQueue.push(pidCounter);
        }
        ++pidCounter;
        cout << "New process made!" << '\n';
      }

      //If the CPU isn't empty and the user issues an 'A'
      //Adds new process to ready queue
      else if (!emptyCPU && input == "A"){
        processes.insert(make_pair(pidCounter,Process(pidCounter)));
        readyQueue.push(pidCounter);
        ++pidCounter;
      }

      //If the user tries to terminate a process
      //while the CPU is empty
      else if (input == "t"){
        if (emptyCPU){
          cout << "Error: CPU is unoccupied, no process can be terminated" << '\n';
        }
        else {
          processes.erase(currProcess);
          currProcess = readyQueue.front();
          readyQueue.pop();
          cout << "Process terminated" << '\n';
        }
      }
      //If the user types 'S' for the snapshot function
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
    return;
  }
};