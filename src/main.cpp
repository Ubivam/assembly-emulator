#include "../h/asm/macros.h"
#include "../h/asm/table.h"
#include <fstream>
#include <memory>
#include "../h/memory.h"
#include "../h/cpu.h"
#include "../h/utility.h"
#include "../h/linker.h"
#include "../h/asm/section.h"
#include "../h/asm/def.h"
#include "../h/asm/macros.h"

int main(int argc, char* argv[])
{
    std::shared_ptr<Linker> linker = Linker::getInstance();
    InstructionCodeTable::init();
    Utility::readBinFile("initialize_system.so");
    uint32_t place_param = 1;
    auto count_param = 1;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            count_param++;
            std::string arg = argv[i];
            auto pos1 = arg.find_first_of('=');
            std::string section_name = arg.substr(pos1 + 1, std::string::npos);
            auto pos2 = section_name.find_first_of('@');
            section_name = section_name.substr(0, pos2);
            pos2 = arg.find_first_of('@');
            std::string adr_s = arg.substr(pos2 + 1, std::string::npos);
            uint16_t adr = strtoul(adr_s.c_str(), nullptr, 16);
            linker->insertPlaceParameter(section_name, adr);
        }
    }
    for (int i = count_param; i < argc; i++)
    {
        Utility::readBinFile(argv[i]);
    }
    Utility::setIVTSectionCounters();
    linker->resolveAdress();
    if (!linker->areAllAdressCorrect())
    {
        PRINT("There was an error with symbols in sections!");
        return -1;
    }
    Utility::printCode();
    std::shared_ptr<Memory> mem = std::make_shared<Memory>();
    Utility::linkerScriptIVT(mem);
    auto section_tab = linker->getSectionTable()->getTable();
    for (auto sec : section_tab)
    {
        if (sec == section_tab[0] || sec == section_tab[1] || sec == section_tab[2] || sec == section_tab[3] || sec == section_tab[4])
            continue;
        Instruction code;
        for (auto inst : sec->getData())
        {
            for (auto& word : inst)
            {
                code.push_back(word);
            }
        }
        mem->loadMemorySegment(sec->getBeginLocationCounter(), code);
    }
    PRINT(mem->to_string_memory_sector(0, 4000));
    std::shared_ptr<Cpu> cpu = Cpu::getInstance();
    cpu->setActiveMemory(mem);
    cpu->cpuWorkLoop();
   
    return 0;
}
