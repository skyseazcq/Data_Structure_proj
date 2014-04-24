#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <vector>

// Global shared vectors 
extern std::vector<int64_t> candidates;
extern std::vector<bool> primeFlags; 

// TODO: This code currently reads only the number of candidate primes
// off the command line
// You need to add code read in the number of threads and also to read
// athe candidate primes into a vector
//
// e.g. If we run ./primesP 2 5 7,
// the number of threads (NT) is 2
// and the candidates are 5 and 7
//
//
// DO NOT change the signature for this code as we rely on
// it to test your assignment
//
void getInput(int argc, char *argv[], int &n, int &NT){
    n = argc; n-=1;
    std::cout << "Testing " << n-1 << " candidate primes" << std::endl;

    // TODO: Read in the number of threads and candidate primes

    NT = atoi(argv[1]); // atoi:translate to int

    for ( int i = 2; i <= n; i++ ){
	int64_t tmp_var = atoll(argv[i]);	
	candidates.push_back(tmp_var);
    }

    n = n - 1;
}
