#include "computer.h"

#include <iostream>

void day17(std::string_view fileName)
{
	Computer	computer	{ fileName };
	computer.run();
	std::cout << "Computer output: ";
	computer.printOutput( std::cout );
	std::cout << '\n';
}
