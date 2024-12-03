#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <functional>

int getListsDistance(std::string_view fileName)
{
	std::ifstream		input { fileName.data() };

	std::vector<int>	left;
	std::vector<int>	right;

	int id;
	while ( input >> id )
	{
		left.push_back( id );

		input >> id;
		right.push_back( id );
	}

	std::ranges::sort( left );
	std::ranges::sort( right );

	const auto	diff	= [] ( int l, int r )
		{
			return std::abs( l - r );
		};

	return std::ranges::fold_left_first( std::views::zip_transform( diff, left, right ), std::plus {} ).value();
}

int getListsSimilarityScore(std::string_view fileName)
{
	std::ifstream					input { fileName.data() };

	std::vector<int>				left;
	std::unordered_map<int, int>	right;

	int id;
	while ( input >> id )
	{
		left.push_back( id );

		input >> id;
		right[ id ]++;
	}

	const auto	similarity	= [ &right ] ( int id )
		{
			return id * right[ id ];
		};

	return std::ranges::fold_left_first( std::views::transform( left, similarity ), std::plus {} ).value();
}

int main()
{
	std::cout << "Distance between lists: " << getListsDistance( "input.txt" ) << std::endl;
	std::cout << "Lists similarity score: " << getListsSimilarityScore( "input.txt" ) << std::endl;
}
