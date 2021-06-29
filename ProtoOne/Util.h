#ifndef UTIL_H_
#define UTIL_H_

#include <cstdlib>

/*
================================================================================

InitRandom

    Functions to initialize the global random number generator.
    Should only be called once at the start of the application, unless
    there is a good reason otherwise.

================================================================================
*/
void InitRandom();                  // initialize using a randomized seed
void InitRandom(unsigned seed);     // initialize using the specified seed

/*
================================================================================

UnitRandom

    Return a float in the range [0, 1)
    
================================================================================
*/
inline float UnitRandom()
{
    return std::rand() / (RAND_MAX + 1.0f);
}

/*
================================================================================

RandomInt

    Return an int in the range [0, upper)

    Good for returning random array indexes.
    
================================================================================
*/
inline int RandomInt(int upper)
{
    return (int)(upper * UnitRandom());
}

/*
================================================================================

RandomInt

    Return an int in the range [lower, upper)
    
================================================================================
*/
inline int RandomInt(int lower, int upper)
{
    return lower + (int)((upper - lower) * UnitRandom());
}

/*
================================================================================

RandomFloat

    Return a float in the range [lower, upper)
    
================================================================================
*/
inline float RandomFloat(float lower, float upper)
{
    return lower + (upper - lower) * UnitRandom();
}

/*
================================================================================

RandomSign
    
    Return either 1 or -1

================================================================================
*/
inline int RandomSign()
{
    return UnitRandom() < 0.5f ? 1 : -1;
}

/*
================================================================================

UnitRandomInclusive

    Return a float in the range [0, 1]

================================================================================
*/
inline float UnitRandomInclusive()
{
    return rand() / (float)RAND_MAX;
}

/*
================================================================================

RandomIntInclusive

    Return an int in the range [lower, upper]

================================================================================
*/
inline int RandomIntInclusive(int lower, int upper)
{
    return lower + (int)((upper - lower + 1) * UnitRandom());
}

/*
================================================================================

RandomFloatInclusive

    Return a float in the range [lower, upper]

================================================================================
*/
inline float RandomFloatInclusive(float lower, float upper)
{
    return lower + (upper - lower) * UnitRandomInclusive();
}

#endif
