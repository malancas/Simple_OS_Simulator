#include <string>
#include <iostream>
#include <sstream>
#include "Memory.h"
using namespace std;

struct Sysgen : public Memory {
	//VARIABLES
  /*
    Used in various functions
    to determine if the user's
    input was valid
  */
  int num;

  //CONSTRUCTORS
  Sysgen() : num(0) {}

  //FUNCTIONS
  /*
    Reads the user's chosen number of device
    queues and checks the input for errors.
    If the input is deemed valid, the device
    queue vectors will be resized appropriately
  */
  void getInstallerInput(){
    getInstallerInput_aux("Enter the number of printer devices: ", false);
    printerQueues.resize(num);

    getInstallerInput_aux("Enter the number of disk devices: ", false);
    diskQueues.resize(num);
    
    getInstallerInput_aux("Enter the number of CD devices: ", false);
    cdQueues.resize(num);
    
    getInstallerInput_aux("Enter the history parameter: ", true);
    historyParameter = num;

    getInstallerInput_aux("Enter the initial burst estimate: ", false);
    initialBurstEstimate = num;

    getInstallerInput_aux("Enter the number of cylinders: ", false);
    cylinderCount.resize(num);
  } 

  void getInstallerInput_aux(const string& userMessage, const bool& checkingHistoryParameter){
    num = 0;
    cout << userMessage << '\n';
    while(!checkInputForErrors(checkingHistoryParameter)){
      cout << userMessage;
    }
    cout << '\n';
  }
  /*
    Prints userMessage to user before receiving input before
    parsing and checking for errors.
  */
  bool checkInputForErrors(const bool& checkingHistoryParameter){
    string line;
    if (getline(cin, line)) {
      istringstream iss{line};
      //Checks if the input can be converted to an int
      if (iss >> num && (iss.eof() || isspace(iss.peek()))) {
          
        //If it was successfully converted, then checks if
        //num is negative
        if (num < 0){
          cerr << '\n' << "Negative number entered. Please try again" << '\n';
	  return false;
	}
	
	/*
	  If the function is being used to error check input for the history parameter,
	  checkingHistoryParameter = true. If this is the case and the input entered
	  is a valid integer, an additional function will be called to determine if the
	  history parameter >= 0 and <= 1
	*/
	if (checkingHistoryParameter){

	  if (!isHistoryParameterInRange()){
	    cerr << "Chosen number isn't in the acceptable >= 0 and <= 1 range." << '\n';
	    cerr << "Please enter a new number and try again." << '\n';
	    return false;
	  }
	}
	
        //If num is not negative and if representing a history parameter
	//in the correct range, then the function will return true
        return true;            
      }
      else {
        cerr << '\n' << "Non numeric character entered. Please try again." << '\n';
	return false;
      }  
    }
    return false;
  }

  /*
    Checks whether num is >= 0 and <= 1,
    which is necessary to be valid history parameter
  */
  bool isHistoryParameterInRange(){
    return (num >= 0 && num <= 1);
  }
};
