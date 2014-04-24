#include <iostream>
#include <unistd.h>
#include <assert.h>
#include <cstdlib>
#include <chrono>
#include <thread>
using namespace std;


void Hello(int TID){
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    sleep(1);
    cout << "Hello from thread " <<   TID << endl;

}

int main(int argc, char *argv[ ]){
  int NT;
  if (argc <2) 
      NT = 8;
  else 
      NT = atoi(*++argv);
  cout << "Running with " << NT << " threads" << endl;


  thread *thrds = new thread[NT];

  // Fork threads
  for(int t=0; t<NT; t++){
      thrds[t] = thread(Hello,t);
  }

  // Join threads
  for(int t=0;t<NT;t++){
      thrds[t].join();
  }


  cout << endl;

  delete [] thrds;
  return 0;
}
