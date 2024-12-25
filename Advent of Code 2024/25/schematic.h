#pragma once

#include <fstream>
#include <array>

class Schematic
{
public:
	explicit	Schematic(std::ifstream& stream);

	bool		isKey() const;
	bool		doesFit(const Schematic& other) const;

private:
	std::array<uint8_t, 5>	fHeights	{ 0, 0, 0, 0, 0 };
	bool					fIsKey		{ false };
};
