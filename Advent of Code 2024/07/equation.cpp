#include "equation.h"

#include <sstream>

static int tenToThePowerOfNumDigits(Equation::Operand number)
{
	if ( number == 0 )
		return 10;

	int	digits	{ 1 };

	while ( number )
	{
		digits	*= 10;
		number	/= 10;
	}

	return digits;
}

static Equation::Operand cc(Equation::Operand lhs, Equation::Operand rhs)
{
	return lhs * tenToThePowerOfNumDigits( rhs ) + rhs;
}

Equation::Equation(const std::string& line)
{
	std::stringstream	stream	{ line };

	stream >> fResult;
	stream.get();

	Operand	num;
	while ( stream >> num )
		fOperands.push_back( num );
}

Equation::Operand Equation::getPossibleResult(bool allowConcat) const
{
	if ( isPossibleExpression( fOperands.front(), std::next( fOperands.begin() ), allowConcat ) )
		return fResult;

	return 0;
}

bool Equation::isPossibleExpression(Operand head, Iter iter, bool allowConcat) const
{
	if ( iter == fOperands.end() )
		return head == fResult;

	return isPossibleExpression( head + *iter, std::next( iter ), allowConcat )
		|| isPossibleExpression( head * *iter, std::next( iter ), allowConcat )
		|| allowConcat && isPossibleExpression( cc( head, *iter ), std::next( iter ), allowConcat );
}
