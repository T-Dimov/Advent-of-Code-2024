#include "pch.h"

#include "fileLines.h"

FileLines::FileLines(std::string_view fileName)
	: fFileName( fileName )
{}

FileLines::FileLineIterator FileLines::begin() const
{
	return FileLineIterator{ fFileName };
}

FileLines::FileLineIterator FileLines::end() const
{
	return {};
}

FileLines::FileLineIterator::FileLineIterator(std::string_view fileName)
	: fFile( fileName.data() )
{
	++( *this ); // Read the first line
}

bool FileLines::FileLineIterator::operator==(const FileLines::FileLineIterator& other) const
{
	return const_cast<FileLineIterator*>( this )->fFile.tellg() == const_cast<FileLineIterator&>( other ).fFile.tellg();
}

FileLines::FileLineIterator::value_type FileLines::FileLineIterator::operator*() const
{
	return fLine;
}

FileLines::FileLineIterator& FileLines::FileLineIterator::operator++()
{
	if ( fFile )
		std::getline( fFile, fLine );

	return *this;
}
