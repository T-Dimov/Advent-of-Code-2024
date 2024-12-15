#pragma once

#include "point2D.h"

#include <string_view>
#include <vector>
#include <string>

namespace Direction
{
	constexpr const std::byte	None	{ 0b0000'0000 };

	constexpr const std::byte	U		{ 0b0000'0001 };
	constexpr const std::byte	R		{ 0b0000'0010 };
	constexpr const std::byte	D		{ 0b0000'0100 };
	constexpr const std::byte	L		{ 0b0000'1000 };

	inline bool operator%(std::byte lhs, std::byte rhs)
	{
		return ( lhs & rhs ) == rhs;
	}
};

class Map
{
public:
				Map(std::string_view fileName, bool wide);

	void		print() const;

	void		moveRobot();
	size_t		calculateSumOfBoxCoordinates() const;

private:
	void		readInstructions(std::string_view line);

	static bool	isBox(char cell, bool allowRightHalf = true);

	void		moveOnce(std::byte dir);
	void		moveU();
	void		moveD();
	void		moveL();
	void		moveR();

	size_t		searchU(size_t i);
	size_t		searchD(size_t i);
	size_t		searchL(size_t j);
	size_t		searchR(size_t j);

	void		pushU(size_t i);
	void		pushD(size_t i);
	void		pushL(size_t j);
	void		pushR(size_t j);

	std::vector<std::string>	fMap;
	Point2D						fSize;
	bool						fIsWide			{ false };

	Point2D						fRobotPosition;

	std::vector<std::byte>		fInstructions;
};

