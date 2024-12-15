#include "map.h"

#include <fstream>
#include <iostream>

Map::Map(std::string_view fileName, bool wide)
	: fIsWide( wide )
{
	std::ifstream	input	{ fileName.data() };
	std::string		line;

	if ( std::getline( input, line ).good() )
	{
		fSize.fX	= line.length();
		fMap.push_back( line );
	}

	size_t	i	{ 1 };
	while ( std::getline( input, line ).good() && !line.empty() )
	{
		if ( auto j = line.find( '@' ); j != std::string::npos )
			fRobotPosition	= { j, i };

		fMap.push_back( line );
		++i;
	}

	fSize.fY	= fMap.size();

	while ( std::getline( input, line ).good() && !line.empty() )
		this->readInstructions( line );

	if ( fIsWide )
	{
		for ( i = 0; i < fSize.fY; i++ )
		{
			auto&	row	{ fMap[ i ] };

			for ( int j = static_cast<int>( row.length() ) - 1; j >= 0; j-- )
			{
				if ( row[ j ] == '#' )		row.replace( j, 1, "##" );
				else if ( row[ j ] == 'O' )	row.replace( j, 1, "[]" );
				else if ( row[ j ] == '.' )	row.replace( j, 1, ".." );
				else if ( row[ j ] == '@' )	row.replace( j, 1, "@." );
			}
		}

		fSize.fX	*= 2;
	}
}

void Map::print() const
{
	for ( size_t i = 0; i < fSize.fY; i++ )
	{
		const auto&	line	{ fMap[ i ] };

		for ( size_t j = 0; j < fSize.fX; j++ )
			std::cout << line[ j ];

		std::cout << '\n';
	}
}

void Map::moveRobot()
{
	for ( auto instr : fInstructions )
		this->moveOnce( instr );
}

size_t Map::calculateSumOfBoxCoordinates() const
{
	size_t	sum	{ 0 };

	for ( size_t i = 0; i < fSize.fY; i++ )
	{
		const auto&	line	{ fMap[ i ] };

		for ( size_t j = 0; j < fSize.fX; j++ )
			if ( Map::isBox( line[ j ], false ) )
				sum	+= 100 * i + j;
	}

	return sum;
}

void Map::readInstructions(std::string_view line)
{
	using namespace Direction;

	for ( char instr : line )
	{
		if ( instr == '^' )
			fInstructions.push_back( U );

		if ( instr == 'v' )
			fInstructions.push_back( D );

		if ( instr == '<' )
			fInstructions.push_back( L );

		if ( instr == '>' )
			fInstructions.push_back( R );
	}
}

/*static*/ bool Map::isBox(char cell, bool allowRightHalf /*= true*/)
{
	return cell == 'O' || cell == '[' || allowRightHalf && cell == ']';
}

void Map::moveOnce(std::byte dir)
{
	using namespace Direction;

	switch ( dir )
	{
		case U:
		{
			this->moveU();
			break;
		}
		case D:
		{
			this->moveD();
			break;
		}
		case L:
		{
			this->moveL();
			break;
		}
		case R:
		{
			this->moveR();
			break;
		}
	}
}

void Map::moveU()
{
	size_t	i		{ fRobotPosition.fY - 1 };

	if ( const char& cell { fMap[ i ][ fRobotPosition.fX ] }; cell == '.' )
	{
		std::swap( fMap[ fRobotPosition.fY ][ fRobotPosition.fX ], fMap[ i ][ fRobotPosition.fX ] );
		fRobotPosition.fY	= i;
		return;
	}
	else if ( cell == '#' )
		return;

	// cell is a box
	i	= this->searchU( i );

	if ( fMap[ i ][ fRobotPosition.fX ] == '#' )
		return;

	// cell is empty, push the boxes
	this->pushU( i );
}

