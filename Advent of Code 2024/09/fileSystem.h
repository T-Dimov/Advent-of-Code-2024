#pragma once

#include <string_view>
#include <vector>
#include <span>

class FileSystem
{
public:
	explicit		FileSystem(std::string_view fileName);

	void			compactFiles();
	size_t			calculateContinuousChecksum()		const;

	void			moveFiles();
	size_t			calculateNonFragmentedChecksum()	const;

private:
	void			seekFreeForwards(size_t& head)		const;
	void			seekFileBackwards(size_t& head)		const;
	std::span<int>	findFile(int fileID);
	std::span<int>	findFree(size_t size, size_t start);

	int					fLastFileID	{ 0 };
	std::vector<int>	fChunks;
};

