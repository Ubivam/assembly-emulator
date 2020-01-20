#ifndef UTIL_H
#define UTIL_H
#include "../h/asm/macros.h"
#include "../h/memory.h"
#include "cpu.h"

class Utility
{
public:
	static void writeFile(std::string fileName);
	static void readBinFile(std::string fileName);
	
	static void linkerScriptIVT(std::shared_ptr<Memory> mem);
	static void setIVTSectionCounters();

	static void printCode();

	static std::vector<uint8_t> code;
};


#endif
