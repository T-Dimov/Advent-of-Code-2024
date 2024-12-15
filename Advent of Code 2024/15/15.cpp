#include "map.h"

#include <iostream>

int main()
{
	const auto		fileName	{ "input.txt" };

	Map				map			{ fileName, true };
	map.moveRobot( false );

	std::cout << "Sum of boxes' coordinates: " << map.calculateSumOfBoxCoordinates() << '\n';
}
