#pragma once

#include "direction.h"

#include <cstddef>
#include <vector>
#include <string>
#include <unordered_set>

using Map	= std::vector<std::string>;

class Guard
{
public:
	explicit			Guard(const Map& map);

	void				patrol(Map& map);

	bool				hasLeft() const;
	bool				isInLoop() const;

private:
	void				moveUntilObstacle(Map& map);
	void				turn();

	static Direction	getDirection(char c);
	void				findGuard(const Map& map);

	struct Turn
	{
		int			fRow	{ -1 };
		int			fCol	{ -1 };
		Direction	fDir	{ Direction::None };

		auto operator<=>(const Turn&) const = default;
	};

	int					fX			{ -1 };
	int					fY			{ -1 };
	Direction			fDir		{ Direction::None };

	bool				fIsInLoop	{ false };
	std::vector<Turn>	fTurns;
};
