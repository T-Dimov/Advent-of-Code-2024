#include "space.h"

#include <iostream>

int main()
{
	Space	testSpace	{ "test.txt", { 11, 7 } };
	Space	space		{ "input.txt", { 101, 103 } };
	Space	space2		{ "input.txt", { 101, 103 } };

	std::cout << "Safety factor after 100 second: " << space.calculateSafetyFactorAfter( 100 ) << '\n';
	const int	searchStart { 0 };
	space2.calculateSafetyFactorAfter( searchStart );
	std::cout << "Easter egg after " << searchStart + space2.findSpaceWithFrameOfRobots() << " seconds\n";
	space2.printGrid();
}
