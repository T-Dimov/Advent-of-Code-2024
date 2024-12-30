#include "map.h"

#include <iostream>

void day08(std::string_view fileName)
{
	const Map	map	{ fileName };

	std::cout << "Number of locations with antinodes: "								<< map.getAntiNodesCount()				<< '\n';
	std::cout << "Number of locations with antinodes(with resonant harmonics): "	<< map.getAntiNodesWithHarmonicsCount()	<< '\n';
}
