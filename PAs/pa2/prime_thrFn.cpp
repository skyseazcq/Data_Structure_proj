#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <thread>
#include <vector>



bool isPrime(int64_t x);

// Shared globals, allocated before threads are spawned
// These will be used in the threaded code
extern std::vector<int64_t> candidates; // Candidate primes
extern std::vector<bool> primeFlags;    // Prime flags

// This is the "thread function" that will be called by a newly created thread
// This provided code currently runs on only 1 thread
// TODO:
// Change this code to subdivide the n primes into disjoint sets
// properly handling the case whtn the number of threads doesn't divide N
//
//
// DO NOT change the signature for this code as we rely on
// it to test your assignment
//
// 
// This function should perform no I/O when you are timing the code
void prime_th(int TID, int NT, int n)
{
    int tmp_itrvl = n / NT;
    for ( int i = tmp_itrvl * TID; i < tmp_itrvl * (TID + 1); i++ ){
	if (isPrime(candidates[i])){
		std::cout << candidates[i] << "is prime\n";
//		primeFlags[i] = true;
	}
    }

    //for (int i=0;i<n; i++) {
    //    if (isPrime (candidates[i])){
    //        std::cout << candidates[i] << " is prime\n";
    //    }
    //}

}
