#pragma once

#include <unordered_set>
#include <iostream>

struct Point2D
{
							Point2D() = default;
							Point2D(int64_t x, int64_t y);

	auto					operator<=>(const Point2D& rhs) const = default;

	Point2D&				operator+=(const Point2D& rhs);
	friend Point2D			operator+(const Point2D& lhs, const Point2D& rhs)
	{
		Point2D	temp	{ lhs };
		return temp += rhs;
	}

	Point2D&				operator-=(const Point2D& rhs);
	friend Point2D			operator-(const Point2D& lhs, const Point2D& rhs)
	{
		Point2D	temp	{ lhs };
		return temp -= rhs;
	}

	Point2D&				operator%=(const Point2D& rhs);
	friend Point2D			operator%=(const Point2D& lhs, const Point2D& rhs)
	{
		Point2D	temp	{ lhs };
		return temp %= rhs;
	}

	Point2D&				operator*=(int64_t rhs);
	friend Point2D			operator*(const Point2D& lhs, int64_t rhs)
	{
		Point2D	temp	{ lhs };
		return temp *= rhs;
	}

	friend std::ostream&	operator<<(std::ostream& os, const Point2D& pt)
	{
		return os << pt.fX << ',' << pt.fY;
	}

	int64_t					manhattanDistanceTo(const Point2D& other) const;

	bool					isInRectangle(const Point2D& topLeft, const Point2D& bottomRight) const;

	int64_t	fX	{ 0 };
	int64_t	fY	{ 0 };
};

template<>
struct std::hash<Point2D>
{
	size_t operator()(const Point2D& s) const noexcept;
};
