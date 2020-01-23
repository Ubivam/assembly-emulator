#ifndef TERMINAL_H
#define TERMINAL_H
#include "asm/def.h"
#include "asm/macros.h"
#include "memory.h"


class Terminal
{
public:
	void setActiveMemory(std::shared_ptr<Memory> mem);
	void terminalLoop();
private:
	uint16_t data_in;
	uint16_t data_out;
	std::shared_ptr<Memory> mem;
};

#endif // !TERMINAL_H
