#include "pch.h"

#include "19.h"

#include "trie.h"
#include "fileLines.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

using Designs	= std::vector<std::string>;

static std::pair<Trie, Designs> buildTrieAndGetDesigns(std::string_view fileName)
{
	Trie	trie;
	Designs	designs;

	FileLines	lines	{ fileName };
	auto		iter	{ lines.begin() };
	auto		end		{ lines.end() };

	{
		std::stringstream	ss	{ *iter };
		std::string			towel;

		while ( std::getline( ss, towel, ',' ) )
		{
			// Remove leading and trailing spaces
			towel.erase( 0, towel.find_first_not_of( ' ' ) );
			towel.erase( towel.find_last_not_of( ' ' ) + 1 );

			trie.addWord( towel );
		}
	}

	++iter;
	++iter;

	while ( iter != end )
	{
		designs.push_back( *iter );

		++iter;
	}

	return std::make_pair( std::move( trie ), std::move( designs ) );
}

static bool isDesignPossible(const Trie& trie, const std::string& design)
{
	//TODO implement
	return false;
}

static size_t countPossibleDesigns(const Trie& trie, const Designs& designs)
{
	size_t	numPossible	{ 0 };

	for ( const auto& design : designs )
		if ( isDesignPossible( trie, design ) )
			numPossible++;

	return numPossible;
}

void day19(std::string_view fileName)
{
	auto	[trie, designs]	{ buildTrieAndGetDesigns( fileName ) };
	std::cout << "Number of possible designs: " << countPossibleDesigns( trie, designs ) << '\n';
}
