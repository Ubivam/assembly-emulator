#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include "asm/def.h"
#include "asm/macros.h"
#include "memory.h"
#include "cpu.h"
class Timer 
{
public:
	Timer();
	void timerLoop();
	void setActiveMemory(std::shared_ptr<Memory> mem);
	void configureTimerFromMem();

private:
	uint16_t timer_cfg;
	uint16_t count_interval;
	std::shared_ptr<Memory> mem;
};

#endif
