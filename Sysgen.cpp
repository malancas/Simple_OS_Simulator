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
  float floatNum;

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
    //Set the number of printer device queues
    getInstallerInput_aux("Enter the number of printer devices: ", 'o');
    printerQueues.resize(num);

    //Set the number of disk device queues
    getInstallerInput_aux("Enter the number of disk devices: ", 'o');
    //diskSets0.resize(num);
    //diskSets1.resize(num);
    diskDeques0.resize(num);
    diskDeques1.resize(num);
    cylinderCount.resize(num);
    scanDiskQueuesStatus.resize(num);

    //The elements are set to false, meaning that scanDiskQueues0 will
    //begin the program as representing the scan queues
    fill(scanDiskQueuesStatus.begin(),scanDiskQueuesStatus.end(),false);

    //Set the number of CD device queues
    getInstallerInput_aux("Enter the number of CD devices: ", 'o');
    cdQueues.resize(num);

    //Set the history parameter
    getInstallerInput_aux("Enter the history parameter: ", 'h');
    historyParameter = floatNum;

    //Set the inital burst estimate
    getInstallerInput_aux("Enter the initial burst estimate: ", 'i');
    initialBurstEstimate = floatNum;
    cout << "Initial burst estimate: " << initialBurstEstimate << '\n';

    //Set the number of cylinders in each disk device
    int n = cylinderCount.size();
    //The two strings are used to create the message used in the user prompt.
    //The message's content depends on which disk device it asking about.
    string messageBase = "Enter the number of cylinders in disk device: ";
    string messageEnd = ": ";
    for (int i = 0; i < n; ++i){
      getInstallerInput_aux(messageBase+to_string(i+1)+messageEnd, 'o');
      cylinderCount[i] = num;
    }

    //Set the total memory size in the OS
    getInstallerInput_aux("Enter the total memory size: ", 'o');
    totalMemorySize = num;
    freeMemory = num;

    //Set the maximum process size
    getInstallerInput_aux("Enter the maximum process size: ", 'o');
    maximumProcessSize = num;

    //Set the page size
    getInstallerInput_aux("Enter the page size: ", 'p');
    pageSize = num;
  } 

  void getInstallerInput_aux(const string& userMessage, const char& variableCode){
    num = 0;
    cout << userMessage << '\n';
    while(!checkInputForErrors(variableCode)){
      cout << userMessage;
    }
    cout << '\n';
  }
  /*
    Prints userMessage to user before receiving input before
    parsing and checking for errors.
  */
  
  bool checkInputForErrors(const char& variableCode){
    string line;
    if (getline(cin, line)) {
      istringstream iss{line};
      //Checks if the input can be converted to an int
      if (variableCode == 'o' || variableCode == 'p'){
       if (iss >> num && (iss.eof() || isspace(iss.peek()))) {
          
         //If it was successfully converted, then checks if
         //num is negative
         if (num < 0){
           cerr << '\n' << "Negative number entered. Please try again" << '\n';
           return false;
         }

         if (variableCode == 'p'){
	   if (totalMemorySize % num){
	     cerr << "Chosen page number doesn't evenly divide the total system memory." << '\n';
	     cerr << "Enter a new number and try again." << '\n';
	     return false;
	   }
	   else{
            if (num > 0 && !(num & (num-1))){
              return true;
            }
            cerr << "Page number entered is not a power of two" << '\n';
            return false;
	   }
	 }
  
         /*
         If the function is being used to error check input for the history parameter,
         checkingHistoryParameter = true. If this is the case and the input entered
         is a valid integer, an additional function will be called to determine if the
         history parameter >= 0 and <= 1
         */
  
         //If num is not negative and if representing a history parameter
         //in the correct range, then the function will return true
         return true;            
       }
       else {
         cerr << '\n' << "Non numeric character entered. Please try again." << '\n';
         return false;
       }
      }
      else {
       if (iss >> floatNum && (iss.eof() || isspace(iss.peek()))) {       
         //If it was successfully converted, then checks if
         //num is negative
         if (floatNum < 0){
           cerr << '\n' << "Negative number entered. Please try again" << '\n';
           return false;
         }
  
         //If num is not negative and if representing a history parameter
         //in the correct range, then the function will return true
         if (variableCode == 'h'){
            if (!isHistoryParameterInRange()){
              cerr << "Input is not in correct range for the history parameter" << '\n';
              return false;
            }
          return true; 
         }
         return true;  
       }
       else {
         cerr << '\n' << "Non numeric character entered. Please try again." << '\n';
         return false;
       }
      }
    }
    return false;
  }

  /*
    Checks whether num is >= 0 and <= 1,
    which is necessary to be valid history parameter
  */
  bool isHistoryParameterInRange(){
    return (floatNum >= 0 && floatNum <= 1);
  }
};
