#include <string>
using namespace std;

struct Process {
  //VARIABLES
  	int pid;
  	char type;
 	string name;
	int memLocation;
	bool read;
	int length;

  	//CONSTRUCTORS
	Process() : pid(), type(), name(), memLocation(), read(), length() {}
	Process(int pd) : pid(pd), type(), name(), memLocation(), read(), length() {}
	Process(int id, char ty, string n, int mL, bool rd, int len) : pid(id), type(ty), name(n), memLocation(mL), 
		read(rd), length(len) {}

  	//FUCNTIONS
};