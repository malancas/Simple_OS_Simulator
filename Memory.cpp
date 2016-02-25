#include <unordered_map>
#include "Process.cpp"

struct Memory {
  //VARIABLES
  static unordered_map<Process> processes
  static int pidCounter;
  queue<int> readyQueue;
  //PCB index?

  //CONSTRUCTORS
  Memory m { pidCounter = 0 };

  //FUNCTIONS
  void addProcesstoQueue(){
    processes.add(Process(pidCounter))
    readyQueue.push_back()
  }
};
