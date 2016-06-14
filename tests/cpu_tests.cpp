#include "../Memory.h"
#include "../Cpu.h"

using namespace std;

int main(){
	Memory m;
	Memory* mptr = &m;
	Cpu c(mptr);

	return 0;
}