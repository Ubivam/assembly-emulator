#ifndef RELOCATION_ENTRY_H
#define RELOCATION_ENTRY_H

#include "macros.h"
#include "def.h"
#include <sstream>

class RelocationEntry
{
private:
	uint32_t _offset;
	RelocationType _type;
	uint32_t _index;

public:
	uint32_t getOffset();
	RelocationType getType();
	uint32_t getIndex();
public:
	RelocationEntry(uint32_t offset, RelocationType type, uint32_t index);

	friend std::ostream& operator <<(std::ostream& os, RelocationEntry& entry);
	std::string to_string() const;
};


#endif