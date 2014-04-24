// Program to sum two vectors with multiple threads
// Scott B. Baden, CSE Dept, UCSD
// 10 Apr 2013
// 

#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <cmath>
#include <thread>

//
// Globals
//
  double *x, *y, *z; 
  const int REPS = 16;

double getTime();

void vector(int TID, int N, int NT){
    int i0 = TID*(N/NT),  i1   = i0 + (N/NT);
    for (int r =0; r<REPS; r++)
	for (int i=i0;  i<i1;  i++) 
#ifdef HEAVY
	    z[i] += exp(x[i]);
#else
	    z[i] += x[i] + y[i];
#endif

}

int main(int argc, char **argv){
// Parse commmand line arguments
// Default values
  int NT = 4;
// Input size in Meg
  int N = 8;
  if (argc >= 2)
      NT = atoi(*++argv); 
  if (argc >= 3)
      N = atol(*++argv);

  std::cout << "N (in Meg): " << N << std::endl;
  std::cout << "NT: " << NT << std::endl;
  std::cout << "Reps = " << REPS << std::endl;

  //
  // Multiply by 2^20
  //

  N <<= 20;

  if (N%NT != 0){
     std::cerr << "# Threads Must divide N" << std::endl;
     exit(0);
  }



//
// Initialize shared state
//
  assert(x = new double[N]);
  assert(y = new double[N]);
  assert(z = new double[N]);

// Initialize input array
  for (int i=0; i < N; i++){
      x[i]  = ((double) i) / ( (double) N);
      y[i]  = ((double) (N-i)) / ( (double) N);
      z[i] = 0;
  }

  std::thread *thrds = new std::thread[NT];

  double tp = -getTime();
  // Fork threads
  for(int t=0; t<NT; t++){
      thrds[t] = std::thread(vector,t,N,NT);
  }
  // join threads
  for(int t=0;t<NT;t++){
      thrds[t].join();
  }

  tp += getTime();

  std::cout << "Running time on " << NT << " threads: " << tp << std::endl;


  delete [] x;
  delete [] y;
  delete [] z;
  delete [] thrds;
  return 0;
}

