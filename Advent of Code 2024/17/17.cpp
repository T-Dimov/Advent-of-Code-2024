#include "computer.h"

#include <iostream>

int main()
{
	const auto		fileName	{ "input.txt" };

	Computer		computer	{ fileName };
	computer.run();
	computer.printOutput( std::cout );
}
