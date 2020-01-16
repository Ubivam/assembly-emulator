#include "../h/utility.h"
#include "../h/asm/symbol.h"
#include "../h/asm/relocation_entry.h"
#include "../h/asm/section.h"
#include "../h/linker.h"
#include <iostream>
#include <fstream>
#include <string>

std::vector<uint8_t>  Utility::code = std::vector<uint8_t>();

void Utility::readBinFile(std::string fileName)
{
    std::shared_ptr<Linker> linker = Linker::getInstance();
	std::ifstream infile(fileName, std::ifstream::binary);

	uint32_t size = 0;
    std::shared_ptr<SectionTable> sec_tab = linker->getSectionTable();
    infile.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string name;
    uint32_t str_size;
    for (int i = 0; i < size-1; i++)
    {
        name = "";
        str_size = 0;
        infile.read(reinterpret_cast<char*>(&str_size), sizeof(str_size));
        name.resize(str_size);
        infile.read(&name[0], str_size);
        std::string temp(name);
        uint32_t index = 0;
        infile.read(reinterpret_cast<char*>(&index), sizeof(uint32_t));
        uint32_t begin_loc = 0;
        infile.read(reinterpret_cast<char*>(&begin_loc), sizeof(uint32_t));
        uint32_t end_loc = 0;
        infile.read(reinterpret_cast<char*>(&end_loc), sizeof(uint32_t));

        std::shared_ptr<Section> sec = std::make_shared<Section>(temp, index, 0);
        sec->setBegin(begin_loc);
        sec->setEnd(end_loc);
        sec_tab->insert(sec);
        uint32_t realoc_size = 0;
        infile.read(reinterpret_cast<char*>(&realoc_size), sizeof(uint32_t));
        for (int i = 0; i < realoc_size; i++)
        {
            uint32_t rel_ind = 0;
            infile.read(reinterpret_cast<char*>(&rel_ind), sizeof(uint32_t));
            uint32_t rel_off = 0;
            infile.read(reinterpret_cast<char*>(&rel_off), sizeof(uint32_t));
            RelocationType rel_type;
            infile.read(reinterpret_cast<char*>(&rel_type), sizeof(RelocationType));
            std::shared_ptr<RelocationEntry> entry = std::make_shared<RelocationEntry>(rel_off, rel_type, rel_ind);
            sec->insertRealocationEntry(entry);
        }
        infile.read(reinterpret_cast<char*>(&size), sizeof(size));
        for (int i=0; i<size; i++)
        {
            Instruction ins;
            uint32_t size_of_instruction;
            infile.read(reinterpret_cast<char*>(&size_of_instruction), sizeof(size_of_instruction));
            for (int j=0; j<size_of_instruction; j++)
            {
                uint8_t word;
                infile.read(reinterpret_cast<char*>(&word), sizeof(word));
                ins.push_back(word);
            }
            sec->inserInstruction(ins);
        }
    }
    std::shared_ptr<SymbolTable> symbTab = linker->getSymbolTable();
    sec_tab->insert(std::shared_ptr<Section>(nullptr));
    size = 0;
    infile.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i =0 ; i < size ; i++)
    {
        name = "";
        str_size = 0;
        infile.read(reinterpret_cast<char*>(&str_size), sizeof(uint32_t));
        name.resize(str_size);
        infile.read(&name[0], str_size);
        std::string temp(name);
        uint32_t index;
        infile.read(reinterpret_cast<char*>(&index), sizeof(uint32_t));
        uint32_t offset;
        infile.read(reinterpret_cast<char*>(&offset), sizeof(uint32_t));
        bool local;
        infile.read(reinterpret_cast<char*>(&local), sizeof(bool));
        bool isSection;
        infile.read(reinterpret_cast<char*>(&isSection), sizeof(bool));
        std::string section_name;
        infile.read(reinterpret_cast<char*>(&str_size), sizeof(uint32_t));
        section_name.resize(str_size);
        infile.read(&section_name[0], str_size);
        std::shared_ptr<Section> sec_pointer;
        for (auto& sec : *sec_tab)
        {
            if (sec->getName() == section_name)
            {
                sec_pointer = sec;
            }
        }
        std::shared_ptr<Symbol> symb = std::make_shared<Symbol>(temp, sec_pointer, offset, isSection, local);
        symb->setIndex(index);
        symbTab->insert(symb);
    }

    for (auto sec : *sec_tab)
    {
        std::cout << sec->to_string() + '\n';
    }

    for (auto sec : *sec_tab)
    {
        auto relTable = sec->getTable();
        for (auto entry : relTable->getTable())
        {
            std::cout << "REL:\t" + sec->getName() + "\t" + entry->to_string() + "\n";
        }
    }
   for (auto symb : *symbTab)
    {
        std::cout << symb->to_string() + '\n';
    }
   printCode();
}

void Utility::printCode()
{
    std::shared_ptr<Linker> linker = Linker::getInstance();
    auto sec_tab = linker->getSectionTable();
    int i = 0;
    for (auto sec : *sec_tab)
    {
        for (auto& instruction : sec->getData())
        {
            std::stringstream ss;
            for (auto& word : instruction)
            {
                code.push_back(word);
                if (word <= 0xf) ss << std::hex << 0;
                ss << std::hex << static_cast<int>(word) << " ";
            }
            ss << "|";
            i++;
            if ((i % 3) == 0)
            {
                ss << "\n";
            }
            std::cout << ss.str();
        }
    }
    std::cout << std::endl;
}
