#include "map.h"

#include <iostream>

int main()
{
	const auto		fileName	{ "input.txt" };

	Map				map			{ fileName, false };
	map.print();
	map.moveRobot();
	map.print();
	const size_t	sum			{ map.calculateSumOfBoxCoordinates() };
	std::cout << "Sum of boxes' coordinates: " << sum << '\n';
}
