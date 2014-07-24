#pragma once
#include "base_dictionary_warehouse.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

class CDictionaryData;

class dictionary_file_memory_map : public base_dictionary_warehouse
{
public:
	CDictionaryData * m_ptrPublic;
public:
	dictionary_file_memory_map(CDictionaryData * ptrPublic);
	~dictionary_file_memory_map();
	bool openFile(const std::string & filePath);
	void close_file();
	void parseLineWordCounts(const char ** memAddr, const char * const endMemPos);
	void moveTillEndOfLine(const char ** memAddr, const char * const endMemPos);
	void parseLineWithWord(const char ** memAddr, const char * const endMemPos);
	void loadFileContent();
	std::string getWordByNdex(unsigned int index);
	void removeDictionary();
public:
	std::size_t dictionaryFileSize;
	boost::interprocess::file_mapping	fileMapping;
	boost::interprocess::mapped_region  fileMappedRegion;
	//vector of pairs - offset and length of each word
	std::vector< std::pair<const char *, long long int> >  dictionaryWords;
};


