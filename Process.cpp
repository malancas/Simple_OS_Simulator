#include <string>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Process {
  //VARIABLES
    
  int pid;
    
  //Whether the process is read or write
  string type;

  string name;

  //Starting location in memory
  int memStart;

  //Starting location in logical memory, represented in hex
  string logicalMemoryAddress;

  int length;

  //The amount of time the process needs in the Cpu to complete
  float burstEstimate;

  //The estimation of the process' next burst
  float nextBurstEstimate;

  float remainingBurst;

  //The total amount of time spent in Cpu
  float totalCpuTime;

  //Number of times the process occupied the Cpu
  int cpuUsageCount;

  //the track that the process is stored on
  int track;

  //In terms of pages
  int size;

  int physicalAddress;

  //Used to determine where a process is at any moment
  string locationCode;

  vector<int> pageTable;

  //CONSTRUCTORS
  Process() : pid(), type(), name(), memStart(), length(), burstEstimate(),
   nextBurstEstimate(), remainingBurst(0), totalCpuTime(0), cpuUsageCount(0), 
   track(), size(-1), locationCode("") {}
  
  Process(int pd, int burstEst, int sz) : pid(pd), type(), name(), memStart(),
   length(), burstEstimate(burstEst), nextBurstEstimate(), remainingBurst(burstEst), 
   totalCpuTime(0), cpuUsageCount(0), track(), size(sz), locationCode("") {}

  //OPERATOR OVERLOADING
  friend ostream& operator<<(ostream& os, const Process& obj)
  {
    os << obj.pid << " " << obj.name << " " << obj.memStart << " " 
    << obj.type << " " << obj.length << " " << obj.totalCpuTime 
    << " " << obj.totalCpuTime/obj.cpuUsageCount << '\n';
    return os;
  }
};
