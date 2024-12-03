#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

int getMultSum(std::string_view fileName)
{
	int					multSum	{ 0 };

	const std::regex	pattern	{ R"rgx(mul\((\d{1,3}),(\d{1,3})\))rgx" };
	std::ifstream		input	{ fileName.data() };

	std::string	line;
	while ( std::getline( input, line ).good() )
	{
		auto	begin	{ line.cbegin() };
		auto	end		{ line.cend() };
		std::smatch	match;
		while ( std::regex_search( begin, end, match, pattern ) )
		{
			std::string	arg0	= match[ 1 ].str();
			std::string	arg1	= match[ 2 ].str();

			multSum	+= std::stoi( arg0 ) * std::stoi( arg1 );

			begin	= match[ 0 ].second;
		}
	}

	return multSum;
}

int getMultSumWithEnable(std::string_view fileName)
{
	int					multSum		{ 0 };

	const std::regex	pattern		{ R"rgx(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))rgx" };
	std::ifstream		input		{ fileName.data() };

	bool				multEnabled	{ true };

	std::string	line;
	while ( std::getline( input, line ).good() )
	{
		auto	begin	{ line.cbegin() };
		auto	end		{ line.cend() };
		std::smatch	match;
		while ( std::regex_search( begin, end, match, pattern ) )
		{
			if ( std::string command = match[ 0 ].str(); command == "do()" )
			{
				multEnabled	= true;
			}
			else if ( command == "don't()" )
			{
				multEnabled	= false;
			}
			else if ( multEnabled )
			{
				std::string	arg0	= match[ 1 ].str();
				std::string	arg1	= match[ 2 ].str();

				multSum	+= std::stoi( arg0 ) * std::stoi( arg1 );
			}

			begin	= match[ 0 ].second;
		}
	}

	return multSum;
}

int main()
{
	const auto	fileName	{ "input.txt" };

	std::cout << "Multiplications' sum: "			<< getMultSum( fileName )			<< '\n';
	std::cout << "Enabled multiplications' sum: "	<< getMultSumWithEnable( fileName )	<< '\n';
}
