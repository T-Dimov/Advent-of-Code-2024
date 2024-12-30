#include "memory.h"

#include <iostream>

void day18(std::string_view fileName)
{
	//Memory			memory		{ "test.txt"	, { 7, 7 }	, 12	};
	Memory			memory		{ fileName	, { 71, 71 }, 1024	};

	std::cout << "Minimum number of steps needed to reach the exit: " << memory.findLowestCostRoute() << '\n';
	std::cout << "First byte that will prevent the exit from being reachable: " << memory.getFirstByteBlockingTheExit() << '\n';
}
