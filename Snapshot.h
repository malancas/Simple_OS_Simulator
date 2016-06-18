#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <sstream>
#include "Memory.h"

using namespace std;

struct Snapshot {
  Memory* mPtr;
  ostringstream os;


  Snapshot(Memory* ptr);


  void header();

  //Prints the data of each process in the chosen device deque
  //Each process gets its own line
  template<typename T>
  void snapshotPrint(T& itB, T& itE);

  void snapshotAux(const string& input);

  void insertSystemInformation();

  void insertReadyDequeContent();

  void insertJobPoolContent();

  void insertDiskContent();

	template <typename T>
	void insertDiskContent2(T& scanIt, const T& scanItEnd);

  //Prints average CPU usage time across the system
  void insertMemoryInformation();

  void handleInputChoice();
};

#endif