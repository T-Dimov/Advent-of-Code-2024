#include "map.h"

#include <iostream>

void day15part1(std::string_view fileName)
{
	Map	map	{ fileName, false };
	map.moveRobot( false );

	std::cout << "Sum of boxes' coordinates: " << map.calculateSumOfBoxCoordinates() << '\n';
}

void day15part2(std::string_view fileName)
{
	Map	map	{ fileName, true };
	map.moveRobot( false );

	std::cout << "Sum of wide boxes' coordinates: " << map.calculateSumOfBoxCoordinates() << '\n';
}
