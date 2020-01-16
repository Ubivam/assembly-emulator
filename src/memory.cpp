#include "../h/memory.h"
#include <ostream>
Memory::Memory()
{
	for (int i = 0; i < (pow(2,16)); i++)
	{
		mem_array.push_back((uint8_t)0x00);
	}
}

uint8_t Memory::readLocation(uint16_t loc)
{
	return mem_array[loc];
}

void Memory::writeToLocation(uint16_t loc, uint8_t value)
{
	mem_array[loc] = value;
}

void Memory::loadMemorySegment(uint16_t begin, std::vector<uint8_t> seg)
{
	auto it = begin;
	for (auto& value : seg)
	{
		mem_array[it++] = value;
	}
}

std::string Memory::to_string() const
{
	std::stringstream stream;
	for (int i = 0; i < (pow(2, 16)); i++)
	{
		if ((i % 30) == 0)
		{
			stream << std::endl;
			if (i == 0)
			{
				stream << "location[" << std::dec << i << "-" << (i + 29) << "]: ";
			}
		}
		if (mem_array[i] <= 0xF) stream << 0;
		stream << std::hex << unsigned(mem_array[i]) << " ";
	}
	return stream.str();
}

std::string Memory::to_string_memory_sector(uint16_t begin, uint16_t end)
{
	std::stringstream stream;
	bool fistFlag = false;
	if (begin < 0 || end  > (pow(2, 16)) || end < begin) return "ERROR";
	
	if (end - begin > 29)
	{
		stream << "location["<< begin << "-" << begin + (29 - (begin % 29)) << "]: ";
	}
	else
	{
		stream << "location[" << begin << "-" << end << "]: ";
	}
	for (int i = begin; i <= end; i++)
	{
		if(fistFlag)
		{
			if ((i % 30) == 0)
			{
				stream << std::endl;
				if (i - end < 30) {
					stream << "location[" << std::dec << i  << "-" << (i + 29) << "]: ";
				}
				else
				{
					stream << "location[" << std::dec << i  << "-" << end << "]: ";
				}
			}
		}
		else
		{
			fistFlag = true;
		}
		
		if (mem_array[i] <= 0xF) stream << 0;
		stream << std::hex << unsigned(mem_array[i]) << " ";
	}
	return stream.str();
}

std::ostream& operator<<(std::ostream& os, const Memory& mem)
{
	return os << mem.to_string();
}
