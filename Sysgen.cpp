#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Memory.cpp"
using namespace std;

struct Sysgen : public Memory {
	//VARIABLES
  class NegativeError {};
  bool goodInput;
  int num;

  //CONSTRUCTORS
  Sysgen() : goodInput(false), num(0) {}

  //FUNCTIONS
  void getUserDeviceInput(){
  	checkInputForErrors("Enter the number of printer devices: ");
  	printerQueues.resize(num);
    goodInput = false;

  	checkInputForErrors("Enter the number of disk devices: ");
  	diskQueues.resize(num);
    goodInput = false;
  		
  	checkInputForErrors("Enter the number of CD devices: ");
  	cdQueues.resize(num);
  }

  void checkInputForErrors(const string& userMessage){
    while (!goodInput){
      cout << userMessage << '\n';
      string line;
      if (getline(cin, line)) {
        istringstream iss{line};
        //Checks if the input can be converted to an int
        if (iss >> num && (iss.eof() || isspace(iss.peek()))) {
          
          //If it was successfully converted, then checks if
          //num is negative
          if (num < 0){
            cerr << "Negative number entered. Please try again" << '\n';
          }
          
          //If num is not negative, goodInput is set to true and loop breaks
          //meaning that num can be used to set how many devices there will
          //be of a certain type
          else{
            goodInput = true;            
          }
        }
        else {
          cerr << "Non numeric character entered. Please try again." << '\n';
        }  
      }   
    }
  }
};