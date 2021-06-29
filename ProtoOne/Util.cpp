#include "Util.h"

#include <ctime>

void InitRandom()
{
    int i, n;

    // seed the random number generator
    std::srand((unsigned)std::time(NULL) ^ (unsigned)std::rand());

    //
    // On some plarforms, similar random number seeds produce random number
    // sequences that are similar initially, but diverge eventually.
    //
    // To help circumvent this, pull a random number of random numbers
    // from the start of the sequence.
    //
    n = 10 + rand() % 10;
    for (i = 0; i < n; i++) {
        std::rand();
    }
}

void InitRandom(unsigned seed)
{
    std::srand(seed);
}
