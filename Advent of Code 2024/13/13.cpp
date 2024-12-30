#include "clawMachine.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <ranges>
#include <functional>

using	Machines	= std::vector<ClawMachine>;

static std::optional<Point2D> parsePoint(std::string_view line)
{
	const std::regex	pattern	{ R"rgx(.*?(\d+).*?(\d+))rgx" };

	auto				begin	{ line.cbegin() };
	auto				end		{ line.cend() };

	if ( std::match_results<decltype(begin)> match; std::regex_search(begin, end, match, pattern) )
	{
		std::string	arg0	= match[ 1 ].str();
		std::string	arg1	= match[ 2 ].str();

		return Point2D { std::stoul( arg0 ), std::stoul( arg1 ) };
	}

	return std::nullopt;
}

static Machines readMachines(std::string_view fileName)
{
	std::ifstream	input	{ fileName.data() };

	std::string		line1;
	std::string		line2;
	std::string		line3;
	std::string		line4;

	Machines		machines;

	while ( std::getline( input, line1 ).good() && std::getline( input, line2 ).good() && std::getline( input, line3 ).good() )
	{
		auto	optA	{ parsePoint( line1 ) };
		auto	optB	{ parsePoint( line2 ) };
		auto	optT	{ parsePoint( line3 ) };

		if ( optA && optB && optT )
			machines.emplace_back( *optA, *optB, *optT );

		std::getline( input, line4 ); // read empty line after prize
	}

	return machines;
}

void day13(std::string_view fileName)
{
	auto		machines	{ readMachines( fileName ) };

	const auto	tokens		{ std::ranges::fold_left_first( std::views::transform( machines, std::mem_fn( &ClawMachine::calculateLowestCostToReachTarget ) ), std::plus {} ).value() };
	std::cout << "Total tokens to get all possible prizes: " << tokens << '\n';

	std::ranges::for_each( machines, std::mem_fn( &ClawMachine::moveTarget ) );

	const auto	farTokens	{ std::ranges::fold_left_first( std::views::transform( machines, std::mem_fn( &ClawMachine::calculateLowestCostToReachTarget ) ), std::plus {} ).value() };
	std::cout << "Total tokens to get all possible far prizes: " << farTokens << '\n';
}
