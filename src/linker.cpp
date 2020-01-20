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

void Linker::resolveAdress()
{
	for (auto& sec : _sectionTable->getTable())
	{
		std::unordered_map<std::string, uint16_t>::const_iterator got = place_parameter.find(sec->getName());
		if (got == place_parameter.end())
		{
			sec->setBegin(location_pointer);
			sec->setEnd(sec->getEndLocationCounter() + sec->getBeginLocationCounter());
			location_pointer += sec->getEndLocationCounter();
		}
		else
		{
			sec->setBegin(got->second);
			sec->setEnd(sec->getEndLocationCounter() + sec->getBeginLocationCounter());
		}

	}
	for (auto& sec : _sectionTable->getTable())
	{
		uint16_t adr;
		adr = sec->getBeginLocationCounter();
		auto table = sec->getTable();
		auto current_code = sec->getData();

		for(auto rel : table->getTable())
		{
			std::shared_ptr<Symbol> symbol = std::shared_ptr<Symbol>(nullptr);
			std::shared_ptr<Symbol> global_symbol = std::shared_ptr<Symbol>(nullptr);
			for (auto s : *_symbolTable)
			{
				if (s->getIndex() == rel->getIndex()) {
					if (s->getSection() == sec)
					{
						symbol = s;
					}
				}
			}
			if (!symbol)
			{
				all_adr_correct = false;
				return;
			}
			if (!symbol->isLocal() && !symbol->getSection()) 
			{
				global_symbol = resolveGlobalSymbol(symbol);
				if (!global_symbol)
				{
					all_adr_correct = false;
					return;
				}
			}
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
						//GLOBAL SYMBOL ABS
					case 0x0000:
						first = (uint8_t)((symbol->getSection()->getBeginLocationCounter() + symbol->getOffset() & 0xFF00) >> 8);
						second = (uint8_t)(symbol->getSection()->getBeginLocationCounter() + symbol->getOffset() & 0x00FF);
						break;
						//LOCAL SYMBOL REL
					case 0x0002:
						if (!symbol->isLocal())
						{
							all_adr_correct = false;
							return;
						}
						first = (uint8_t)((symbol->getOffset() - loc_cnt & 0xFF00) >> 8);
						second = (uint8_t)(symbol->getOffset() - loc_cnt & 0x00FF);
						break;

						//GLOBAL SYMBOL REL
					case 0xFFFC:
						if (symbol->isLocal())
						{
							first = (uint8_t)(((symbol->getSection()->getBeginLocationCounter() + symbol->getOffset()) - loc_cnt & 0xFF00) >> 8);
							second = (uint8_t)((symbol->getSection()->getBeginLocationCounter() + symbol->getOffset()) - loc_cnt & 0x00FF);
							all_adr_correct = false;
							return;
						}
						break;
						//LOCAL SYMBOL ABS
					default:
						if (!symbol->isLocal())
						{
							all_adr_correct = false;
							return;
						}
						first = (uint8_t)((adr + reloc_mem_part & 0xFF00) >> 8);
						second = (uint8_t)(adr + reloc_mem_part & 0x00FF);
						break;
					}
					
					current_code[ins_ind][second_layer_index] = second;
					current_code[ins_ind][second_layer_index + 1] = first;
					loc_cnt += second_layer_index;
				}
				else
				{
					loc_cnt += current_code[ins_ind].size();
					ins_ind++;
				}
			}
		}
		sec->insertData(current_code);
	}
}

void Linker::insertPlaceParameter(std::string sec, uint32_t adr)
{
	place_parameter.insert(std::make_pair<>(sec, adr));
}

bool Linker::areAllAdressCorrect()
{
	return all_adr_correct;
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

std::shared_ptr<Symbol> Linker::resolveGlobalSymbol(std::shared_ptr<Symbol> s)
{
	for (auto symbol : *_symbolTable)
	{
		if (symbol != s && (symbol->getName() == s->getName()))
		{
			return symbol;
		}
	}
	return nullptr;
}

Linker::Linker()
{
	all_adr_correct = true;
	_sectionTable = std::make_shared<SectionTable>();
	_symbolTable = std::make_shared<SymbolTable>();
	location_pointer = 1000;
}
