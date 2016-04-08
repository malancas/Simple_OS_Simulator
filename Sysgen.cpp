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
  bool goodInput;
  int num;

  //CONSTRUCTORS
  Sysgen() : goodInput(false), num(0) {}

  //FUNCTIONS
  /*
    Reads the user's chosen number of device
    queues and checks the input for errors.
    If the input is deemed valid, the device
    queue vectors will be resized appropriately
  */
  void getInstallerInput(){
    while (!goodInput){
      checkInputForErrors("Enter the number of printer devices: ");
    }
    printerQueues.resize(num);
    goodInput = false;

    cout << '\n';
    num = 0;
    while (!goodInput){
      checkInputForErrors("Enter the number of disk devices: ");      
    }
    diskQueues.resize(num);
    goodInput = false;
    
    cout << '\n';  
    num = 0;
    while (!goodInput){
      checkInputForErrors("Enter the number of CD devices: ");
    }
    cdQueues.resize(num);
    goodInput = false;
    
    cout << '\n';
    num = 0;
    while (!goodInput){
      checkInputForErrors("Enter the history parameter: ");
    }
    historyParameter = num;
    goodInput = false;

    cout << '\n';
    num = 0;
    while (!goodInput){
      checkInputForErrors("Enter the initial burst estimate: ");
    }
    initialBurstEstimate = num;
    goodInput = false;

    cout << '\n';
    num = 0;
    while (!goodInput){
      checkInputForErrors("Enter the number of cylinders: ");
    }
    cylinderCount.resize(num);
    cout << '\n';
  } 

  /*
    Prints userMessage to user before receiving input before
    parsing and checking for errors.
  */
  void checkInputForErrors(const string& userMessage){
    cout << userMessage;
    string line;
    if (getline(cin, line)) {
      istringstream iss{line};
      //Checks if the input can be converted to an int
      if (iss >> num && (iss.eof() || isspace(iss.peek()))) {
          
        //If it was successfully converted, then checks if
        //num is negative
        if (num < 0){
          cerr << '\n' << "Negative number entered. Please try again" << '\n' << '\n';
        }
          
        //If num is not negative, goodInput is set to true and loop breaks
        //meaning that num can be used to set how many devices there will
        //be of a certain type
        else{
          goodInput = true;            
        }
      }
      else {
        cerr << '\n' << "Non numeric character entered. Please try again." << '\n' << '\n';
      }  
    }   
  }
};
