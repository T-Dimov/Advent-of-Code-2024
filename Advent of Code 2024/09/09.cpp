#include "fileSystem.h"

#include <iostream>

void day09part1(std::string_view fileName)
{
	FileSystem	fileSystem	{ fileName };
	fileSystem.compactFiles();

	std::cout << "Compacted file system checksum: " << fileSystem.calculateContinuousChecksum() << '\n';
}

void day09part2(std::string_view fileName)
{
	FileSystem	fileSystem	{ fileName };
	fileSystem.moveFiles();

	std::cout << "Compacted file system checksum: " << fileSystem.calculateNonFragmentedChecksum() << '\n';
}
