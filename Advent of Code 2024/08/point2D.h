#pragma once

#include <unordered_set>

struct Point2D
{
				Point2D() =default;
				Point2D(size_t x, size_t y);

	auto		operator<=>(const Point2D& rhs) const = default;

	Point2D		operator+(const Point2D& rhs) const;
	Point2D&	operator+=(const Point2D& rhs);
	Point2D		operator-(const Point2D& rhs) const;
	Point2D&	operator-=(const Point2D& rhs);

	bool		isInRectangle(const Point2D& topLeft, const Point2D& bottomRight) const;

	size_t	fX	{ 0 };
	size_t	fY	{ 0 };
};

template<>
struct std::hash<Point2D>
{
	size_t operator()(const Point2D& s) const noexcept;
};
