#include <string>
#include <sstream>
#include <iostream>
using namespace std;

struct Process {
  //VARIABLES
    
    int pid;
    
    //Whether the process is read or write
    string type;

    //the filename
  string name;

  //Starting location in memory
  int memStart;

  //file length
  int length;

  //The amount of time the process needs in the CPU to complete
  float burstEstimate;

  //The estimation of the process' next burst
  float nextBurstEstimate;

  float remainingBurst;

  //The total amount of time spent in CPU
  float totalCPUTime;

  //Number of times the process occupied the CPU
  int cpuUsageCount;

  //the cylinder track that the process is stored on
  int cylinderTrack;

  //CONSTRUCTORS
  Process() : pid(), type(), name(), memStart(), length(), burstEstimate(),
   nextBurstEstimate(), remainingBurst(0), totalCPUTime(0), cpuUsageCount(0), cylinderTrack() {}
  
  Process(int pd, int burstEst) : pid(pd), type(), name(), memStart(),
   length(), burstEstimate(burstEst), nextBurstEstimate(), remainingBurst(burstEst), 
   totalCPUTime(0), cpuUsageCount(0), cylinderTrack() {}

  //OPERATOR OVERLOADING
  friend ostream& operator<<(ostream& os, const Process& obj)
  {
    os << obj.pid << " " << obj.name << " " << obj.memStart << " " 
    << obj.type << " " << obj.length << " " << obj.totalCPUTime 
    << " " << " " << obj.cpuUsageCount << " " << obj.nextBurstEstimate << '\n';
    return os;
  }
};

