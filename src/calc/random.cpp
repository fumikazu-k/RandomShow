#include "random.h"

Random::Random()
{
	std::random_device rnd;
	mt.seed(rnd());
}

uint32_t Random::getIntRange(const uint32_t min, const uint32_t max)
{
	static uint32_t preVal = UINT32_MAX;
	uint32_t retVal;

	do{
		std::uniform_int_distribution<int> dist(min, max);
		retVal = dist(mt);

	}while(retVal == preVal); //前回と同じならもう一度計算

	preVal = retVal;

	return retVal;
}