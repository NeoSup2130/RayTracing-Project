#include "MyRandom.h"

#define FAST_RAND false
#if FAST_RAND
#define MAX_RAND 1.0f / (float)(0xFFFFFFFF)
#else
#define MAX_RAND 0xFFFFFFFF
#endif

MyRandom::MyRandom(unsigned int& seed)
    : m_Seed(seed)
{
}

// Algorithm - XorShift RNGs from p. 4 of Marsaglia
// 
unsigned int MyRandom::xorshift32()
{
    unsigned int x{ m_Seed + 1 };
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    m_Seed = x;
    return x;
}

float MyRandom::GetAFloatBetween(const float& min, const float& max)
{
#if FAST_RAND
    float x = (float)xorshift32() * (float)MAX_RAND;
#else
    float x = (float)xorshift32() / (float)MAX_RAND;
#endif
    return x * (max - min) + min;
}

float MyRandom::GetUniformFloat()
{
#if FAST_RAND
    return (float)xorshift32() * (float)MAX_RAND;
#else
    return (float)xorshift32() / (float)MAX_RAND;
#endif
}
