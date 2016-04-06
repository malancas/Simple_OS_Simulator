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

        //Amount of time the process has used the CPU
        double cpuTime;
  
  	//CONSTRUCTORS
  Process() : pid(), type(), name(), memStart(), length(), cpuTime() {}
  Process(int pd) : pid(pd), type(), name(), memStart(), length(), cpuTime() {}
  Process(int id, string ty, string n, int mL, int len, double cpT) : pid(id), type(ty), name(n), memStart(mL), length(len), cpuTime(cpT) {}

	//OPERATOR OVERLOADING
	friend ostream& operator<<(ostream& os, const Process& obj)
	{
	  os << obj.pid << " " << obj.name << " " << obj.memStart << " " << obj.type << " " << obj.length <<
	     obj.cpuTime << '\n';
    	return os;
	}
};

