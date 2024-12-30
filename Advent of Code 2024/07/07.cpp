#include "equation.h"

#include <iostream>
#include <fstream>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <functional>

static std::vector<Equation> readEquations(std::string_view fileName)
{
	std::vector<Equation>	equations;

	std::ifstream			input	{ fileName.data() };
	std::string				line;
	while ( std::getline( input, line ).good() && !line.empty() )
		equations.emplace_back( line );

	return equations;
}

static Equation::Operand calculateTotalCalibrationResult(const std::vector<Equation>& equations, bool allowConcat)
{
	return std::ranges::fold_left_first( equations | std::views::transform( std::bind_back( &Equation::getPossibleResult, allowConcat ) ), std::plus {} ).value();
}

void day07(std::string_view fileName)
{
	const auto	equations	{ readEquations( fileName ) };

	std::cout << "Total calibration result: "						<< calculateTotalCalibrationResult( equations, false )	<< '\n';
	std::cout << "Total calibration result with concatenation: "	<< calculateTotalCalibrationResult( equations, true )	<< '\n';
}
