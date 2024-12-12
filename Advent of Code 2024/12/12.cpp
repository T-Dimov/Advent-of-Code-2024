#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ranges>

using	Map		= std::vector<std::string>;
using	Visited	= std::vector<std::vector<bool>>;
using	Regions	= std::unordered_map<uint16_t, std::vector<uint8_t>>;

static Map readMap(std::string_view fileName)
{
	std::ifstream	input	{ fileName.data() };
	std::string		line;

	Map				map;

	while ( std::getline( input, line ).good() && !line.empty() )
		map.push_back( line );

	return map;
}

static void exploreRegion(size_t i, size_t j, const Map& map, Visited& visited, std::vector<uint8_t>& regionPlotFences, std::vector<uint8_t>& regionSideFences)
{
	visited[ i ][ j ]	= true;

	const char	type		{ map[ i ][ j ] };

	const bool	hasFenceTop	{ i == 0					|| map[ i - 1 ][ j ] != type };
	const bool	hasFenceBot	{ i == map.size() - 1		|| map[ i + 1 ][ j ] != type };
	const bool	hasFenceLft	{ j == 0					|| map[ i ][ j - 1 ] != type };
	const bool	hasFenceRgt	{ j == map[ i ].size() - 1	|| map[ i ][ j + 1 ] != type };

	uint8_t		fenceCount	{ 0 };

	if ( hasFenceTop )
		fenceCount++;
	else if ( !visited[ i - 1 ][ j ] )
		exploreRegion( i - 1, j, map, visited, regionPlotFences, regionSideFences );

	if ( hasFenceBot )
		fenceCount++;
	else if ( !visited[ i + 1 ][ j ] )
		exploreRegion( i + 1, j, map, visited, regionPlotFences, regionSideFences );

	if ( hasFenceLft )
		fenceCount++;
	else if ( !visited[ i ][ j - 1 ] )
		exploreRegion( i, j - 1, map, visited, regionPlotFences, regionSideFences );

	if ( hasFenceRgt )
		fenceCount++;
	else if ( !visited[ i ][ j + 1 ] )
		exploreRegion( i, j + 1, map, visited, regionPlotFences, regionSideFences );

	regionPlotFences.push_back( fenceCount );

	// use the fact that #side = #corners of the shape
	uint8_t		cornerCount	{ 0 };

	// count convex corners
	if ( hasFenceTop && hasFenceLft )
		cornerCount++;
	if ( hasFenceTop && hasFenceRgt )
		cornerCount++;
	if ( hasFenceBot && hasFenceLft )
		cornerCount++;
	if ( hasFenceBot && hasFenceRgt )
		cornerCount++;

	// count concave corners
	if ( !hasFenceTop && !hasFenceLft && map[ i - 1 ][ j - 1 ] != type )
		cornerCount++;
	if ( !hasFenceTop && !hasFenceRgt && map[ i - 1 ][ j + 1 ] != type )
		cornerCount++;
	if ( !hasFenceBot && !hasFenceLft && map[ i + 1 ][ j - 1 ] != type )
		cornerCount++;
	if ( !hasFenceBot && !hasFenceRgt && map[ i + 1 ][ j + 1 ] != type )
		cornerCount++;

	regionSideFences.push_back( cornerCount );
}

static size_t calculateRegionFencingPrice(const std::vector<uint8_t>& region)
{
	return region.size() * std::ranges::fold_left_first( region, std::plus {} ).value();
}

static std::pair<size_t, size_t> calculateFencingPrice(const Map& map)
{
	std::vector<bool>	rowVisited;
	rowVisited.resize( map[ 0 ].size(), false );

	Visited				visited( map.size(), rowVisited );

	static uint16_t		regionID	{ 0 };
	Regions				regionsPlotFences;
	Regions				regionsSideFences;

	for ( size_t i = 0, height = map.size(); i < height; i++ )
	{
		const auto&	row	{ map[ i ] };

		for ( size_t j = 0, width = row.size(); j < width; j++ )
		{
			if ( !visited[ i ][ j ] )
			{
				exploreRegion( i, j, map, visited, regionsPlotFences[ regionID ], regionsSideFences[ regionID ] );
				++regionID;
			}
		}
	}

	return { std::ranges::fold_left_first( regionsPlotFences | std::views::values | std::views::transform( calculateRegionFencingPrice ), std::plus {} ).value(),
			std::ranges::fold_left_first( regionsSideFences | std::views::values | std::views::transform( calculateRegionFencingPrice ), std::plus {} ).value() };
}

int main()
{
	const auto	fileName					{ "input.txt" };

	const auto	map							{ readMap( fileName ) };
	const auto&	[plotFences, sideFences]	{ calculateFencingPrice( map ) };


	std::cout << "Total price of plot fencing: " << plotFences << '\n';
	std::cout << "Total price of side fencing: " << sideFences << '\n';
}
