#include "CSRandom.h"
#include <iostream>

CSRandom::CSRandom(int seed)
{
	initGenRandom((uint)seed);
}

int CSRandom::next()
{
	return genRandomInt();
}

int CSRandom::next(int minValue, int maxValue)
{
	if (minValue > maxValue)
	{
		int tmp = maxValue;
		maxValue = minValue;
		minValue = tmp;
	}

	int range = maxValue - minValue;
	return minValue + next() % range;
}

void CSRandom::initGenRandom(uint s)
{
	mt[0] = s & 0xffffffffU;
	for (mti = 1; mti < N; mti++)
	{
		mt[mti] = (uint)(1812433253U * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
		mt[mti] &= 0xffffffffU;
	}
}

uint CSRandom::genRandom()
{
	uint y;
	if (mti >= N)
	{
		int kk;
		if (mti == N + 1)
			initGenRandom(5489U);
		for (kk = 0; kk < N - M; kk++)
		{
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
		}
		for (; kk < N - 1; kk++)
		{
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
		}
		y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];
		mti = 0;
	}
	y = mt[mti++];
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680U;
	y ^= (y << 15) & 0xefc60000U;
	y ^= (y >> 18);
	return y;
}

int CSRandom::genRandomInt()
{
	return (int)(genRandom() >> 1);
}
