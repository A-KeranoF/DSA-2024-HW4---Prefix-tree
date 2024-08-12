#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <string.h>

namespace fs = std::filesystem;

/// Display how to use the program from the command line
void displayUsage(const char* executablePath)
{
	try {
		fs::path ep(executablePath);
		
		std::cout
		<< "Usage:\n\t"
		<< ep.filename()
		<< " <dictionary> <filter> <text>"
		<< std::endl;
	}
	catch (...) {
		std::cout << "Cannot parse path from argv[0]";
	}
}

struct Statistics {
	size_t dictValid = 0;
	size_t dictInvalid = 0;
	size_t filterValid = 0;
	size_t filterInvalid = 0;
	size_t removedWords = 0;
	size_t correctWords = 0;
	size_t incorrectWords = 0;
};

void fillDictionary(Dictionary& dictionary, std::ifstream &file, Statistics& statistics) {
	size_t line = 1;
	std::string word;
	while (std::getline(file, word)) 
	{
		if (word[0] == '#' || word.empty() || word[0] == '\n')
			continue;
		if (!Dictionary::isCorrectWord(word.c_str())) 
		{
			++statistics.dictInvalid;
			std::cout << "ERROR: incorrect entry " << '\"' << word << '\"' << "on line " << line++ << std::endl; 
			continue;
		}
		++statistics.dictValid;
		dictionary.insert(word.c_str());
	}
}

void filterDictionary(Dictionary& dictionary, std::ifstream &file, Statistics& statistics) {
	size_t line = 1;
	std::string word;
	while (std::getline(file, word)) 
	{
		if (word[0] == '#' || word.empty() || word[0] == '\n')
			continue;
		if (!Dictionary::isCorrectWord(word.c_str())) 
		{
			++statistics.filterInvalid;
			std::cout << "ERROR: incorrect entry " << '\"' << word << '\"' << "on line " << line++ << std::endl; 
			continue;
		}
		++statistics.filterValid;
		dictionary.erase(word.c_str());
		++statistics.removedWords;
	}
}

void compareDictionary(Dictionary& dictionary, std::ifstream &file, Statistics& statistics) {
	size_t line = 1;
	std::string word;
	while (std::getline(file, word)) 
	{
		if (word[0] == '#' || word.empty() || word[0] == '\n')
			continue;
		if (!Dictionary::isCorrectWord(word.c_str())) 
		{
			++statistics.incorrectWords;
			std::cout << "SPELLING ERROR: " << '\"' << word << '\"' << "on line " << line++ << std::endl; 
			continue;
		}
		if(!dictionary.contains(word.c_str()))
			++statistics.incorrectWords;
		else 
			++statistics.correctWords;
	}
}

void printStatistics(const Statistics& statistics, size_t dictSize) {
    printf("\nStatistics:\n");
    printf("%24s %4zu correct, %4zu incorrect\n", "Dictionary entries:", statistics.dictValid, statistics.dictInvalid);
    printf("%24s %4zu correct, %4zu incorrect\n", "Filter entries:", statistics.filterValid, statistics.filterInvalid);
    printf("%24s %4zu\n", "Removed words:", statistics.removedWords);
    printf("%24s %4zu\n", "Resultant dictionary:", dictSize);
    printf("%24s %4zu correct, %4zu incorrect\n", "Words in text:", statistics.correctWords, statistics.incorrectWords);
}

int main(int argc, char* argv[])
{
	if(argc < 4) {
		displayUsage(argv[0]);
		return 1;
	}

	Dictionary dictionary;
	Statistics statistics;
	std::ifstream dict(argv[1]);
	std::ifstream filter(argv[2]);
	std::ifstream controlText(argv[3]);

	if (!dict.is_open() || !filter.is_open() || !controlText.is_open()) {
		std::cout << "Error, couldn't open files!" << std::endl;
		return -1;
	}

	std::cout << "\nLoading dictionary from .\\" << argv[1] << "..." << std::endl;
	fillDictionary(dictionary, dict, statistics);

	std::cout << "\nRemoving the words listed at .\\" << argv[2] << "..." << std::endl;
	filterDictionary(dictionary, filter, statistics);
	
	std::cout << "\nVerifying the contents of .\\" << argv[3] << "..." << std::endl;
	compareDictionary(dictionary, controlText, statistics);

	dict.close();
	filter.close();
	controlText.close();
			
	printStatistics(statistics, dictionary.size());

	return 0;
}