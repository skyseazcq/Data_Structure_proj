#include <stdlib.h>
#include <iostream>
//
// Tests for primality
// Based on Java code written by Alan Kaminsky
// Ported to C++ by Scott B. Baden, UCSD, Dec. 28, 2010
//
// DO NOT change the signature for this code as we rely on
// it to test your assignment
//

//
// Returns true if x is prime, else false
bool isPrime(int64_t x) {
    std::cout << "in test\n";

    if (x == 2) return true;
    if (x % 2 == 0) return false;
    int64_t p = 3;
    int64_t psqr = p*p;
    while (psqr <= x) {
        if (x % p == 0) return false;
        p += 2;
        psqr = p*p;
    }
    return true;
}
