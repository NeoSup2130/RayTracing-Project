#pragma once
class MyRandom
{
public:
	MyRandom(unsigned int& seed);
	unsigned int xorshift32();
	float GetAFloatBetween(const float& min, const float& max);
	//A value between 0.0f and 1.0f
	float GetUniformFloat();
private:
	unsigned int& m_Seed;
};

