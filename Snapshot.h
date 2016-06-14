#include <sstream>
#include "Memory.h"

using namespace std;

struct Snapshot {
  Memory* mPtr;
  ostringstream os;


  Snapshot(Memory* ptr);


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

	template <typename T>
	void snapshotAux_Disk2(T& scanIt, const T& scanItEnd);

  //Prints average CPU usage time across the system
  void snapshotAux_memoryInformation();
};
