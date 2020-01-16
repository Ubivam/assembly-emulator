#ifndef UTIL_H
#define UTIL_H
#include "../h/asm/macros.h"

class Utility
{
public:
	static void writeFile(std::string fileName);
	static void readBinFile(std::string fileName);

	static void printCode();

	static std::vector<uint8_t> code;
	static SymbolTable symbTab;
};


#endif
