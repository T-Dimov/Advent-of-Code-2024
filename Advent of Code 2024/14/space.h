#pragma once

#include "robot.h"

#include <vector>

class Space
{
public:
			Space(std::string_view fileName, const Point2D& size);

	size_t	calculateSafetyFactorAfter(size_t seconds);
	size_t	findLowestSafetyFactorConfiguration();
	size_t	findSpaceWithFrameOfRobots();

	void	showAllGrids();
	void	printGrid() const;

private:

	void	calculateGridAfter(size_t seconds);
	size_t	gridHasVerticalLineOfRobots() const;
	bool	gridHasHorizontalLineOfRobotsAtRow(size_t row) const;

	std::vector<Robot>					fRobots;
	Point2D								fSize;
	std::vector<std::vector<size_t>>	fRobotGrid;
};

