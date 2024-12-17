#include "computer.h"

#include <fstream>
#include <regex>

static int intPow(int base, int64_t exp)
{
	int	result	{ 1 };

	while ( exp > 0 )
	{
		result	*= base;
		--exp;
	}

	return result;
}

/*static*/ std::vector<Computer::Instruction>	Computer::sISA {
	Computer::Instruction { [] ( Computer& computer, int64_t operand )
	{
		computer.fA	/= intPow( 2, Computer::Instruction::translateComboOperand( computer, operand ) );
		computer.incrementPC();
	} },
	Computer::Instruction { [] ( Computer& computer, int64_t operand )
	{
		computer.fB	^= operand;
		computer.incrementPC();
	} },
	Computer::Instruction { [] ( Computer& computer, int64_t operand )
	{
		computer.fB	= Computer::Instruction::translateComboOperand( computer, operand ) % 8;
		computer.incrementPC();
	} },
	Computer::Instruction { [] ( Computer& computer, int64_t operand )
	{
		if ( computer.fA == 0 )
			computer.incrementPC();
		else
			computer.setPC( operand );
	} },
	Computer::Instruction { [] ( Computer& computer, int64_t )
	{
		computer.fB	^= computer.fC;
		computer.incrementPC();
	} },
	Computer::Instruction { [] ( Computer& computer, int64_t operand )
	{
		computer.fOutput.push_back( Computer::Instruction::translateComboOperand( computer, operand ) % 8 );
		computer.incrementPC();
	} },
	Computer::Instruction { [] ( Computer& computer, int64_t operand )
	{
		computer.fB	= computer.fA / intPow( 2, Computer::Instruction::translateComboOperand( computer, operand ) );
		computer.incrementPC();
	} },
	Computer::Instruction { [] ( Computer& computer, int64_t operand )
	{
		computer.fC	= computer.fA / intPow( 2, Computer::Instruction::translateComboOperand( computer, operand ) );
		computer.incrementPC();
	} },
};

Computer::Computer(std::string_view fileName)
	: fPC( 0 )
{
	std::ifstream		input	{ fileName.data() };

	const std::regex	pattern	{ R"rgx(\d+)rgx" };

	std::string	line;
	if ( std::getline( input, line ).good() )
	{
		auto	begin	{ line.cbegin() };
		auto	end		{ line.cend() };
		if ( std::smatch match; std::regex_search( begin, end, match, pattern ) )
		{
			fA	= std::stoi( match[ 0 ].str() );
		}
	}
	if ( std::getline( input, line ).good() )
	{
		auto	begin	{ line.cbegin() };
		auto	end		{ line.cend() };
		if ( std::smatch match; std::regex_search( begin, end, match, pattern ) )
		{
			fB	= std::stoi( match[ 0 ].str() );
		}
	}
	if ( std::getline( input, line ).good() )
	{
		auto	begin	{ line.cbegin() };
		auto	end		{ line.cend() };
		if ( std::smatch match; std::regex_search( begin, end, match, pattern ) )
		{
			fC	= std::stoi( match[ 0 ].str() );
		}
	}
	if ( std::getline( input, line ).good() && std::getline( input, line ).good() )
	{
		auto		begin	{ line.cbegin() };
		auto		end		{ line.cend() };
		std::smatch	match;
		while ( std::regex_search( begin, end, match, pattern ) )
		{
			fInstructions.push_back( std::stoi( match[ 0 ].str() ) );

			begin	= match[ 0 ].second;
		}
	}
}

void Computer::run()
{
	const auto	last	{ fInstructions.size() - 1 };

	while ( fPC < last )
		Computer::sISA[ fInstructions[ fPC ] ]( *this, fInstructions[ fPC + 1 ] );
}

void Computer::printOutput(std::ostream& os) const
{
	const auto	size	{ fOutput.size() };

	if ( size > 1 )
		std::copy( fOutput.begin(), fOutput.end() - 1, std::ostream_iterator<int>( os, "," ) );

	if ( size > 0 )
		os << fOutput.back() << '\n';
}

void Computer::incrementPC()
{
	fPC	+= 2;
}

void Computer::setPC(size_t newPC)
{
	fPC	= newPC;
}
