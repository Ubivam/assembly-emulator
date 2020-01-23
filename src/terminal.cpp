#include "../h/terminal.h"
#include "../h/cpu.h"
#include <thread>

void Terminal::setActiveMemory(std::shared_ptr<Memory> mem)
{
	this->mem = mem;
}

void Terminal::terminalLoop()
{
	while (1)
	{
		char c;
		data_in = 0;
		std::cin >> c;
		data_in |= c;
		mem->writeToLocation(0xff02, data_in);
		Cpu::getInstance()->interruptRequest(3);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		data_out = mem->readLocation(0xff00);
		std::cout << (char) data_out << std::endl;
	}
}
