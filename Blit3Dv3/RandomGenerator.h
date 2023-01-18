#pragma once

#include <random>
/**
* This class represents handles random number generation.
*/
class RandomGenerator
{
private:
	/** 
	* The randome device used.
	*/
	std::mt19937 rng;
public:
	/**
	* Constructor of the RandomGenerator object.
	*/
	RandomGenerator();
	/**
	* Seeds the RandomGenerator.
	* @param unsigned int The value to seed the random device.
	*/
	void SeedRNG(unsigned int seedVal);
	/**
	* Generates a random floar between two numbers and with decimals related to the precision specified.
	* If presicion is 100, the number will have at most two decimal components.
	* @param int The minimum value
	* @param int The maximum value
	* @param int The precision of the number.
	* @return A random float between min and max.
	*/
	float RandomFloat(int min, int max, int precision);
	/**
	* Generates a random int between two numbers.
	* @param int The minimum value
	* @param int The maximum value
	* @return A random int between min and max.
	*/
	int RandomInt(int min, int max);
};