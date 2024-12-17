#include "maze.h"

#include <iostream>

int main()
{
	const auto		fileName	{ "input.txt" };

	Maze			maze		{ fileName };

	std::cout << "Lowest cost of a route from S to E: " << maze.findLowestCostRoute() << '\n';
	std::cout << "Number of cells visited by lowest-cost routes from S to E: " << maze.getVisitedCellsCount() << '\n';
}
