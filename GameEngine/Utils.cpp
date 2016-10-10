#include "Utils.h"
std::random_device __rd;
std::mt19937 __mt(__rd());
float getRandom(float min, float max)
{
	std::uniform_real_distribution<float> __dist(min, max);
	return __dist(__mt);
}