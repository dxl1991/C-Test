#pragma once
typedef unsigned int uint;
class CSRandom
{
public:
	CSRandom(int seed);
	~CSRandom()
	{
		delete[] mt;
	}
	int next();
	int next(int minValue, int maxValue);

private:
	const int N = 624;
	const int M = 397;
	const uint MATRIX_A = 0x9908b0dfU;
	const uint UPPER_MASK = 0x80000000U;
	const uint LOWER_MASK = 0x7fffffffU;
	const int MAX_RAND_INT = 0x7fffffff;
	uint mag01[2] = { 0x0U, MATRIX_A };
	uint* mt = new uint[N];
	int mti = N + 1;

	void initGenRandom(uint s);
	uint genRandom();
	int genRandomInt();
};

