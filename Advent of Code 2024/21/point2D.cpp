#include "point2D.h"

Point2D::Point2D(int x, int y)
	: fX( x )
	, fY( y )
{}

Point2D Point2D::operator+(const Point2D& rhs) const
{
	Point2D	temp	{ *this };
	return temp += rhs;
}

Point2D& Point2D::operator+=(const Point2D& rhs)
{
	fX	+= rhs.fX;
	fY	+= rhs.fY;

	return *this;
}

Point2D Point2D::operator-(const Point2D& rhs) const
{
	Point2D	temp	{ *this };
	return temp -= rhs;
}

Point2D& Point2D::operator-=(const Point2D& rhs)
{
	fX	-= rhs.fX;
	fY	-= rhs.fY;

	return *this;
}

Point2D& Point2D::operator%=(const Point2D& rhs)
{
	fX	%= rhs.fX;
	fY	%= rhs.fY;

	return *this;
}

Point2D Point2D::operator*(int rhs) const
{
	Point2D	temp { *this };
	return temp *= rhs;
}

Point2D& Point2D::operator*=(int rhs)
{
	fX	*= rhs;
	fY	*= rhs;

	return *this;
}

int Point2D::distanceTo(const Point2D& other)
{
	return std::abs( fX - other.fX ) + std::abs( fY - other.fY );
}

bool Point2D::isInRectangle(const Point2D& topLeft, const Point2D& bottomRight) const
{
	return fX >= topLeft.fX
		&& fY >= topLeft.fY
		&& fX <= bottomRight.fX
		&& fY <= bottomRight.fY;
}


size_t std::hash<Point2D>::operator()(const Point2D& pt) const noexcept
{
	size_t	h1	{ std::hash<int> {}( pt.fX ) };
	size_t	h2	{ std::hash<int> {}( pt.fY ) };

	return h1 ^ ( h2 << 1 ); // or use boost::hash_combine
}