#pragma once

#include <string_view>
#include <vector>
#include <unordered_set>

class TopoMap
{
public:
	explicit					TopoMap(std::string_view fileName);

	std::pair<size_t, size_t>	getTrailheadsScoresSumAndRatingsSum() const;

private:
	size_t						searchReachablePeaksFrom(size_t i, size_t j, uint16_t value, std::unordered_set<uint16_t>& peaks) const;

	std::vector<std::vector<uint16_t>>	fMap;
	size_t								fWidth	{ 0 };
	size_t								fHeight	{ 0 };
};

