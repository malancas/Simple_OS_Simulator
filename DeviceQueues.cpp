#include <unordered_map>
#include <queue>
#include <vector>
using namespace std;

struct DeviceQueues{
  //VARIABLES
  /* unordered hash that associates device name input with
     queue that was specified during sysgen */ 
  vector<queue<int>> printerQueues;
  vector<queue<int>> diskQueues;
  vector<queue<int>> cdQueues;

  //CONTRUCTORS
  DeviceQueues();

  //FUNCTIONS
  makeQueues(int& pNum, int& dNum, int& cNum){
    printerQueues.resize(pNum);
    diskQueues.resize(dNum);
    cdQueues.resize(cNum);
  }
};
