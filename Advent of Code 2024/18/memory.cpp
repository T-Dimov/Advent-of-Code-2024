#include "memory.h"

#include <fstream>
#include <algorithm>
#include <array>
#include <queue>
#include <unordered_map>
#include <sstream>

Memory::Memory(std::string_view fileName, const Point2D& size, size_t byteLimit)
	: fMap( size.fY, std::string( size.fX, '.' ) )
	, fSize( size )
	, fStart( 0, 0 )
	, fEnd( size.fX - 1, size.fY - 1 )
{
	std::ifstream	input	{ fileName.data() };
	std::string		line;

	for ( size_t i = 0; i < byteLimit && std::getline( input, line ).good() && !line.empty(); i++ )
	{
		std::stringstream	lineStream	{ line };

		int	x;
		int	y;
		lineStream >> x;
		lineStream.get();
		lineStream >> y;

		fMap[ y ][ x ]	= '#';
	}

	while ( std::getline( input, line ).good() && !line.empty() )
	{
		std::stringstream	lineStream	{ line };

		int	x;
		int	y;
		lineStream >> x;
		lineStream.get();
		lineStream >> y;

		fRemainingBytes.emplace_back( x, y );
	}

	fMap[ fEnd.fY ][ fEnd.fX ]	= 'E';
}

size_t Memory::findLowestCostRoute() const
{
	std::priority_queue<State, std::vector<State>, std::greater<State>>	queue;
	std::unordered_map<Point2D, int64_t>								dist;

	Visited	startVisited( this->getVisitedVectorSize() );
	startVisited[ this->getVisitedVectorIndex( fStart ) ]	= true;
	queue.emplace( fStart, 0, this->heuristic( fStart ), startVisited );
	dist[ fStart ]	= 0;

	while ( !queue.empty() )
	{
		State	current	{ queue.top() };
		queue.pop();

		if ( current.fPos == fEnd )
		{
			Memory::addNewVisited( fVisitedCells[ current.fCost ], current.fVisitedCells );
			continue;
		}

		const States	neighbours	{ this->getPossibleNeighbours( current ) };

		for ( const auto& [newPos, addCost, heuristic, _] : neighbours )
		{
			const int		newCost			{ current.fCost + addCost };
			const int64_t	newHeuristic	{ this->heuristic( newPos ) };

			if ( !dist.contains( newPos ) || newCost < dist[ newPos ] )
			{
				dist[ newPos ]	= newCost;
				auto	newVisited	{ current.fVisitedCells };
				newVisited[ this->getVisitedVectorIndex( newPos ) ]	= true;
				queue.emplace( newPos, newCost, newHeuristic, newVisited );
			}
		}
	}

	return fVisitedCells.empty() ? -1 : fVisitedCells.begin()->first;
}

Point2D Memory::getFirstByteBlockingTheExit()
{
	//TODO try to optimise with binary search
	for ( const Point2D& byte : fRemainingBytes )
	{
		fMap[ byte.fY ][ byte.fX ]	= '#';
		fVisitedCells.clear();
		if ( const size_t cost { this->findLowestCostRoute() }; cost == std::numeric_limits<size_t>::max() )
			return byte;
	}

	return {};
}

Memory::States Memory::getPossibleNeighbours(const State& current) const
{
	static const Point2D	dirX		{ 1, 0 };
	static const Point2D	dirY		{ 0, 1 };

	static const auto		canMoveTo	= [] ( char cell ) { return cell == '.' || cell == 'E'; };

	const Point2D&			curr		{ current.fPos };

	const Point2D&			cellU		{ curr - dirY };
	const Point2D&			cellD		{ curr + dirY };
	const Point2D&			cellL		{ curr - dirX };
	const Point2D&			cellR		{ curr + dirX };

	const char				charU		{ cellU.fY < 0			? '#' : fMap[ cellU.fY ][ cellU.fX ] };
	const char				charD		{ cellD.fY >= fSize.fY	? '#' : fMap[ cellD.fY ][ cellD.fX ] };
	const char				charL		{ cellL.fX < 0			? '#' : fMap[ cellL.fY ][ cellL.fX ] };
	const char				charR		{ cellD.fX >= fSize.fX	? '#' : fMap[ cellR.fY ][ cellR.fX ] };

	States	possibleMoves;

	if ( canMoveTo( charU ) && !current.fVisitedCells[ this->getVisitedVectorIndex( cellU ) ] )
		possibleMoves.emplace_back( cellU, 1 );

	if ( canMoveTo( charD ) && !current.fVisitedCells[ this->getVisitedVectorIndex( cellD ) ] )
		possibleMoves.emplace_back( cellD, 1 );

	if ( canMoveTo( charL ) && !current.fVisitedCells[ this->getVisitedVectorIndex( cellL ) ] )
		possibleMoves.emplace_back( cellL, 1 );

	if ( canMoveTo( charR ) && !current.fVisitedCells[ this->getVisitedVectorIndex( cellR ) ] )
		possibleMoves.emplace_back( cellR, 1 );

	return possibleMoves;
}

size_t Memory::getVisitedVectorSize() const
{
	return fSize.fX * fSize.fY;
}

size_t Memory::getVisitedVectorIndex(const Point2D& cell) const
{
	return cell.fY * fSize.fX + cell.fX;
}

int64_t Memory::heuristic(const Point2D& cell) const
{
	return cell.manhattanDistanceTo( fEnd );
}

/*static*/ void Memory::addNewVisited(Visited& a, const Visited& b)
{
	const auto	length	{ b.size() };
	a.resize( length );
	for ( size_t i = 0; i < length; i++ )
		if ( b[ i ] )
			a[ i ]	= true;
}
