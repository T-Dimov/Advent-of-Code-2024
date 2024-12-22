#pragma once

#include "point2D.h"

#include <array>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

using Sequences		= std::unordered_set<std::string>;
using Coordinates	= std::unordered_map<char, Point2D>;
template<size_t H, size_t W>
using Grid			= std::array<std::array<char, W>, H>;

class Keypad
{
protected:
	static Sequences	getSequences(const Coordinates& coordinates, const std::string& doorCode);

private:
	static Sequences	getSequences(const Coordinates& coordinates, char from, char to);
	static std::string	getSequence(Point2D delta);

	static bool			isSequenceSafe(const Coordinates& coordinates, char from, std::string_view sequence);
};

class NumericKeypad : public Keypad
{
public:
	static Sequences	getSequences(const std::string& doorCode);

private:
	static const Grid<4, 3>		sLayout;
	static const Coordinates	sCoordinates;
};

class DirectionalKeypad : Keypad
{
public:
	static Sequences	getSequences(const std::string& keypadSequence);

private:
	static const Grid<2, 3>		sLayout;
	static const Coordinates	sCoordinates;
};
