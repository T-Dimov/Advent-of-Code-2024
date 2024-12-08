#include "map.h"

#include <iostream>

int main()
{
	const auto	fileName	{ "input.txt" };
	const Map	map			{ fileName };

	std::cout << "Number of locations with antinodes: "								<< map.getAntiNodesCount()				<< '\n';
	std::cout << "Number of locations with antinodes(with resonant harmonics): "	<< map.getAntiNodesWithHarmonicsCount()	<< '\n';
}
