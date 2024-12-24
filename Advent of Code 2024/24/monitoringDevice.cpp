#include "monitoringDevice.h"

#include <fstream>
#include <regex>

MonitoringDevice::MonitoringDevice(std::string_view fileName)
{
	std::ifstream		input { fileName.data() };
	std::string			line;

	const std::regex	wirePattern	{ R"rgx((.+?): (\d))rgx" };
	//const std::regex	gatePattern	{ R"rgx((.+?) (AND|OR|XOR) (.+?) -> (.+))rgx" };
	const std::regex	gatePattern	{ R"rgx((.+?) (AND|OR|XOR) (.+?) -> (z(\d{2})|.+))rgx" };

	while ( std::getline( input, line ).good() && !line.empty() )
	{
		auto	begin	{ line.cbegin() };
		auto	end		{ line.cend() };
		if ( std::smatch match; std::regex_search( begin, end, match, wirePattern ) )
			fWires.emplace( match[ 1 ].str(), std::stoi( match[ 2 ].str() ) );
	}

	while ( std::getline( input, line ).good() )
	{
		auto	begin { line.cbegin() };
		auto	end { line.cend() };
		if ( std::smatch match; std::regex_search( begin, end, match, gatePattern ) )
		{
			auto&	gate	{ fWires[ match[ 4 ].str() ] };
			gate.set( match[ 2 ].str(), &fWires[ match[ 1 ].str() ], &fWires[ match[ 3 ].str() ] );
			if ( match[ 5 ].matched )
				fZGates[ std::stoi( match[ 5 ].str() ) ]	= &gate;
		}
	}
}

size_t MonitoringDevice::calculateZ()
{
	size_t	result	{ 0 };

	for ( size_t i = 0; i < 64; i++ )
		if ( LogicGate* gate { fZGates[ i ] }; gate )
			result	|= (*gate)() << i;

	return result;
}

MonitoringDevice::LogicGate::LogicGate(size_t value)
	: fRes( value )
{}

void MonitoringDevice::LogicGate::set(std::string_view operation, LogicGate* lhs, LogicGate* rhs)
{
	fLhs	= lhs;
	fRhs	= rhs;

	using enum Operation;

	if ( operation == "AND" )
		fOperation	= AND;
	else if ( operation == "OR" )
		fOperation	= OR;
	else if ( operation == "XOR" )
		fOperation	= XOR;
}

size_t MonitoringDevice::LogicGate::operator()()
{
	if ( fLhs && fRhs && !fRes.has_value() )
	{
		const size_t	lhs	{ (*fLhs)() };
		const size_t	rhs	{ (*fRhs)() };

		using enum Operation;

		if ( fOperation == AND )
			fRes	= lhs & rhs;
		else if ( fOperation == OR )
			fRes	= lhs | rhs;
		else if ( fOperation == XOR )
			fRes	= lhs ^ rhs;
	}

	return fRes.value_or( 0 );
}
