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

	int64_t	i	{ 1 };
	while ( std::getline( input, line ).good() && !line.empty() )
	{
		if ( auto j = line.find( '@' ); j != std::string::npos )
			fRobotPosition	= { static_cast<int64_t>( j ), i };

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

		fSize.fX			*= 2;
		fRobotPosition.fX	*= 2;
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

	std::cout << '\n';
}

void Map::moveRobot(bool printEachStep)
{
	this->print();

	size_t	iter	{ 0 };

	for ( auto instr : fInstructions )
	{
		++iter;

		if ( printEachStep )
			std::cout << std::format( "{} ({})\n", instr, iter );

		this->moveOnce( instr );

		if ( printEachStep )
			this->print();

		if ( this->hasBrokenBox() )
		{
			std::cout << std::format( "{} ({}) broke a box!\n", instr, iter );
			if ( !printEachStep )
				this->print();
		}
	}

	if ( !printEachStep )
		this->print();
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

/*static*/ bool Map::isBox(char cell, bool allowRightHalf /*= true*/)
{
	return cell == 'O' || cell == '[' || allowRightHalf && cell == ']';
}

bool Map::hasBrokenBox() const
{
	for ( size_t i = 0; i < fSize.fY; i++ )
	{
		const auto& line { fMap[ i ] };

		for ( size_t j = 0; j < fSize.fX; j++ )
			if ( line[ j ] == '[' && line[ j + 1 ] != ']' )
				return true;
	}

	return false;
}

void Map::readInstructions(std::string_view line)
{
	fInstructions.insert_range( fInstructions.end(), line );
}

void Map::moveOnce(char dir)
{
	switch ( dir )
	{
		case '^':
		{
			this->moveU();
			break;
		}
		case 'v':
		{
			this->moveD();
			break;
		}
		case '<':
		{
			this->moveL();
			break;
		}
		case '>':
		{
			this->moveR();
			break;
		}
	}
}

void Map::moveU()
{
	int64_t	i	{ fRobotPosition.fY - 1 };

	if ( const char& cell { fMap[ i ][ fRobotPosition.fX ] }; cell == '.' )
	{
		std::swap( fMap[ fRobotPosition.fY ][ fRobotPosition.fX ], fMap[ i ][ fRobotPosition.fX ] );
		fRobotPosition.fY	= i;
		return;
	}
	else if ( cell == '#' )
		return;

	// cell is a box
	if ( fIsWide )
		this->searchAndPushWide( i, -1 );
	else
	{
		i	= this->searchU( i );

		if ( fMap[ i ][ fRobotPosition.fX ] == '#' )
			return;

		// cell is empty, push the boxes
		this->pushU( i );
	}
}

void Map::moveD()
{
	int64_t	i	{ fRobotPosition.fY + 1 };

	if ( const char& cell { fMap[ i ][ fRobotPosition.fX ] }; cell == '.' )
	{
		std::swap( fMap[ fRobotPosition.fY ][ fRobotPosition.fX ], fMap[ i ][ fRobotPosition.fX ] );
		fRobotPosition.fY	= i;
		return;
	}
	else if ( cell == '#' )
		return;

	// cell is a box
	if ( fIsWide )
		this->searchAndPushWide( i, 1 );
	else
	{
		i	= this->searchD( i );

		if ( fMap[ i ][ fRobotPosition.fX ] == '#' )
			return;

		// cell is empty, push the boxes
		this->pushD( i );
	}
}

void Map::moveL()
{
	int64_t	j	{ fRobotPosition.fX - 1 };

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
	int64_t	j	{ fRobotPosition.fX + 1 };

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

void Map::searchAndPushWide(size_t i, int dir)
{
	WideBox	box	{ i, static_cast<size_t>( fRobotPosition.fX ), *this };
	box.propagate( dir );

	if ( box.canMove( dir ) )
	{
		box.move( dir, fMap );

		std::swap( fMap[ fRobotPosition.fY + dir ][ fRobotPosition.fX ], fMap[ fRobotPosition.fY ][ fRobotPosition.fX ] );
		fRobotPosition.fY	+= dir;
	}
}


Map::WideBox::WideBox(size_t row, size_t col, const Map& map)
	: fRow( row )
	, fMap( map )
{
	if ( char cell { fMap.fMap[ fRow ][ col ] }; cell == '[' )
	{
		fLeft	= col;
		fRight	= col + 1;
	}
	else
	{
		fLeft	= col - 1;
		fRight	= col;
	}
}

void Map::WideBox::propagate(int dir)
{
	const size_t	nextRow	{ fRow + dir };
	const auto&		row		{ fMap.fMap[ nextRow ] };

	if ( Map::isBox( row[ fLeft ] ) )
	{
		fLeftBox	= std::make_unique<WideBox>( nextRow, fLeft, fMap );
		fLeftBox->propagate( dir );
	}

	if ( char cell { row[ fRight ] }; cell == '[' )
	{
		fRightBox	= std::make_unique<WideBox>( nextRow, fRight, fMap );
		fRightBox->propagate( dir );
	}
	else if ( cell == ']' ) // left and right shouldn't be the same child
		fRightIsLeft	= true;

	// left and right shouldn't have the same child
	if ( fLeftBox && fRightBox && fLeftBox->fRightBox && fRightBox->fLeftBox )
	{
		fRightBox->fLeftBox.reset();
		fRightBox->fLeftIsFree	= true;
	}
}

bool Map::WideBox::canMove(int dir)
{
	const bool	canMoveLeft		{ fLeftBox ? fLeftBox->canMove( dir ) : fLeftIsFree || fMap.fMap[ fRow + dir ][ fLeft ] == '.' };
	const bool	canMoveRight	{ fRightBox ? fRightBox->canMove( dir ) : fRightIsLeft || fMap.fMap[ fRow + dir ][ fRight ] == '.' };

	return canMoveLeft && canMoveRight;
}

void Map::WideBox::move(int dir, std::vector<std::string>& map)
{
	if ( fLeftBox )		fLeftBox->move( dir, map );
	if ( fRightBox )	fRightBox->move( dir, map );

	std::swap( map[ fRow + dir ][ fLeft ]	, map[ fRow ][ fLeft ]	);
	std::swap( map[ fRow + dir ][ fRight ]	, map[ fRow ][ fRight ]	);

	fRow	+= dir;
}
