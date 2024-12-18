#include "memory.h"

#include <iostream>

int main()
{
	//Memory			memory		{ "test.txt"	, { 7, 7 }	, 12	};
	Memory			memory		{ "input.txt"	, { 71, 71 }, 1024	};

	std::cout << "Minimum number of steps needed to reach the exit: " << memory.findLowestCostRoute() << '\n';
	std::cout << "First byte that will prevent the exit from being reachable: " << memory.getFirstByteBlockingTheExit() << '\n';
}
