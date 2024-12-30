#include "space.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

Space::Space(std::string_view fileName, const Point2D& size)
	: fSize( size )
	, fRobotGrid( fSize.fY, std::vector<size_t>( fSize.fX ) )
{
	std::ifstream	input	{ fileName.data() };
	std::string		line;

	while ( std::getline( input, line ).good() && !line.empty() )
		fRobots.emplace_back( line );
}

size_t Space::calculateSafetyFactorAfter(size_t seconds)
{
	this->calculateGridAfter( seconds );

	size_t	quad1Num	{ 0 };
	size_t	quad2Num	{ 0 };
	size_t	quad3Num	{ 0 };
	size_t	quad4Num	{ 0 };

	for ( int64_t i = 0, height = fSize.fY / 2; i < height; i++ )
		for ( int64_t j = fSize.fX / 2 + 1; j < fSize.fX; j++ )
			quad1Num	+= fRobotGrid[ i ][ j ];

	for ( int64_t i = 0, height = fSize.fY / 2; i < height; i++ )
		for ( int64_t j = 0, width = fSize.fX / 2; j < width; j++ )
			quad2Num	+= fRobotGrid[ i ][ j ];

	for ( int64_t i = fSize.fY / 2 + 1; i < fSize.fY; i++ )
		for ( int64_t j = 0, width = fSize.fX / 2; j < width; j++ )
			quad3Num	+= fRobotGrid[ i ][ j ];

	for ( int64_t i = fSize.fY / 2 + 1; i < fSize.fY; i++ )
		for ( int64_t j = fSize.fX / 2 + 1; j < fSize.fX; j++ )
			quad4Num	+= fRobotGrid[ i ][ j ];

	return quad1Num * quad2Num * quad3Num * quad4Num;
}

// inspired by reddit, didn't pan out.
size_t Space::findLowestSafetyFactorConfiguration()
{
	const int64_t		iterations		{ fSize.fX * fSize.fY };

	std::vector<size_t>	safetyFactors	( iterations, 0 );

	for ( int64_t i = 0; i < iterations; i++ )
		safetyFactors[ i ]	= this->calculateSafetyFactorAfter( 1 );

	return std::distance( safetyFactors.begin(), std::ranges::min_element( safetyFactors ) ) + 1;
}

size_t Space::findSpaceWithFrameOfRobots()
{
	const int64_t		iterations { fSize.fX * fSize.fY };

	std::vector<size_t>	safetyFactors	( iterations, 0 );

	for ( size_t i = 0; i < iterations; i++ )
	{
		this->calculateGridAfter( 1 );
		if ( size_t start { this->gridHasVerticalLineOfRobots() }; start != -1 && this->gridHasHorizontalLineOfRobotsAtRow( start ) )
		{
			return i + 1;
		}
	}

	return -1;
}

void Space::showAllGrids()
{
	const int64_t	iterations	{ fSize.fX * fSize.fY };

	const int64_t	startIter	{ 7573 };
	this->calculateGridAfter( startIter );

	for ( int64_t i = startIter; i < iterations; i++ )
	{
		this->calculateGridAfter( 1 );
		std::cout << "\n\nGrid after " << i << "\n";
		this->printGrid();
	}
}

void Space::printGrid() const
{
	for ( int64_t i = 0; i < fSize.fY; i++ )
	{
		for ( int64_t j = 0; j < fSize.fX; j++ )
			if ( const size_t& count { fRobotGrid[ i ][ j ] }; count == 0 )
				std::cout << ' ';
			else
				std::cout << count;

		std::cout << '\n';
	}
}

void Space::calculateGridAfter(size_t seconds)
{
	for ( int64_t i = 0; i < fSize.fY; i++ )
		for ( int64_t j = 0; j < fSize.fX; j++ )
			fRobotGrid[ i ][ j ]	= 0;

	for ( Robot& robot : fRobots )
	{
		robot.move( seconds, fSize );
		const auto& pos { robot.getPosition() };
		fRobotGrid[ pos.fY ][ pos.fX ]++;
	}
}

size_t Space::gridHasVerticalLineOfRobots() const
{
	const size_t	threshold	{ 30 };

	for ( size_t j = 0; j < fSize.fX; j++ )
	{
		size_t	count	{ 0 };

		for ( size_t i = 0; i < fSize.fY; i++ )
		{
			if ( fRobotGrid[ i ][ j ] != 0 )
			{
				if ( ++count > threshold )
					return i - threshold;
			}
			else
			{
				count	= 0;
			}
		}
	}

	return -1;
}

bool Space::gridHasHorizontalLineOfRobotsAtRow(size_t row) const
{
	const size_t	threshold { 20 };
	size_t			count { 0 };

	for ( size_t j = 0; j < fSize.fX; j++ )
	{
		if ( fRobotGrid[ row ][ j ] != 0 )
		{
			if ( ++count > threshold )
				return true;
		}
		else
		{
			count	= 0;
		}
	}

	return false;
}
