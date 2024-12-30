#include "monitoringDevice.h"

#include <iostream>

void day24(std::string_view fileName)
{
	MonitoringDevice	monitoringDevice	{ fileName };

	std::cout << "Z wires output: " << monitoringDevice.calculateZ() << '\n';
}
