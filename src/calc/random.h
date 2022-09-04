#pragma once
#include <random>

class Random
{
public:
	Random();
	uint32_t getIntRange(const uint32_t min, const uint32_t max);
private:
	std::mt19937_64 mt;
};