#include <string>
#include <iostream>
#include "Memory.cpp"
using namespace std;

struct Sysgen : public Memory {
	class NegativeError {};

  	//FUNCTIONS
  	void getUserDeviceInput(){
  		int num = 0;
  		checkInputForErrors("Enter the number of printer devices: ", num);
  		printerQueues.resize(num);

  		checkInputForErrors("Enter the number of disk devices: ", num);
  		diskQueues.resize(num);
  		
  		checkInputForErrors("Enter the number of CD devices: ", num);
  		cdQueues.resize(num);
  	}

  	void checkInputForErrors(const string& userMessage, int& num){
  		bool goodInput = false;
  		while (!goodInput) {
        goodInput = true;
    		try {
      			cout << userMessage << '\n';
      			cin >> num;

      			if (num < 0){
        			throw NegativeError();
      			}
    		}
    
    		catch (NegativeError){
      			cerr << "Negative number entered. Try again." << '\n' << '\n';
      			goodInput = false;      
    		}
  		}
  	}
};