//
// A timer
//
//
// DO NOT change the signature for this code as we rely on
// it to test your assignment
//


#include <sys/time.h>
#include <iostream>

const double kMicro = 1.0e-6;

double getTime()
{
    struct timeval TV;

    const int RC = gettimeofday(&TV, NULL);
    if(RC == -1)
    {
        std::cerr << "ERROR: Bad call to gettimeofday()\n";
        return(-1);
    }

    return( ((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec) );

}  // end getTime()
