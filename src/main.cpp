#include "../h/asm/macros.h"
#include "../h/asm/table.h"
#include <fstream>
#include <memory>
#include "../h/memory.h"
#include "../h/cpu.h"
#include "../h/utility.h"
#include "../h/linker.h"

int main(int argc, char *argv[])
{
        /*if (argc < 3)
        {
                PRINT("You entered wrong arguments");
                return -1;
        }*/
    std::shared_ptr<Linker> linker = Linker::getInstance();
    InstructionCodeTable::init();
    Utility::readBinFile(argv[1]);
    linker->resolveAdress();
    Utility::printCode();
    std::shared_ptr<Memory> mem = std::make_shared<Memory>();
    mem->loadMemorySegment(0,Utility::code);
    mem->writeToLocation(0, 0x9c);
    mem->writeToLocation(2, 0x16);
    mem->writeToLocation(1001, 0xFF);
    mem->writeToLocation(1002, 0xFF);
    mem->writeToLocation(1003, 0xFF);
    std::vector<uint8_t> seg;
    seg.push_back(0x0b);
    seg.push_back(0x0c);
    seg.push_back(0x0d);
    mem->loadMemorySegment(1200, seg);
    PRINT(mem->to_string_memory_sector(0, 4000));
    std::shared_ptr<Cpu> cpu = Cpu::getInstance();
    cpu->setActiveMemory(mem);
    cpu->cpuWorkLoop();
   
    return 0;
}
