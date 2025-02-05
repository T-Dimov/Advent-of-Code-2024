#pragma once

#include <array>
#include <memory>
#include <string_view>
#include <vector>

class Trie
{
public:
	void				addWord(std::string_view word);

	// finds the xs where [ 0; x ) is a prefix of @word
	std::vector<size_t>	findPrefixes(std::string_view word) const;

private:
	void				findPrefixes(std::string_view word, size_t index, std::vector<size_t>& prefixes) const;

	bool									fWordEnd	{ false };
	std::array<std::unique_ptr<Trie>, 26>	fChildren;
};
