#pragma once

#include "point2D.h"

class ClawMachine
{
public:
			ClawMachine(const Point2D& a, const Point2D& b, const Point2D& target);

	size_t	calculateLowestCostToReachTarget() const;

	void	moveTarget();

private:
	Point2D	fA;
	Point2D	fB;
	Point2D	fTarget;
};

