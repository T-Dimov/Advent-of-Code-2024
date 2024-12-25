#include "schematic.h"

#include <string_view>
#include <vector>
#include <iostream>

static size_t countPairs(std::string_view fileName)
{
	size_t					count	{ 0 };

	std::ifstream			input	{ fileName.data() };

	std::vector<Schematic>	locks;
	std::vector<Schematic>	keys;

	while ( input.good() )
	{
		Schematic	schematic	{ input };

		if ( schematic.isKey() )
			keys.push_back( schematic );
		else
			locks.push_back( schematic );
	}

	for ( const Schematic& key : keys )
		for ( const Schematic& lock : locks )
			if ( key.doesFit( lock ) )
				count++;

	return count;
}

int main()
{
	std::cout << "Fitting pairs count: " << countPairs( "input.txt." ) << '\n';
}
