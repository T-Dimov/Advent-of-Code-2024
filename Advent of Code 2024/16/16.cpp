#include "maze.h"

#include <iostream>

void day16(std::string_view fileName)
{
	Maze	maze	{ fileName };

	std::cout << "Lowest cost of a route from S to E: " << maze.findLowestCostRoute() << '\n';
	std::cout << "Number of cells visited by lowest-cost routes from S to E: " << maze.getVisitedCellsCount() << '\n';
}
