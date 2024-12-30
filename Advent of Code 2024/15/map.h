#pragma once

#include "point2D.h"

#include <string_view>
#include <vector>
#include <string>
#include <memory>

class Map
{
public:
				Map(std::string_view fileName, bool wide);

	void		print() const;

	void		moveRobot(bool printEachStep);
	size_t		calculateSumOfBoxCoordinates() const;

	static bool	isBox(char cell, bool allowRightHalf = true);

private:
	bool		hasBrokenBox() const;

	void		readInstructions(std::string_view line);

	void		moveOnce(char dir);
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

	void		searchAndPushWide(size_t i, int dir);

	std::vector<std::string>	fMap;
	Point2D						fSize;
	bool						fIsWide			{ false };

	Point2D						fRobotPosition;

	std::vector<char>			fInstructions;


	class WideBox
	{
	public:
				WideBox(size_t row, size_t col, const Map& map);

		void	propagate(int dir);
		bool	canMove(int dir);
		void	move(int dir, std::vector<std::string>& map);

	private:

		size_t						fRow			{ 0 };
		size_t						fLeft			{ 0 };
		size_t						fRight			{ 0 };

		std::unique_ptr<WideBox>	fLeftBox;
		std::unique_ptr<WideBox>	fRightBox;
		bool						fLeftIsFree		{ false };
		bool						fRightIsLeft	{ false };

		const Map&					fMap;
	};
};

