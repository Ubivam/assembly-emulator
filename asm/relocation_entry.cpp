#ifndef RELOCATION_ENTRY
#define RELOCATION_ENTRY

#include "../h/asm/relocation_entry.h"
#include "../h/asm/def.h"
#include "../h/asm/macros.h"

uint32_t RelocationEntry::getOffset()
{
	return _offset;
}

RelocationType RelocationEntry::getType()
{
	return _type;
}

uint32_t RelocationEntry::getIndex()
{
	return _index;
}

RelocationEntry::RelocationEntry(uint32_t offset, RelocationType type, uint32_t index) : _offset(offset),
																						 _type(type),
																						 _index(index)
{
}

std::string RelocationEntry::to_string() const
{
	std::stringstream stream;
	std::string type = (_type == R_386_32) ? "ABS" : "REL";
	stream << std::dec << _index  << "\t\t" << type << "\t\t" << "0x0" << std::uppercase << std::hex << _offset;
	auto s = stream.str();
	return s;
}

std::ostream &operator<<(std::ostream &os, RelocationEntry &entry)
{
	os << entry.to_string();
	return os;
}

#endif