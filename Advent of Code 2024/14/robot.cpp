#include "robot.h"

#include <regex>

Robot::Robot(std::string_view input)
{
	const std::regex	pattern	{ R"rgx((-?\d+).*?(-?\d+).*?(-?\d+).*?(-?\d+))rgx" };

	auto				begin	{ input.cbegin() };
	auto				end		{ input.cend() };

	if ( std::match_results<decltype(begin)> match; std::regex_search(begin, end, match, pattern) )
	{
		std::string	arg0	= match[ 1 ].str();
		std::string	arg1	= match[ 2 ].str();
		std::string	arg2	= match[ 3 ].str();
		std::string	arg3	= match[ 4 ].str();

		fPosition.fX	= std::stoi( arg0 );
		fPosition.fY	= std::stoi( arg1 );
		fVelocity.fX	= std::stoi( arg2 );
		fVelocity.fY	= std::stoi( arg3 );
	}
}

Point2D Robot::getPosition() const
{
	return fPosition;
}

void Robot::move(size_t seconds, const Point2D& spaceSize)
{
	fPosition	+= fVelocity * seconds;
	fPosition	+= spaceSize * seconds;

	fPosition	%= spaceSize;
}
