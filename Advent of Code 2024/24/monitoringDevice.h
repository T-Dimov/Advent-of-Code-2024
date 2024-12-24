#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <array>
#include <optional>
#include <string>

class MonitoringDevice
{
public:
	explicit	MonitoringDevice(std::string_view fileName);

	size_t		calculateZ();

private:
	class LogicGate
	{
	public:
		enum class Operation
		{
			AND,
			OR,
			XOR,
		};

					LogicGate() = default;
		explicit	LogicGate(size_t value);

		void		set(std::string_view operation, LogicGate* lhs, LogicGate* rhs);

		size_t		operator()();

	private:
		using Value	= std::optional<size_t>;

		Operation	fOperation	{ Operation::AND };

		LogicGate*	fLhs		{ nullptr };
		LogicGate*	fRhs		{ nullptr };

		Value		fRes		{ std::nullopt };
	};

	std::unordered_map<std::string, LogicGate>	fWires;
	std::array<LogicGate*, 64>					fZGates;
};
