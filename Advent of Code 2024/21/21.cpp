#include "keypads.h"

#include <iostream>
#include <fstream>

size_t calculateComplexity(std::string_view numericCode, const Sequences& finalSequences)
{
	const auto seq	{ std::ranges::min( finalSequences, std::ranges::less {}, [] ( const std::string& sequence ) { return sequence.length(); } ) };
	const auto num	{ std::stoi( std::string{ numericCode.substr( 0, numericCode.size() - 1 ) } ) };
	return seq.length() * num;
}

size_t calculateComplexities(std::string_view fileName, size_t dirKeypadCount)
{
	size_t	sum	{ 0 };

	std::ifstream	input	{ fileName.data() };
	std::string		line;

	while ( std::getline( input, line ).good() )
	{
		Sequences	inputSequences	{ NumericKeypad::getSequences( line ) };

		for ( size_t i = 0; i < dirKeypadCount; i++ )
		{
			Sequences	outputSequences;

			for ( const auto& seq : inputSequences )
			{
				std::cout << i << ": " << seq << '\n';

				Sequences	sequences	{ DirectionalKeypad::getSequences( seq ) };

				outputSequences.insert( std::make_move_iterator( sequences.begin() ), std::make_move_iterator( sequences.end() ) );
			}

			inputSequences	= std::move( outputSequences );
		}

		sum	+= calculateComplexity( line, inputSequences );
	}

	return sum;
}

int main()
{
	std::cout << "Sum of complexities: " << calculateComplexities( "test.txt", 3 ) << "\n";
}
