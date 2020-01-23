#include "../h/timer.h"

Timer::Timer()
{
}

void Timer::timerLoop()
{
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(count_interval));
		Cpu::getInstance()->interruptRequest(2);
		std::cout << "R4 Value: " << Cpu::getInstance()->testGetRegister(4) << std::endl;
		std::cout.flush();
	}
}

void Timer::setActiveMemory(std::shared_ptr<Memory> mem)
{
	this->mem = mem;
}

void Timer::configureTimerFromMem()
{
	if (mem)
	{
		auto a1 = mem->readLocation(0xFF10);
		auto a2 = mem->readLocation(0xFF11);
		timer_cfg = a2 << 8 | a1;
	
		switch (timer_cfg)
		{
		case _500MS:
			count_interval = 0.5;
			break;
		case _1000MS:
			count_interval = 1;
			break;
		case _1500MS:
			count_interval = 1.5;
			break;
		case _2000MS:
			count_interval = 2;
			break;
		case _5000MS:
			count_interval = 5;
			break;
		case _10S:
			count_interval = 10;
			break;
		case _30S:
			count_interval = 30;
			break;
		case _60S:
			count_interval = 60;
			break;
		default:
			count_interval = 0;
		}
	}
}
