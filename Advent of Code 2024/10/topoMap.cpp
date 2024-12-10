#include "topoMap.h"

#include <fstream>
#include <string>

TopoMap::TopoMap(std::string_view fileName)
{
	std::ifstream	input { fileName.data() };
	std::string		line;

	while ( std::getline( input, line ).good() && !line.empty() )
	{
		fWidth	= line.size();

		std::vector<uint16_t>	row;
		row.reserve( fWidth );

		for ( size_t i = 0; i < fWidth; i++ )
			row.push_back( line[ i ] - '0' );

		fMap.push_back( row );
	}

	fHeight	= fMap.size();
}

std::pair<size_t, size_t> TopoMap::getTrailheadsScoresSumAndRatingsSum() const
{
	size_t	scoresSum	{ 0 };
	size_t	ratingsSum	{ 0 };

	for ( size_t i = 0; i < fHeight; i++ )
		for ( size_t j = 0; j < fWidth; j++ )
			if ( fMap[ i ][ j ] == 0 )
			{
				std::unordered_set<uint16_t>	peaks;
				ratingsSum	+= this->searchReachablePeaksFrom( i, j, 0, peaks );
				scoresSum	+= peaks.size();
			}

	return { scoresSum, ratingsSum };
}

size_t TopoMap::searchReachablePeaksFrom(size_t i, size_t j, uint16_t value, std::unordered_set<uint16_t>& peaks) const
{
	if ( value == 9 )
	{
		peaks.insert( i * fWidth + j );
		return 1;
	}

	size_t	reachableCount	{ 0 };

	if ( i >= 1 && fMap[ i - 1 ][ j ] == value + 1 )
		reachableCount	+= this->searchReachablePeaksFrom( i - 1, j, value + 1, peaks );

	if ( i < fHeight - 1 && fMap[ i + 1 ][ j ] == value + 1 )
		reachableCount	+= this->searchReachablePeaksFrom( i + 1, j, value + 1, peaks );

	if ( j >= 1 && fMap[ i ][ j - 1 ] == value + 1 )
		reachableCount	+= this->searchReachablePeaksFrom( i, j - 1, value + 1, peaks );

	if ( j < fWidth - 1 && fMap[ i ][ j + 1 ] == value + 1 )
		reachableCount	+= this->searchReachablePeaksFrom( i, j + 1, value + 1, peaks );

	return reachableCount;
}
