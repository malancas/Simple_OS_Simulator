struct CPU : public Memory {
  //VARIABLES
  int currProcess;
  bool emptyCPU;

  //CONTRUCTORS
  CPU() { currProcess = -1, emptyCPU = true };

  //FUNCTIONS
  void waitForInput(){
    string input = "";
    while (input != 't'){
      cin >> input;
      if (emptyCPU && input != 'A'){
        cout << "No processes running. Add processes and try again" << '\n';
      }
      else if (input == 'A'){
        //Generate process, add to ready queue
      }
      else { 
        
      }
    }
  }

};
