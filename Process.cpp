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
  int totalBurst;

  //The average burst of the process
  double averageBurst;

  //Number of times the process occupied the CPU
  int cpuUsageCount;

  //The estimation of the process' next burst
  double estimatedNextBurst;

  //the cylinder track that the process is stored on
  int cylinderTrack;

  //CONSTRUCTORS
  Process() : pid(), type(), name(), memStart(), length(), totalBurst(), averageBurst(0), cpuUsageCount(0), estimatedNextBurst(), cylinderTrack() {}
  Process(int pd, int burstEst) : pid(pd), type(), name(), memStart(), length(), totalBurst(burstEst), averageBurst(0), cpuUsageCount(0), estimatedNextBurst(), cylinderTrack() {}

	//OPERATOR OVERLOADING
	friend ostream& operator<<(ostream& os, const Process& obj)
	{
	  os << obj.pid << " " << obj.name << " " << obj.memStart << " " << obj.type << " " << obj.length << " " << obj.totalBurst << " " << obj.averageBurst << " " << obj.cpuUsageCount << " " << obj.estimatedNextBurst << '\n';
    	return os;
	}
};

