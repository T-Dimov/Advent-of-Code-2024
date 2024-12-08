#pragma once

#include "point2D.h"

#include <string_view>
#include <vector>
#include <unordered_map>

class Map
{
public:
	explicit	Map(std::string_view fileName);

	size_t		getAntiNodesCount() const;
	size_t		getAntiNodesWithHarmonicsCount() const;

private:
	using Locations	= std::vector<Point2D>;
	using Antenae	= std::unordered_map<char, Locations>;

	void		findAntiNodes(const Point2D& a, const Point2D& b, std::unordered_set<Point2D>& antiNodes) const;
	void		findAntiNodesWithHarmonics(const Point2D& a, const Point2D& b, std::unordered_set<Point2D>& antiNodes) const;

	Antenae	fAntenae;
	Point2D	fMapSize;
};

