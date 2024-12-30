#include "guard.h"

#include <algorithm>
#include <iostream>

Guard::Guard(const Map& map)
{
	this->findGuard( map );
}

void Guard::patrol(Map& map)
{
	do
	{
		this->moveUntilObstacle( map );
	}
	while ( !this->hasLeft() && !isInLoop() );
}

bool Guard::hasLeft() const
{
	return std::cmp_equal( fX, -1 ) || std::cmp_equal( fY, -1 );
}

bool Guard::isInLoop() const
{
	return fIsInLoop;
}

void Guard::moveUntilObstacle(Map& map)
{
	const size_t	lastD		{ map.size() - 1 };
	const size_t	lastR		{ map.front().length() - 1 };

	switch ( fDir )
	{
		using enum Direction;

		case U:
		{
			for ( ; fY >= 0; fY-- )
			{
				map[ fY ][ fX ]	= 'X';

				if ( fY > 0 && map[ fY - 1 ][ fX ] == '#' )
				{
					this->turn();
					break;
				}
			}

			break;
		}

		case R:
		{
			for ( ; fX <= lastR; fX++ )
			{
				map[ fY ][ fX ]	= 'X';

				if ( fX < lastR && map[ fY ][ fX + 1 ] == '#' )
				{
					this->turn();
					break;
				}
			}

			if ( fX == lastR + 1 )
				fX	= -1;

			break;
		}

		case D:
		{
			for ( ; fY <= lastD; fY++ )
			{
				map[ fY ][ fX ]	= 'X';

				if ( fY < lastD && map[ fY + 1 ][ fX ] == '#' )
				{
					this->turn();
					break;
				}
			}

			if ( fY == lastD + 1 )
				fY	= -1;

			break;
		}

		case L:
		{
			for ( ; fX >= 0; fX-- )
			{
				map[ fY ][ fX ]	= 'X';

				if ( fX > 0 && map[ fY ][ fX - 1 ] == '#' )
				{
					this->turn();
					break;
				}
			}

			break;
		}

		default:
			break;
	}
}

void Guard::turn()
{
	using enum Direction;

	if ( fDir == L )
		fDir	= U;
	else
		fDir	<<= 1;

	Turn	t	{ fY, fX, fDir };

	if ( auto iter = std::ranges::find( fTurns, t ); iter != fTurns.end() )
		fIsInLoop	= true;

	fTurns.push_back( t );
}

/*static*/ Direction Guard::getDirection(char c)
{
	using enum Direction;

	if ( c == '^' )
		return U;

	if ( c == 'v' )
		return D;

	if ( c == '<' )
		return L;

	if ( c == '>' )
		return R;

	return None;
}

void Guard::findGuard(const Map& map)
{
	for ( size_t i = 0, length = map.size(); i < length; i++ )
	{
		const auto&	line	{ map[ i ] };

		for ( size_t j = 0, width = line.length(); j < width; j++ )
			if ( Direction dir = getDirection( line[ j ] ); dir != Direction::None )
			{
				fX		= static_cast<int>( j );
				fY		= static_cast<int>( i );
				fDir	= dir;
				return;
			}
	}
}
