#include "..\h\linker.h"

std::weak_ptr<Linker> Linker::mInstance;

void Linker::connectSections()
{
	for (auto sec1 : *_sectionTable)
	{
		auto current_end_section = sec1->getEndLocationCounter();
		for (auto sec2 : *_sectionTable)
		{
			if (sec1 != sec2)
			{

				if (sec1->getName() == sec2->getName())
				{
					sec2->setBegin(current_end_section);
					current_end_section = sec2->getEndLocationCounter();
				}
			}
		}
	}
}

void Linker::resolveGlobal()
{

}

void Linker::resolveAdress()
{
	for (auto& sec : *_sectionTable)
	{
		uint16_t adr;
		std::unordered_map<std::string, uint16_t>::const_iterator got = place_parameter.find(sec->getName());
		if (got == place_parameter.end())
		{
			adr = location_pointer;
		}
		else
		{
			adr = got->second;
		}
		auto table = sec->getTable();
		auto current_code = sec->getData();

		for(auto rel : *table)
		{
			auto off = rel->getOffset();
			int loc_cnt = 0;
			uint32_t ins_ind=0;
			while (loc_cnt < off)
			{
				if ((current_code[ins_ind].size() + loc_cnt) > off)
				{
					uint32_t second_layer_index = off - loc_cnt;
					uint16_t reloc_mem_part = current_code[ins_ind][second_layer_index + 1] << 8 | current_code[ins_ind][second_layer_index];
					uint8_t first = 0;
					uint8_t second = 0;
					switch (reloc_mem_part)
					{
						//GLOBAL SYMBOL REL
					case 0x0000:
						break;

						//LOCAL SYMBOL ABS
					case 0x0004:
						first = (uint8_t)((adr + off & 0xFF00) >> 8);
						second = (uint8_t)(adr + off & 0x00FF);
						break;

						//GLOBAL SYMBOL ABS
					case 0xFFFC:
						break;

						//LOCAL SYMBOL REL
					default:
						auto pc_relative_adr = reloc_mem_part - off;
						first = (uint8_t)((pc_relative_adr & 0xFF00) >> 8);
						second = (uint8_t)(pc_relative_adr & 0x00FF);
						break;
					}
					
					current_code[ins_ind][second_layer_index] = second;
					current_code[ins_ind][second_layer_index + 1] = first;
					loc_cnt += 2;
				}
				else
				{
					loc_cnt += current_code[ins_ind].size();
					ins_ind++;
				}
			}
		}
		sec->insertData(current_code);
		location_pointer += sec->getEndLocationCounter();
	}
}

void Linker::insertPlaceParameter(std::string sec, uint32_t adr)
{
	place_parameter.insert(std::make_pair<>(sec, adr));
}

std::shared_ptr<SectionTable> Linker::getSectionTable() const
{
	return _sectionTable;
}

std::shared_ptr<SymbolTable> Linker::getSymbolTable() const
{
	return _symbolTable;
}

std::shared_ptr<Linker> Linker::getInstance()
{
	if (auto ptr = mInstance.lock())
	{
		return ptr;
	}
	auto ptr = std::shared_ptr<Linker>(new Linker());
	mInstance = ptr;
	return ptr;
}

Linker::Linker()
{
	_sectionTable = std::make_shared<SectionTable>();
	_symbolTable = std::make_shared<SymbolTable>();
	location_pointer = 0x00FF;
}
