#pragma once

#include "point2D.h"

#include <string_view>
#include <vector>
#include <string>
#include <unordered_set>
#include <map>

class Memory
{
public:
					Memory(std::string_view fileName, const Point2D& size, size_t byteLimit);

	size_t			findLowestCostRoute() const;
	Point2D			getFirstByteBlockingTheExit();

private:
	using	Visited			= std::vector<bool>;
	using	VisitedByCost	= std::map<size_t, Visited>;

	struct State
	{
		Point2D		fPos;
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

	int				heuristic(const Point2D& cell) const;

	static void		addNewVisited(Visited& a, const Visited& b);

	std::vector<std::string>	fMap;
	std::vector<Point2D>		fRemainingBytes;
	Point2D						fSize;
	Point2D						fStart;
	Point2D						fEnd;
	mutable VisitedByCost		fVisitedCells;
};
