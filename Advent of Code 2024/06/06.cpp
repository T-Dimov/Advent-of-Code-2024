#include "06.h"

#include "guard.h"

#include <iostream>
#include <fstream>

static Map readMap(std::string_view fileName)
{
	Map				map;

	std::ifstream	input	{ fileName.data() };

	std::string	line;
	while ( std::getline( input, line ).good() && !line.empty() )
	{
		map.push_back( line );
	}

	return map;
}

static size_t countX(const Map& map)
{
	size_t	count	{ 0 };

	for ( const auto& line : map )
		for ( char pos : line )
			if ( pos == 'X' )
				count++;

	return count;
}

static size_t countVisitedPositions(Map map)
{
	Guard	guard	{ map };

	guard.patrol( map );

	return countX( map );
}

// brute force approach
static size_t countWaysOfCreatingLoop(const Map& map)
{
	size_t	count	{ 0 };

	for ( size_t i = 0, length = map.size(); i < length; i++ )
	{
		const auto&	line	{ map[ i ] };

		for ( size_t j = 0, width = line.length(); j < width; j++ )
		{
			if ( map[ i ][ j ] == '.' )
			{
				Map	mapCopy	{ map };
				mapCopy[ i ][ j ]	= '#';

				Guard	guard	{ mapCopy };

				guard.patrol( mapCopy );

				if ( guard.isInLoop() )
					count++;
			}
		}
	}

	return count;
}

void day06(std::string_view fileName)
{
	const Map	map	{ readMap( fileName ) };

	std::cout << "Number of visited positions: "				<< countVisitedPositions( map )		<< '\n';
	std::cout << "Number of ways to put the guard in a loop: "	<< countWaysOfCreatingLoop( map )	<< '\n';
}
