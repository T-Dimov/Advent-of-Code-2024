#include "fileSystem.h"

#include <fstream>

FileSystem::FileSystem(std::string_view fileName)
{
	std::ifstream	input	{ fileName.data() };
	std::string		diskMap;

	input >> diskMap;

	bool	isFile	{ true };

	for ( auto c : diskMap )
	{
		const int	chunk	{ std::exchange( isFile, !isFile ) ? fLastFileID++ : -1 };

		const int	size	{ c - '0' };
		for ( int j = 0; j < size; j++ )
			fChunks.push_back( chunk );
	}

	fLastFileID--;
}

void FileSystem::compactFiles()
{
	size_t	left	{ 0 };
	size_t	right	{ fChunks.size() - 1 };

	this->seekFreeForwards( left );
	this->seekFileBackwards( right );

	while ( left < right )
	{
		std::swap( fChunks[ left++ ], fChunks[ right-- ] );
		this->seekFreeForwards( left );
		this->seekFileBackwards( right );
	}
}

size_t FileSystem::calculateContinuousChecksum() const
{
	size_t	checksum	{ 0 };
	size_t	length		{ fChunks.size() - 1 };
	this->seekFileBackwards( length );
	length++;

	for ( size_t i = 0; i < length; i++ )
		checksum	+= i * fChunks[ i ];

	return checksum;
}

void FileSystem::moveFiles()
{
	for ( int fileID = fLastFileID; fileID > 0; fileID-- )
	{
		auto	fileSpan	{ this->findFile( fileID ) };
		auto	freeSpan	{ this->findFree( fileSpan.size(), 0 ) };

		if ( freeSpan.data() < fileSpan.data() )
			for ( size_t i = 0, length = freeSpan.size(); i < length; i++ )
				std::swap( fileSpan[ i ], freeSpan[ i ] );
	}
}

size_t FileSystem::calculateNonFragmentedChecksum() const
{
	size_t	checksum	{ 0 };

	for ( size_t i = 0, length = fChunks.size(); i < length; i++ )
		if ( int chunk = fChunks[ i ]; chunk != -1 )
			checksum	+= i * chunk;

	return checksum;
}


void FileSystem::seekFreeForwards(size_t& head) const
{
	const size_t	size	{ fChunks.size() };

	while ( head < size && fChunks[ head ] != -1 )
		head++;
}

void FileSystem::seekFileBackwards(size_t& head) const
{
	const size_t	size	{ fChunks.size() };

	while ( head < size && fChunks[ head ] == -1 )
		head--;
}

std::span<int> FileSystem::findFile(int fileID)
{
	size_t	right	{ fChunks.size() - 1 };

	while ( fChunks[ right ] != fileID )
	{
		this->seekFileBackwards( right );
		if ( fChunks[ right ] != fileID )
			right--;
	}

	size_t left		{ right - 1 };

	while ( left > 0 && fChunks[ left ] == fileID )
		left--;

	return { fChunks.begin() + ( left == 0 ? left : left + 1), fChunks.begin() + right + 1 };
}

std::span<int> FileSystem::findFree(size_t size, size_t start)
{
	size_t	left	{ start };
	this->seekFreeForwards( left );

	size_t	end		{ left + size };
	if ( end > fChunks.size() )
		return {};

	size_t	right	{ left };

	while ( right < end )
	{
		if ( fChunks[ right ] != -1 )
			break;
		right++;
	}

	if ( right == end )
		return { fChunks.begin() + left,size };

	return this->findFree( size, right );
}
