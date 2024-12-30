#include <string_view>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <functional>
#include <array>

using	Numbers	= std::vector<size_t>;
using	Iter	= Numbers::iterator;
using	Hash	= std::unordered_map<size_t, size_t>;

static size_t getNumDigits(size_t number)
{
	if ( number == 0 )
		return 1;

	size_t	result	{ 0 };

	while ( number != 0 )
	{
		result++;
		number	/= 10;
	}

	return result;
}

static size_t tenToThePowerOf(size_t power)
{
	const static std::array<size_t, 10>	powers	{ 1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000 };

	if ( power < 10 )
		return powers[ power ];

	size_t	result	{ powers[ 9 ] };
	power	-= 9;

	while ( power > 0 )
	{
		result	*= 10;
		power--;
	}

	return result;
}

static std::pair<size_t, size_t> splitNumber(size_t num, size_t digits)
{
	const size_t	denom	{ tenToThePowerOf( digits / 2 ) };

	return { num / denom, num % denom };
}

static Numbers readNumbers(std::string_view fileName)
{
	std::ifstream		input	{ fileName.data() };

	Numbers	numbers;

	size_t				number	{ 0 };
	while ( ( input >> number ).good() )
		numbers.push_back( number );

	return numbers;
}



static size_t countDescendants(size_t number, size_t currIteration, size_t maxIterations, Hash& hash)
{
	const size_t	key					{ ( number << 7 ) | currIteration }; // shift 7 bits as the iteration is max 75 which fits in 7 bits

	auto			[iter, inserted]	{ hash.insert( std::make_pair( key, 1 ) ) };

	if ( inserted && currIteration != maxIterations )
	{
		if ( number == 0 )
		{
			iter->second	= countDescendants( 1, currIteration + 1, maxIterations, hash );
		}
		else if ( size_t numDigits { getNumDigits( number ) }; numDigits % 2 == 0 )
		{
			auto [left, right]	= splitNumber( number, numDigits );
			iter->second		= countDescendants( left, currIteration + 1, maxIterations, hash )
								+ countDescendants( right, currIteration + 1, maxIterations, hash );
		}
		else
		{
			iter->second	= countDescendants( number * 2024, currIteration + 1, maxIterations, hash );
		}
	}

	return iter->second;
}

static size_t calculateNumberOfStoneAfterBlinks(const Numbers& numbers, size_t numBlinks, Hash& hash)
{
	return std::ranges::fold_left_first( std::views::transform( numbers, std::bind_back( countDescendants, 0, numBlinks, hash ) ), std::plus {} ).value();
}

void day11(std::string_view fileName)
{
	const auto	numbers	{ readNumbers( fileName ) };
	Hash		hash;

	std::cout << "Number of stones after 25 blinks: " << calculateNumberOfStoneAfterBlinks( numbers, 25, hash ) << '\n';
	std::cout << "Number of stones after 75 blinks: " << calculateNumberOfStoneAfterBlinks( numbers, 75, hash ) << '\n';
}
