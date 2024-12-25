#include "schematic.h"

#include <string>

Schematic::Schematic(std::ifstream& stream)
{
	std::string	line;

	std::getline( stream, line );
	fIsKey	= line[ 0 ] == '.';

	for ( size_t i = 0; i < 5; i++ )
	{
		std::getline( stream, line );

		for ( size_t j = 0; j < 5; j++ )
			if ( line[ j ] == '#' )
				fHeights[ j ]++;
	}

	std::getline( stream, line );
	std::getline( stream, line );
}

bool Schematic::isKey() const
{
	return fIsKey;
}

bool Schematic::doesFit(const Schematic& other) const
{
	for ( size_t i = 0; i < 5; i++ )
		if ( fHeights[ i ] + other.fHeights[ i ] > 5 )
			return false;

	return true;
}
