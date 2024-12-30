#include "topoMap.h"

#include <iostream>

void day10(std::string_view fileName)
{
	TopoMap	topoMap	{ fileName };
	auto [scoresSum, ratingsSum]	= topoMap.getTrailheadsScoresSumAndRatingsSum();

	std::cout << "Sum of the scores of all trailheads: " << scoresSum << '\n';
	std::cout << "Sum of the ratings of all trailheads: " << ratingsSum << '\n';
}
