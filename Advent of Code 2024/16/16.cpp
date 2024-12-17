#include "maze.h"

#include <iostream>

int main()
{
	const auto		fileName	{ "input.txt" };

	Maze			maze		{ fileName };

	std::cout << maze.findLowestCostRoute();
}
