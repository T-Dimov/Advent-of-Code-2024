#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <numeric>

struct Report
{
	std::vector<int>	fLevels;

	explicit Report(std::istream& is)
	{
		while ( is.good() )
		{
			int level;
			is >> level;
			fLevels.push_back( level );
		}
	}

	Report(const std::vector<int>& levels, size_t exclude)
		: fLevels( levels )
	{
		fLevels.erase( fLevels.begin() + exclude );
	}

	bool isSafe() const
	{
		const size_t	length		{ fLevels.size() };

		if ( length <= 1 )
			return true;

		const int		firstDiff	{ fLevels[ 0 ] - fLevels[ 1 ] };
		const int		firstSign	{ Report::sgn( firstDiff ) };

		if ( !Report::isSafeDifference( firstDiff ) )
			return false;

		for ( size_t i = 2; i < length; i++ )
			if ( const int diff = fLevels[ i - 1 ] - fLevels[ i ]; !Report::isSafeDifference( diff ) || firstSign != Report::sgn( diff ) )
				return false;

		return true;
	}

	bool isSafeWithDampener() const
	{
		if ( this->isSafe() )
			return true;

		const size_t		length	{ fLevels.size() };

		std::vector<Report>	modified;
		modified.reserve( length );

		for ( size_t i = 0; i < length; i++ )
			modified.emplace_back( fLevels, i );

		return std::ranges::any_of( modified, &Report::isSafe );
	}

private:
	static bool isSafeDifference(int difference)
	{
		const int	diff	{ std::abs( difference ) };
		return diff >= 1 && diff <= 3;
	}

	static int sgn( int val )
	{
		return ( 0 < val ) - ( val < 0 );
	}
};

void day02(std::string_view fileName)
{
	std::ifstream		input	{ fileName.data() };

	std::vector<Report>	reports;

	std::string			line;
	while ( std::getline( input, line ).good() )
	{
		std::stringstream	lineStream { line };
		reports.emplace_back( lineStream );
	}

	std::cout << "Safe: " << std::ranges::count_if( reports, &Report::isSafe ) << '\n';
	std::cout << "Safe with dampener: " << std::ranges::count_if( reports, &Report::isSafeWithDampener ) << '\n';
}
