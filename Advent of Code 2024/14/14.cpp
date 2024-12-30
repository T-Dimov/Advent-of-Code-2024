#include "space.h"

#include <iostream>

void day14part1(std::string_view fileName)
{
	Space	space	{ fileName, { 101, 103 } };

	std::cout << "Safety factor after 100 second: " << space.calculateSafetyFactorAfter( 100 ) << '\n';
}

void day14part2(std::string_view fileName)
{
	Space		space		{ fileName, { 101, 103 } };

	const int	searchStart	{ 0 };
	space.calculateSafetyFactorAfter( searchStart );
	std::cout << "Easter egg after " << searchStart + space.findSpaceWithFrameOfRobots() << " seconds\n";
	space.printGrid();
}
