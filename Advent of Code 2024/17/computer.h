#pragma once

#include <string_view>
#include <vector>
#include <iostream>
#include <functional>

class Computer
{
public:
	explicit	Computer(std::string_view fileName);

	void		run();
	void		printOutput(std::ostream& os) const;

private:
	class Instruction
	{
	public:
		using	Operation	= std::function<void( Computer&, int64_t )>;

		explicit		Instruction(Operation operation) : fOperation( std::move( operation ) ) {}
		void			operator()(Computer& computer,int64_t operand) const { fOperation( computer, operand ); }

		static int64_t	translateComboOperand(const Computer& computer,int64_t operand)
		{
			if ( operand == 4 )
				return computer.fA;
			else if ( operand == 5 )
				return computer.fB;
			else if ( operand == 6 )
				return computer.fC;

			return operand;
		}

	private:
		Operation	fOperation;
	};

	void		incrementPC();
	void		setPC(size_t newPC);

	size_t							fPC;

	std::vector<int8_t>				fInstructions;

	int64_t							fA;
	int64_t							fB;
	int64_t							fC;

	std::vector<int64_t>			fOutput;

	static std::vector<Instruction>	sISA;
};
