#include "monitoringDevice.h"

#include <iostream>

int main()
{
	MonitoringDevice	monitoringDevice	{ "input.txt" };

	std::cout << "Z wires output: " << monitoringDevice.calculateZ() << '\n';
}
