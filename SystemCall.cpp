#include <string>
using namespace std;

struct SystemCall {
	//VARIABLES
	int id;
	char type;
	string name;
	int memLocation;
	bool read;
	int length;

	//CONTRUCTORS
	SystemCall() : type('n'), name("default"), memLocation(-3), read(false), length(-8) {}
	//output overloader

	//FUNCTIONS
};