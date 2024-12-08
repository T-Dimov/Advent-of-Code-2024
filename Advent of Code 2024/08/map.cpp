#include "map.h"

#include <fstream>
#include <string>

Map::Map(std::string_view fileName)
{
	std::ifstream	input	{ fileName.data() };
	std::string		line;
	size_t			i		{ 0 };

	while ( std::getline( input, line ).good() && !line.empty() )
	{
		fMapSize.fX	= line.size();

		for ( size_t j = 0; j < fMapSize.fX; j++ )
			if ( line[ j ] != '.' )
				fAntenae[ line[ j ] ].emplace_back( j, i );

		i++;
	}

	fMapSize.fY	= i;
}

size_t Map::getAntiNodesCount() const
{
	std::unordered_set<Point2D>	antiNodes;

	for ( const auto& [_, locations] : fAntenae )
		for ( size_t i = 0, length = locations.size(); i < length - 1; i++ )
			for ( size_t j = i + 1; j < length; j++ )
				this->findAntiNodes( locations[ i ], locations[ j ], antiNodes );

	return antiNodes.size();
}

size_t Map::getAntiNodesWithHarmonicsCount() const
{
	std::unordered_set<Point2D>	antiNodes;

	for ( const auto& [_, locations] : fAntenae )
		for ( size_t i = 0, length = locations.size(); i < length - 1; i++ )
			for ( size_t j = i + 1; j < length; j++ )
				this->findAntiNodesWithHarmonics( locations[ i ], locations[ j ], antiNodes );

	return antiNodes.size();
}

void Map::findAntiNodes(const Point2D& a, const Point2D& b, std::unordered_set<Point2D>& antiNodes) const
{
	const auto	max		{ fMapSize - Point2D { 1, 1 } };
	const auto	dir		{ b - a };

	const auto	aSide	{ a - dir };
	const auto	bSide	{ b + dir };

	if ( aSide.isInRectangle( {}, max ) )
		antiNodes.insert( aSide );

	if ( bSide.isInRectangle( {}, max ) )
		antiNodes.insert( bSide );
}

void Map::findAntiNodesWithHarmonics(const Point2D& a, const Point2D& b, std::unordered_set<Point2D>& antiNodes) const
{
	const auto	max		{ fMapSize - Point2D { 1, 1 } };
	const auto	dir		{ b - a };

	auto		aSide	{ a - dir };
	auto		bSide	{ b + dir };

	antiNodes.insert( a );
	antiNodes.insert( b );

	while ( aSide.isInRectangle( {}, max ) )
	{
		antiNodes.insert( aSide );

		aSide	-= dir;
	}

	while ( bSide.isInRectangle( {}, max ) )
	{
		antiNodes.insert( bSide );

		bSide	+= dir;
	}
}
