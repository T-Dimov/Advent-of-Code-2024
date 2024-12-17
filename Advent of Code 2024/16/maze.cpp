#include "maze.h"

#include <fstream>
#include <algorithm>
#include <array>
#include <queue>
#include <unordered_map>

Maze::Maze(std::string_view fileName)
{
	std::ifstream	input	{ fileName.data() };
	std::string		line;

	if ( std::getline( input, line ).good() )
	{
		fSize.fX	= line.length();
		fMap.push_back( line );
	}

	int	i { 1 };
	while ( std::getline( input, line ).good() && !line.empty() )
	{
		if ( auto j = line.find( 'S' ); j != std::string::npos )
			fStart	= { static_cast<int>( j ), i };

		if ( auto j = line.find( 'E' ); j != std::string::npos )
			fEnd	= { static_cast<int>( j ), i };

		fMap.push_back( line );
		++i;
	}

	fSize.fY	= fMap.size();
}

size_t Maze::findLowestCostRoute() const
{
	std::priority_queue<State, std::vector<State>, std::greater<State>>	queue;
	std::unordered_map<Point2D, std::unordered_map<std::byte, int>>		dist;

	queue.emplace( fStart, Direction::R, 0, Maze::heuristic( fStart, fEnd, Direction::R ) );
	dist[ fStart ][ Direction::R ]	= 0;

	while ( !queue.empty() )
	{
		State	current	{ queue.top() };
		queue.pop();

		if ( current.fPos == fEnd )
			return current.fCost;

		const States	neighbours	{ this->getPossibleNeighbours( current ) };

		for ( const auto& [newPos, newDir, addCost, heuristic] : neighbours )
		{
			const int	newCost			{ current.fCost + addCost };
			const int	newHeuristic	{ Maze::heuristic( newPos, fEnd, newDir ) };

			if ( !dist[ newPos ].contains( newDir ) || newCost < dist[ newPos ][ newDir ] )
			{
				dist[ newPos ][ newDir ]	= newCost;
				queue.emplace( newPos, newDir, newCost, newHeuristic );
			}
		}
	}

	return -1; // No path found

}

Maze::States Maze::getPossibleNeighbours(const State& current) const
{
	static const Point2D	dirX		{ 1, 0 };
	static const Point2D	dirY		{ 0, 1 };

	static const auto		canMoveTo	= [] ( char cell ) { return cell == '.' || cell == 'E'; };

	const Point2D&			curr		{ current.fPos };

	const Point2D&			cellU		{ curr - dirY };
	const Point2D&			cellD		{ curr + dirY };
	const Point2D&			cellL		{ curr - dirX };
	const Point2D&			cellR		{ curr + dirX };

	const char				charU		{ fMap[ cellU.fY ][ cellU.fX ] };
	const char				charD		{ fMap[ cellD.fY ][ cellD.fX ] };
	const char				charL		{ fMap[ cellL.fY ][ cellL.fX ] };
	const char				charR		{ fMap[ cellR.fY ][ cellR.fX ] };

	States	possibleMoves;

	if ( current.fDir == Direction::U )
	{
		if ( canMoveTo( charU ) )
			possibleMoves.emplace_back( cellU, Direction::U, 1 );

		if ( canMoveTo( charL ) )
			possibleMoves.emplace_back( cellL, Direction::L, 1001 );

		if ( canMoveTo( charR ) )
			possibleMoves.emplace_back( cellR, Direction::R, 1001 );
	}

	if ( current.fDir == Direction::D )
	{
		if ( canMoveTo( charD ) )
			possibleMoves.emplace_back( cellD, Direction::D, 1 );

		if ( canMoveTo( charR ) )
			possibleMoves.emplace_back( cellR, Direction::R, 1001 );

		if ( canMoveTo( charL ) )
			possibleMoves.emplace_back( cellL, Direction::L, 1001 );
	}

	if ( current.fDir == Direction::L )
	{
		if ( canMoveTo( charL ) )
			possibleMoves.emplace_back( cellL, Direction::L, 1 );

		if ( canMoveTo( charD ) )
			possibleMoves.emplace_back( cellD, Direction::D, 1001 );

		if ( canMoveTo( charU ) )
			possibleMoves.emplace_back( cellU, Direction::U, 1001 );
	}

	if ( current.fDir == Direction::R )
	{
		if ( canMoveTo( charR ) )
			possibleMoves.emplace_back( cellR, Direction::R, 1 );

		if ( canMoveTo( charU ) )
			possibleMoves.emplace_back( cellU, Direction::U, 1001 );

		if ( canMoveTo( charD ) )
			possibleMoves.emplace_back( cellD, Direction::D, 1001 );
	}

	return possibleMoves;
}

int Maze::heuristic(const Point2D& a, const Point2D& b, std::byte dir)
{
	using namespace Direction;

	const int	deltaX				{ std::abs( a.fX - b.fX ) };
	const int	deltaY				{ std::abs( a.fY - b.fY ) };
	const int	manhattanDistance	{ deltaX + deltaY };
	const int	turn				{ deltaX != 0 && deltaY != 0 };

	return manhattanDistance + turn * 1000;
}
