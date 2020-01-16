#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <vector>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>

class Memory
{
public:
	Memory();
	uint8_t readLocation(uint16_t loc);
	void writeToLocation(uint16_t loc, uint8_t value);
	void loadMemorySegment(uint16_t begin, std::vector<uint8_t> seg);

public:
	//Pomocne metode
	std::string to_string() const;
	std::string to_string_memory_sector(uint16_t begin, uint16_t end);
	
public:
	friend std::ostream& operator<<(std::ostream& os, const Memory& symbol);

private:
	std::vector <uint8_t> mem_array;
};

#endif