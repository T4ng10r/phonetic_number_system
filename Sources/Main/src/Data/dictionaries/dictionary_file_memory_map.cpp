#include "dictionary_file_memory_map.h"
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#define WINDOWS_EOF_1CHAR '\r'
#define WINDOWS_EOF_2CHAR '\n'
#define LINUX_EOF_CHAR '\n'

dictionary_file_memory_map::dictionary_file_memory_map(CDictionaryData * ptrPublic)
{
	m_ptrPublic = ptrPublic;
}
//--------------------------------------------------------------------------------
dictionary_file_memory_map::~dictionary_file_memory_map(){};
//--------------------------------------------------------------------------------
bool dictionary_file_memory_map::openFile(const std::string & filePath)
{
	if (false==boost::filesystem::exists(filePath.c_str()))
		return false;
	dictionaryFileSize = boost::filesystem::file_size(filePath.c_str());
	fileMapping = boost::interprocess::file_mapping
		(filePath.c_str(), boost::interprocess::read_only);
	return true;
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::close_file(){};
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::parseLineWordCounts(const char ** memAddr, const char * const endMemPos)
{
	const char * startAddr = *memAddr;
	while(!isspace(**memAddr) && *memAddr<endMemPos)
	{
		(*memAddr)++;
	}
	if (*memAddr<endMemPos)
	{
		words_count_ = boost::lexical_cast<unsigned int>(std::string(startAddr,*memAddr-startAddr));
	}
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::moveTillEndOfLine(const char ** memAddr, const char * const endMemPos)
{
	while(*memAddr<endMemPos )
	{
		if (**memAddr==WINDOWS_EOF_1CHAR && *(*memAddr+1)==WINDOWS_EOF_2CHAR)
		{
			(*memAddr)++;
			(*memAddr)++;
			return;
		}
		if (**memAddr==LINUX_EOF_CHAR)
		{
			(*memAddr)++;
			return;
		}
		(*memAddr)++;
	}
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::parseLineWithWord(const char ** memAddr, const char * const endMemPos)
{
	const char * startAddr = *memAddr;
	while(*memAddr<endMemPos)
	{
		if (**memAddr=='/')
		{
			dictionaryWords.push_back(std::make_pair(startAddr,*memAddr-startAddr));
			return;
		}
		if ((**memAddr==WINDOWS_EOF_1CHAR && *(*memAddr+1)==WINDOWS_EOF_2CHAR) || **memAddr==LINUX_EOF_CHAR)
		{
			dictionaryWords.push_back(std::make_pair(startAddr,*memAddr-startAddr));
			return;
		}
		(*memAddr)++;
	}
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::loadFileContent()
{
	fileMappedRegion = boost::interprocess::mapped_region
		( fileMapping                   //Memory-mappable object
		, boost::interprocess::read_only               //Access mode
		, 0               //Offset from the beginning of shm
		, dictionaryFileSize      //Length of the region
		);

	//process all entries into pair - start offset and length
	//get entries count
	const char *mem = static_cast<char*>(fileMappedRegion.get_address());
	const char *endMemPos = mem+fileMappedRegion.get_size();
	parseLineWordCounts(&mem, endMemPos);
	while(mem<endMemPos)
	{
		moveTillEndOfLine(&mem, endMemPos);
		parseLineWithWord(&mem, endMemPos);
	}
}
//--------------------------------------------------------------------------------
std::string dictionary_file_memory_map::getWordByNdex(unsigned int index)
{
	if (index >= words_count_)
	{
		return std::string();
	}
	return std::string(dictionaryWords[index].first,dictionaryWords[index].second);
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::removeDictionary()
{
	dictionaryWords.clear();
};

