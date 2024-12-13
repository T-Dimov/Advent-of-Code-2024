#include "clawMachine.h"

#include <vector>
#include <limits>

static int64_t calculateDeterminant(const Point2D& column0, const Point2D& column1)
{
	return static_cast<int64_t>( column0.fX * column1.fY ) - static_cast<int64_t>( column0.fY * column1.fX );
}

ClawMachine::ClawMachine(const Point2D& a, const Point2D& b, const Point2D& fTarget)
	: fA( a )
	, fB( b )
	, fTarget( fTarget )
{}

size_t ClawMachine::calculateLowestCostToReachTarget() const
{
	// if the determinant is 0 there are 0 or many solutions, resulting from linearly dependent A and B
	// the input didn't have such cases do I didn't need to handle it
	if ( const int64_t determinant { calculateDeterminant( fA, fB ) }; determinant != 0 ) // 1 solution
	{
		const double	coeffA	{ calculateDeterminant( fTarget, fB ) / double( determinant ) };
		const double	coeffB	{ calculateDeterminant( fA, fTarget ) / double( determinant ) };

		if ( coeffA > 0 && coeffB > 0 ) // need only positive coefficients
		{
			const double	floorA	{ std::floor( coeffA ) };
			const double	floorB	{ std::floor( coeffB ) };

			if ( coeffA == floorA && coeffB == floorB ) // need only integer coefficients
				return static_cast<size_t>( 3 * coeffA + coeffB );
		}
	}

	return 0;
}

void ClawMachine::moveTarget()
{
	fTarget.fX	+= 10'000'000'000'000;
	fTarget.fY	+= 10'000'000'000'000;
}
