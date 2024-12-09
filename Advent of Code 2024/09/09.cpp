#include "fileSystem.h"

#include <iostream>

int main()
{
	const auto	fileName	{ "input.txt" };

	FileSystem	fileSystem	{ fileName };
	fileSystem.compactFiles();

	std::cout << "Compacted file system checksum: " << fileSystem.calculateContinuousChecksum() << '\n';

	FileSystem	fileSystem2	{ fileName };
	fileSystem2.moveFiles();

	std::cout << "Compacted file system checksum: " << fileSystem2.calculateNonFragmentedChecksum() << '\n';
}
