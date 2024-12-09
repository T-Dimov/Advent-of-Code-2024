#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <array>

template<typename T>
concept IsEnum = std::is_enum_v<T>;

template<IsEnum Enum>
Enum operator|(Enum lhs, Enum rhs)
{
	return static_cast<Enum>( std::to_underlying( lhs ) | std::to_underlying( rhs ) );
}

template<IsEnum Enum>
Enum operator|=(Enum& lhs, Enum rhs)
{
	return lhs = lhs | rhs;
}

template<IsEnum Enum>
Enum operator&(Enum lhs, Enum rhs)
{
	return static_cast<Enum>( std::to_underlying( lhs ) & std::to_underlying( rhs ) );
}

template<IsEnum Enum>
Enum operator&=(Enum& lhs, Enum rhs)
{
	return lhs = lhs & rhs;
}

template<IsEnum Enum>
bool operator%(Enum lhs, Enum rhs)
{
	return ( lhs & rhs ) == rhs;
}


enum class Direction : uint8_t
{
	None	= 0b0000'0000,

	U		= 0b0000'0001,
	D		= 0b0000'0010,
	L		= 0b0000'0100,
	R		= 0b0000'1000,

	UL		= U | L,
	UR		= U | R,
	DL		= D | L,
	DR		= D | R,
};


constexpr std::string_view	string	{ "XMAS" };
constexpr size_t			strLen	{ 4 };

static auto countXMASAtPosition(const std::vector<std::string>& lines, size_t row, size_t column, Direction directions)
{
	using enum Direction;

	std::vector<std::string>	potentials;

	if ( directions % U )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row - i ][ column ];

		potentials.push_back( potential );
	}

	if ( directions % D )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row + i ][ column ];

		potentials.push_back( potential );
	}

	if ( directions % L )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row ][ column - i ];

		potentials.push_back( potential );
	}

	if ( directions % R )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row ][ column + i ];

		potentials.push_back( potential );
	}

	if ( directions % UL )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row - i ][ column - i ];

		potentials.push_back( potential );
	}

	if ( directions % UR )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row - i ][ column + i ];

		potentials.push_back( potential );
	}

	if ( directions % DL )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row + i ][ column - i ];

		potentials.push_back( potential );
	}

	if ( directions % DR )
	{
		std::string	potential;
		for ( size_t i = 0; i < strLen; i++ )
			potential	+= lines[ row + i ][ column + i ];

		potentials.push_back( potential );
	}

	return std::ranges::count( potentials, string );
}

static size_t countXMAS(std::string_view fileName)
{
	size_t						counts	{ 0 };

	std::ifstream				input	{ fileName.data() };

	std::vector<std::string>	lines;

	std::string	line;
	while ( std::getline( input, line ).good() )
		lines.emplace_back( std::move( line ) );

	if ( !lines.empty() )
	{
		for ( size_t i = 0, length = lines.size(); i < length; i++ )
		{
			using enum Direction;

			Direction	directions	{ None };

			if ( i >= strLen - 1 )
				directions	|= U;

			if ( i <= length - strLen )
				directions	|= D;

			// assume all lines are the same length, it certainly looks so
			for ( size_t j = 0, lineLen = lines.front().length(); j < lineLen; j++ )
			{
				Direction	dirs	{ directions };

				if ( j >= strLen - 1 )
					dirs	|= L;

				if ( j <= lineLen - strLen )
					dirs	|= R;

				counts	+= countXMASAtPosition( lines, i, j, dirs );
			}
		}
	}

	return counts;
}

static auto countX_MASAtPosition(const std::vector<std::string>& lines, size_t row, size_t column)
{
	std::array<std::string, 4>	potentials;

	( ( potentials[ 0 ] += lines[ row - 1 ][ column - 1 ] ) += 'A' ) += lines[ row + 1 ][ column + 1 ];
	( ( potentials[ 1 ] += lines[ row + 1 ][ column + 1 ] ) += 'A' ) += lines[ row - 1 ][ column - 1 ];
	( ( potentials[ 2 ] += lines[ row - 1 ][ column + 1 ] ) += 'A' ) += lines[ row + 1 ][ column - 1 ];
	( ( potentials[ 3 ] += lines[ row + 1 ][ column - 1 ] ) += 'A' ) += lines[ row - 1 ][ column + 1 ];

	return std::ranges::count( potentials, "MAS" ) / 2; // the X-MASes are each counted twice because both legs are included
}

static size_t countX_MAS(std::string_view fileName)
{
	size_t						counts { 0 };

	std::ifstream				input { fileName.data() };

	std::vector<std::string>	lines;

	std::string	line;
	while ( std::getline( input, line ).good() )
		lines.emplace_back( std::move( line ) );

	if ( !lines.empty() )
	{
		for ( size_t i = 1, length = lines.size() - 1; i < length; i++ )
		{
			// assume all lines are the same length, it certainly looks so
			for ( size_t j = 1, lineLen = lines.front().length() - 1; j < lineLen; j++ )
			{
				if ( lines[ i ][ j ] == 'A' )
					counts	+= countX_MASAtPosition( lines, i, j );
			}
		}
	}

	return counts;
}

int main()
{
	const auto	fileName	{ "input.txt" };

	std::cout << std::format( "\"{}\" appears {} times\n", string, countXMAS( fileName ) );
	std::cout << std::format( "\"X-MAS\" appears {} times\n", countX_MAS( fileName ) );
}
