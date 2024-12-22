#include "keypads.h"
#include "cartesianForEach.h"

#include <algorithm>
#include <ranges>

using Hash	= std::unordered_map<const Coordinates*, std::unordered_map<std::string, Sequences>>;

Point2D& operator+=(Point2D& pt, char dir)
{
	if ( dir == '<' )
		--pt.fX;
	if ( dir == '>' )
		++pt.fX;
	if ( dir == '^' )
		--pt.fY;
	if ( dir == 'v' )
		++pt.fY;

	return pt;
}

/*static*/ const std::array<std::array<char, 3>, 4>	NumericKeypad::sLayout { {	{ '7', '8', '9' },
																				{ '4', '5', '6' },
																				{ '1', '2', '3' },
																				{ ' ', '0', 'A' } } };

/*static*/ const std::unordered_map<char, Point2D>	NumericKeypad::sCoordinates {	{ 'A', { 2, 3 } },
																					{ ' ', { 0, 3 } },
																					{ '0', { 1, 3 } },
																					{ '1', { 0, 2 } },
																					{ '2', { 1, 2 } },
																					{ '3', { 2, 2 } },
																					{ '4', { 0, 1 } },
																					{ '5', { 1, 1 } },
																					{ '6', { 2, 1 } },
																					{ '7', { 0, 0 } },
																					{ '8', { 1, 0 } },
																					{ '9', { 2, 0 } } };

/*static*/ const std::array<std::array<char, 3>, 2>	DirectionalKeypad::sLayout { {	{ ' ', '^', 'A' },
																					{ '<', 'v', '>' } } };

/*static*/ const std::unordered_map<char, Point2D>	DirectionalKeypad::sCoordinates {	{ 'A', { 2, 0 } },
																						{ ' ', { 0, 0 } },
																						{ '^', { 1, 0 } },
																						{ '<', { 0, 1 } },
																						{ 'v', { 1, 1 } },
																						{ '>', { 2, 1 } }, };

Sequences Keypad::getSequences(const Coordinates& coordinates, const std::string& doorCode)
{
	static Hash	hash;

	auto&		map		{ hash[ &coordinates ] };

	if ( map.contains( doorCode ) )
		return map[ doorCode ];

	std::vector<Sequences>	sequences;

	char	from	{ 'A' };
	for ( char to : doorCode )
	{
		sequences.push_back( Keypad::getSequences( coordinates, from, to ) );
		from	= to;
	}

	Sequences	result;

	cartesian_foreach( sequences, [ & ] ( const std::vector<std::string>& vec )
		{
			result.insert( std::ranges::fold_left_first( vec, std::plus {} ).value() );
		} );

	return map[ doorCode ]	= result;
}

Sequences Keypad::getSequences(const Coordinates& coordinates, char from, char to)
{
	static Hash	hash;

	auto&		map		{ hash[ &coordinates ] };
	auto		str		{ std::string { from, to } };

	if ( map.contains( str ) )
		return map[ str ];

	const Point2D	delta		{ coordinates.at( to ) - coordinates.at( from ) };
	auto			directions	{ Keypad::getSequence( delta ) };

	Sequences	sequences;

	do
	{
		if ( Keypad::isSequenceSafe( coordinates, from, directions ) )
			sequences.insert( directions + 'A' );
	}
	while ( std::ranges::next_permutation( directions ).found );

	return map[ str ]	= sequences;
}

std::string Keypad::getSequence(Point2D delta)
{
	std::string	directions;

	while ( delta.fX < 0 )
	{
		directions.push_back( '<' );
		++delta.fX;
	}
	while ( delta.fX > 0 )
	{
		directions.push_back( '>' );
		--delta.fX;
	}
	while ( delta.fY < 0 )
	{
		directions.push_back( '^' );
		++delta.fY;
	}
	while ( delta.fY > 0 )
	{
		directions.push_back( 'v' );
		--delta.fY;
	}

	return directions;
}

bool Keypad::isSequenceSafe(const Coordinates& coordinates, char from, std::string_view sequence)
{
	const Point2D&	empty	{ coordinates.at( ' ' ) };

	Point2D			curr	{ coordinates.at( from ) };

	for ( char dir : sequence )
	{
		curr	+= dir;
		if ( curr == empty )
			return false;
	}

	return true;
}

Sequences NumericKeypad::getSequences(const std::string& doorCode)
{
	return Keypad::getSequences( NumericKeypad::sCoordinates, doorCode );
}

Sequences DirectionalKeypad::getSequences(const std::string& keypadSequence)
{
	return Keypad::getSequences( DirectionalKeypad::sCoordinates, keypadSequence );
}
