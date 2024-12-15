#include "point2D.h"

Point2D::Point2D(size_t x, size_t y)
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

Point2D Point2D::operator*(size_t rhs) const
{
	Point2D	temp { *this };
	return temp *= rhs;
}

Point2D& Point2D::operator*=(size_t rhs)
{
	fX	*= rhs;
	fY	*= rhs;

	return *this;
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
	size_t	h1	{ std::hash<size_t> {}( pt.fX ) };
	size_t	h2	{ std::hash<size_t> {}( pt.fY ) };

	return h1 ^ ( h2 << 1 ); // or use boost::hash_combine
}
