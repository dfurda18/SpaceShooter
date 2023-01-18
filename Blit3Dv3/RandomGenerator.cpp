#include "RandomGenerator.h"

/**
* Constructor of the RandomGenerator object.
*/
RandomGenerator::RandomGenerator() 
{
	std::random_device rd;
	this->SeedRNG(rd());
}
/**
* Seeds the RandomGenerator.
* @param unsigned int The value to seed the random device.
*/
void RandomGenerator::SeedRNG(unsigned int seedVal) 
{
	rng.seed(seedVal);
}
/**
* Generates a random float between two numbers and with decimals related to the precision specified.
* If presicion is 100, the number will have at most two decimal components.
* @param int The minimum value
* @param int The maximum value
* @param int The precision of the number.
* @return A random float between min and max.
*/
float RandomGenerator::RandomFloat(int min, int max, int precision)
{
	int distance = (max - min) * precision;
	std::uniform_int_distribution<unsigned int> uint_distDice(0, distance);
	distance = (uint_distDice(rng) / precision) + min;
	return distance;
}
/**
* Generates a random int between two numbers.
* @param int The minimum value
* @param int The maximum value
* @return A random int between min and max.
*/
int RandomGenerator::RandomInt(int min, int max)
{
	std::uniform_int_distribution<unsigned int> uint_distDice(min, max);
	return uint_distDice(rng);
}