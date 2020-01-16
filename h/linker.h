#ifndef LINKER_H
#define LINKER_H

#include <stdint.h>
#include <unordered_map>
#include "../h/asm/def.h"
#include "../h/asm/macros.h"
#include "../h/asm/section.h"
#include "../h/asm/symbol.h"
#include "../h/asm/relocation_entry.h"

class Linker
{
public:
	void connectSections();
	void resolveGlobal();
	void resolveAdress();
	void insertPlaceParameter(std::string, uint32_t);
public:
	std::shared_ptr<SectionTable> getSectionTable() const;
	std::shared_ptr<SymbolTable> getSymbolTable() const;
public:
	static std::shared_ptr<Linker> getInstance();

private:
	static std::weak_ptr<Linker> mInstance;

private:
	Linker();
	std::shared_ptr<SectionTable> _sectionTable;
	std::shared_ptr<SymbolTable> _symbolTable;
	std::unordered_map <std::string, uint16_t> place_parameter;
	bool all_adr_correct;
	uint16_t location_pointer;
};


#endif // !LINKER_H