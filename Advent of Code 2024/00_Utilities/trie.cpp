#include "pch.h"

#include "trie.h"

void Trie::addWord(std::string_view word)
{
	if ( word.empty() )
	{
		fWordEnd	= true;
		return;
	}

	auto&	child	{ fChildren[ word.front() - 'a' ] };

	if ( !child )
		child.reset( new Trie );

	child->addWord( word.substr( 1 ) );
}

std::vector<size_t> Trie::findPrefixes(std::string_view word) const
{
	std::vector<size_t>	prefixes;

	this->findPrefixes( word, 0, prefixes );

	return prefixes;
}

void Trie::findPrefixes(std::string_view word, size_t index, std::vector<size_t>& prefixes) const
{
	if ( fWordEnd )
		prefixes.push_back( index );

	if ( const auto& child { fChildren[ word.front() - 'a' ] }; child )
		child->findPrefixes( word.substr( 1 ), index + 1, prefixes );
}
