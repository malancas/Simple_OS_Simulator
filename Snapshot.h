#include "Cpu.h"
using namespace std;

struct Snapshot : public Cpu {
  
  void snapshotHeader();

  /*
    Prints the data of each process in the chosen device deque
    Each process gets its own line
  */
  template<typename T>
  void snapshotPrint(T& itB, T& itE);

  void snapshotAux(const string& input);

  void snapshotAux_SystemInformation();

  void snapshotAux_ReadyDeque();

  void snapshotAux_JobPool();

  void snapshotAux_Disk();

  void snapshotAux_Disk2(deque<int>::iterator scanIt, deque<int>::iterator scanItEnd);

  void snapshotAux_memoryInformation();
};
