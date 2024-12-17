#pragma once

#include "point2D.h"

#include <string_view>
#include <vector>
#include <string>
#include <unordered_set>
#include <map>

namespace Direction
{
	constexpr const std::byte	None	{ 0b0000'0000 };

	constexpr const std::byte	U		{ 0b0000'0001 };
	constexpr const std::byte	R		{ 0b0000'0010 };
	constexpr const std::byte	D		{ 0b0000'0100 };
	constexpr const std::byte	L		{ 0b0000'1000 };

	inline bool operator%(std::byte lhs, std::byte rhs)
	{
		return ( lhs & rhs ) == rhs;
	}
};

class Maze
{
public:
	explicit		Maze(std::string_view fileName);

	size_t			findLowestCostRoute() const;
	size_t			getVisitedCellsCount() const;

private:
	using	Visited			= std::vector<bool>;
	using	VisitedByCost	= std::map<size_t, Visited>;

	struct State
	{
		Point2D		fPos;
		std::byte	fDir;
		int			fCost;
		int			fHeuristic;
		Visited		fVisitedCells;

		auto operator<=>(const State& rhs) const
		{
			return fCost + fHeuristic <=> rhs.fCost + rhs.fHeuristic;
		}
	};

	using	States	= std::vector<State>;

	States			getPossibleNeighbours(const State& current) const;
	size_t			getVisitedVectorSize() const;
	size_t			getVisitedVectorIndex(const Point2D& cell) const;

	int				heuristic(const Point2D& cell, std::byte dir) const;

	static void		addNewVisited(Visited& a, const Visited& b);

	std::vector<std::string>	fMap;
	Point2D						fSize;
	Point2D						fStart;
	Point2D						fEnd;
	mutable VisitedByCost		fVisitedCells;
};
