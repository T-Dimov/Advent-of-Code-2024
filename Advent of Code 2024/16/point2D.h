#pragma once

#include <unordered_set>

struct Point2D
{
				Point2D() =default;
				Point2D(int x, int y);

	auto		operator<=>(const Point2D& rhs) const = default;

	Point2D		operator+(const Point2D& rhs) const;
	Point2D&	operator+=(const Point2D& rhs);
	Point2D		operator-(const Point2D& rhs) const;
	Point2D&	operator-=(const Point2D& rhs);

	Point2D&	operator%=(const Point2D& rhs);

	Point2D		operator*(int rhs) const;
	Point2D&	operator*=(int rhs);

	bool		isInRectangle(const Point2D& topLeft, const Point2D& bottomRight) const;

	int	fX	{ 0 };
	int	fY	{ 0 };
};

template<>
struct std::hash<Point2D>
{
	size_t operator()(const Point2D& s) const noexcept;
};
