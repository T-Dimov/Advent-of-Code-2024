#pragma once

#include "point2D.h"

#include <string_view>

class Robot
{
public:
	explicit	Robot(std::string_view input);

	Point2D		getPosition() const;

	void		move(size_t seconds, const Point2D& spaceSize);

private:
	Point2D	fPosition;
	Point2D	fVelocity;
};

