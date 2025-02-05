#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>

class FileLines
{
private:
	class FileLineIterator
	{
	public:
		using iterator_category	= std::input_iterator_tag;
		using value_type		= std::string;
		using difference_type	= std::ptrdiff_t;
		using pointer			= value_type*;
		using reference			= value_type&;

							FileLineIterator() = default;
		explicit			FileLineIterator(std::string_view fileName);

		bool				operator==(const FileLineIterator& other) const;

		value_type			operator*() const;

		FileLineIterator&	operator++();

	private:
		std::ifstream	fFile;
		std::string		fLine;
	};

public:
	explicit			FileLines(std::string_view fileName);

	FileLineIterator	begin() const;
	FileLineIterator	end() const;

private:
	std::string	fFileName;
};
