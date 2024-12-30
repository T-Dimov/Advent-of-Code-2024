#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <ranges>
#include <algorithm>
#include <optional>

using Update	= std::vector<uint16_t>;

class Ordering
{
public:
	void	addRule(uint16_t first, uint16_t second)
	{
		fRules[ first ].insert( second );
	}

	bool	areAllAfterGiven(uint16_t page, Update::const_iterator begin, Update::const_iterator end) const
	{
		if ( begin == end )
			return true;

		if ( !fRules.contains( page ) )
			return false;

		const auto&	pageRules	{ fRules.at( page ) };

		return std::ranges::all_of( begin, end, [ & ] ( uint16_t page ) { return pageRules.contains( page ); } );
	}

	Update	getReorderedUpdate(const Update& update) const
	{
		Update	result;

		Update	tail	{ update };
		while ( auto res = this->findHeadAndTail( tail ) )
		{
			result.push_back( res.value().first );
			tail	= std::move( res.value().second );
		}

		return result;
	}

private:
	std::optional<std::pair<uint16_t, Update>>	findHeadAndTail(Update& update) const
	{
		for ( size_t i = 0, length = update.size(); i < length; i++ )
		{
			uint16_t	head	{ update[ i ] };
			Update		tail	{ update };
			tail.erase( tail.begin() + i );

			if ( this->areAllAfterGiven( head, tail.begin(), tail.end() ) )
				return std::pair<uint16_t, Update>{ head, tail };
		}

		return std::nullopt;
	}

	std::unordered_map<uint16_t, std::unordered_set<uint16_t>>	fRules;
};

static bool isUpdateCorrect(const Ordering& rules, const Update& update)
{
	for ( auto iter = update.begin(), end = update.end(); iter != end; iter++ )
		if ( !rules.areAllAfterGiven( *iter, std::next( iter ), end ) )
			return false;

	return true;
}

static std::pair<Ordering, std::vector<Update>> readRulesAndUpdates(std::string_view fileName)
{
	size_t								sum		{ 0 };

	std::ifstream						input	{ fileName.data() };

	Ordering							rules;
	std::vector<Update>	updates;

	std::string	line;
	while ( std::getline( input, line ).good() && !line.empty() )
	{
		std::stringstream	lineStream	{ line };

		uint16_t	first;
		uint16_t	second;
		lineStream >> first;
		lineStream.get();
		lineStream >> second;

		rules.addRule( first, second );
	}

	while ( std::getline( input, line ).good() )
	{
		std::stringstream		lineStream	{ line };

		Update	update;

		uint16_t				pageNum;
		while ( lineStream >> pageNum )
		{
			update.push_back( pageNum );
			lineStream.get();
		}
		updates.emplace_back( std::move( update ) );
	}

	return { rules, updates };
}

static size_t calculateCorrectMiddleSum(std::string_view fileName)
{
	size_t	sum					{ 0 };

	auto	[rules, updates]	= readRulesAndUpdates( fileName );

	for ( const auto& update : updates )
		if ( isUpdateCorrect( rules, update ) )
			sum	+= update[ update.size() / 2 ];

	return sum;
}

static size_t calculateFixedMiddleSum(std::string_view fileName)
{
	size_t	sum					{ 0 };

	auto	[rules, updates]	= readRulesAndUpdates( fileName );

	for ( const auto& update : updates )
		if ( !isUpdateCorrect( rules, update ) )
		{
			Update	corrected	= rules.getReorderedUpdate( update );
			sum	+= corrected[ corrected.size() / 2 ];
		}

	return sum;
}


void day05part1(std::string_view fileName)
{
	std::cout << "Sum of the middle page numbers of correct updates: " << calculateCorrectMiddleSum( fileName ) << '\n';
}

void day05part2(std::string_view fileName)
{
	std::cout << "Sum of the middle page numbers of corrected updates: " << calculateFixedMiddleSum( fileName ) << '\n';
}
