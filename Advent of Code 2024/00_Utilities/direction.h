#pragma once

#include <concepts>

enum class Direction : unsigned char
{
	None	= 0b0000'0000,

	U		= 0b0000'0001,
	R		= 0b0000'0010,
	D		= 0b0000'0100,
	L		= 0b0000'1000,

	UL		= U | L,
	UR		= U | R,
	DL		= D | L,
	DR		= D | R,
};

// copied from std::byre because I can't inject my own constants in it

template <std::integral IntType>
_NODISCARD constexpr Direction operator<<(const Direction dir, const IntType Shift) noexcept
{
	// every static_cast is intentional
	return static_cast<Direction>(static_cast<unsigned char>(static_cast<unsigned int>(dir) << Shift));
}

template <std::integral IntType>
_NODISCARD constexpr Direction operator>>(const Direction dir, const IntType Shift) noexcept
{
	// every static_cast is intentional
	return static_cast<Direction>(static_cast<unsigned char>(static_cast<unsigned int>(dir) >> Shift));
}

_NODISCARD constexpr Direction operator|(const Direction lhs, const Direction rhs) noexcept
{
	// every static_cast is intentional
	return static_cast<Direction>(
		static_cast<unsigned char>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs)));
}

_NODISCARD constexpr Direction operator&(const Direction lhs, const Direction rhs) noexcept
{
	// every static_cast is intentional
	return static_cast<Direction>(
		static_cast<unsigned char>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs)));
}

_NODISCARD constexpr Direction operator^(const Direction lhs, const Direction rhs) noexcept
{
	// every static_cast is intentional
	return static_cast<Direction>(
		static_cast<unsigned char>(static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs)));
}

_NODISCARD constexpr Direction operator~(const Direction dir) noexcept
{
	// every static_cast is intentional
	return static_cast<Direction>(static_cast<unsigned char>(~static_cast<unsigned int>(dir)));
}

//my addition
_NODISCARD constexpr bool operator%(Direction lhs, Direction rhs) noexcept
{
	return ( lhs & rhs ) == rhs;
}

template <std::integral IntType>
constexpr Direction& operator<<=(Direction& dir, const IntType Shift) noexcept
{
	return dir = dir << Shift;
}

template <std::integral IntType>
constexpr Direction& operator>>=(Direction& dir, const IntType Shift) noexcept
{
	return dir = dir >> Shift;
}

constexpr Direction& operator|=(Direction& lhs, const Direction rhs) noexcept
{
	return lhs = lhs | rhs;
}

constexpr Direction& operator&=(Direction& lhs, const Direction rhs) noexcept
{
	return lhs = lhs & rhs;
}

constexpr Direction& operator^=(Direction& lhs, const Direction rhs) noexcept
{
	return lhs = lhs ^ rhs;
}