void Map::moveD()
{
	size_t	i		{ fRobotPosition.fY + 1 };

	if ( const char& cell { fMap[ i ][ fRobotPosition.fX ] }; cell == '.' )
	{
		std::swap( fMap[ fRobotPosition.fY ][ fRobotPosition.fX ], fMap[ i ][ fRobotPosition.fX ] );
		fRobotPosition.fY	= i;
		return;
	}
	else if ( cell == '#' )
		return;

	// cell is a box
	i	= this->searchD( i );

	if ( fMap[ i ][ fRobotPosition.fX ] == '#' )
		return;

	// cell is empty, push the boxes
	this->pushD( i );
}

void Map::moveL()
{
	size_t	j		{ fRobotPosition.fX - 1 };

	if ( const char& cell { fMap[ fRobotPosition.fY ][ j ] }; cell == '.' )
	{
		std::swap( fMap[ fRobotPosition.fY ][ fRobotPosition.fX ], fMap[ fRobotPosition.fY ][ j ] );
		fRobotPosition.fX	= j;
		return;
	}
	else if ( cell == '#' )
		return;

	// cell is a box
	j	= this->searchL( j );

	if ( fMap[ fRobotPosition.fY ][ j ] == '#' )
		return;

	// cell is empty, push the boxes
	this->pushL( j );
}

void Map::moveR()
{
	size_t	j		{ fRobotPosition.fX + 1 };

	if ( const char& cell { fMap[ fRobotPosition.fY ][ j ] }; cell == '.' )
	{
		std::swap( fMap[ fRobotPosition.fY ][ fRobotPosition.fX ], fMap[ fRobotPosition.fY ][ j ] );
		fRobotPosition.fX	= j;
		return;
	}
	else if ( cell == '#' )
		return;

	// cell is a box
	j	= this->searchR( j );

	if ( fMap[ fRobotPosition.fY ][ j ] == '#' )
		return;

	// cell is empty, push the boxes
	this->pushR( j );
}

size_t Map::searchU(size_t i)
{
	char	cell;

	do
	{
		cell	= fMap[ --i ][ fRobotPosition.fX ];
	}
	while ( Map::isBox( cell ) );

	return i;
}

size_t Map::searchD(size_t i)
{
	char	cell;

	do
	{
		cell	= fMap[ ++i ][ fRobotPosition.fX ];
	}
	while ( Map::isBox( cell ) );

	return i;
}

size_t Map::searchL(size_t j)
{
	char	cell;

	do
	{
		cell	= fMap[ fRobotPosition.fY ][ --j ];
	}
	while ( Map::isBox( cell ) );

	return j;
}

size_t Map::searchR(size_t j)
{
	char	cell;

	do
	{
		cell	= fMap[ fRobotPosition.fY ][ ++j ];
	}
	while ( Map::isBox( cell ) );

	return j;
}

void Map::pushU(size_t i)
{
	while ( i < fRobotPosition.fY )
	{
		fMap[ i ][ fRobotPosition.fX ]	= fMap[ i + 1 ][ fRobotPosition.fX ];
		++i;
	}
	fMap[ i ][ fRobotPosition.fX ]	= '.';
	--fRobotPosition.fY;
}

void Map::pushD(size_t i)
{
	while ( i > fRobotPosition.fY )
	{
		fMap[ i ][ fRobotPosition.fX ]	= fMap[ i - 1 ][ fRobotPosition.fX ];
		--i;
	}
	fMap[ i ][ fRobotPosition.fX ]	= '.';
	++fRobotPosition.fY;
}

void Map::pushL(size_t j)
{
	while ( j < fRobotPosition.fX )
	{
		fMap[ fRobotPosition.fY ][ j ]	= fMap[ fRobotPosition.fY ][ j + 1 ];
		++j;
	}
	fMap[ fRobotPosition.fY ][ j ]	= '.';
	--fRobotPosition.fX;
}

void Map::pushR(size_t j)
{
	while ( j > fRobotPosition.fX )
	{
		fMap[ fRobotPosition.fY ][ j ]	= fMap[ fRobotPosition.fY ][ j - 1 ];
		--j;
	}
	fMap[ fRobotPosition.fY ][ j ]	= '.';
	++fRobotPosition.fX;
}
