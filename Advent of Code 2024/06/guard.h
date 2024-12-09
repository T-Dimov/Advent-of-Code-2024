#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <unordered_set>

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

	static std::byte	getDirection(char c);
	void				findGuard(const Map& map);

	struct Turn
	{
		int			fRow	{ -1 };
		int			fCol	{ -1 };
		std::byte	fDir	{ Direction::None };

		auto operator<=>(const Turn&) const = default;
	};

	int					fX			{ -1 };
	int					fY			{ -1 };
	std::byte			fDir		{ Direction::None };

	bool				fIsInLoop	{ false };
	std::vector<Turn>	fTurns;
};
