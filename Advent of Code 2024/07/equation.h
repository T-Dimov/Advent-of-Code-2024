#pragma once

#include <string>
#include <vector>

class Equation
{
public:
	using Operand	= uint64_t;

	explicit	Equation(const std::string& line);

	Operand		getPossibleResult(bool allowConcat) const;

private:
	using Operands	= std::vector<Operand>;
	using Iter		= Operands::const_iterator;

	bool		isPossibleExpression(Operand head, Iter iter, bool allowConcat) const;

	Operand		fResult		{ 0 };
	Operands	fOperands;
};

