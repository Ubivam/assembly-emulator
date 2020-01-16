#include "../h/asm/section.h"
#include <vector>
#include <sstream>
#include <iomanip>

Section::Section(std::string name, uint32_t index, uint32_t locCounter)
    : _stringName(name),
      _index(index),
      _beginLocCounter(locCounter),
      _endLocCouter(locCounter),
      _locCounter(locCounter)
{
    _table = std::make_shared<RelocationTable>();
}
void Section::incLocationCounter(uint32_t amount)
{
    _locCounter += amount;
}
void Section::resetLocationCounter()
{
    _locCounter = 0;
}
void Section::inserInstruction(Instruction inst)
{
    _data.push_back(inst);
}
void Section::saveAndResetLocationCounter()
{
    _endLocCouter = _locCounter;
    _locCounter = _beginLocCounter;
}
void Section::insertRealocationEntry(std::shared_ptr<RelocationEntry>& entry)
{
    _table->insert(entry);
}
std::string Section::getName() const
{
    return _stringName;
}
uint32_t Section::getLocationCounter() const
{
    return _locCounter;
}
uint32_t Section::getBeginLocationCounter() const
{
    return _beginLocCounter;
}
uint32_t Section::getEndLocationCounter() const
{
    return _endLocCouter;
}
Code Section::getData() const
{
    return _data;
}
uint32_t Section::getIndex() const
{
    return _index;
}

std::string Section::to_string() const
{
    std::stringstream stream;
    stream << "0x0" << std::uppercase << std::hex << _beginLocCounter;
    auto begin = stream.str();

    stream.str(std::string());

    stream << "0x0" << std::uppercase << std::hex << _endLocCouter - _beginLocCounter;
    auto end = stream.str();

     stream.str(std::string());

     stream << "0x0" <<  std::uppercase << std::hex << _locCounter;
     auto loc = stream.str();

    return "SEG:\t" + std::to_string(_index) + "\t" + _stringName + "\t" + loc + "\t" + begin + "\t" + end;
}
std::string Section::to_string_data() const
{
    return std::string();
}
std::string Section::to_string_table() const
{
    return std::string();
}
std::shared_ptr<RelocationTable> Section::getTable()
{
    return _table;
}

void Section::setName(std::string nameS)
{
    _stringName = nameS;
}

void Section::setBegin(uint32_t begin)
{
    _beginLocCounter = begin;
}

void Section::setEnd(uint32_t end)
{
    _endLocCouter = end;
}

void Section::setIndex(uint32_t ind)
{
    _index = ind;
}

void Section::setTable(std::shared_ptr<RelocationTable> tab)
{
    _table = tab;
}

void Section::insertData(Code& cd)
{
    _data = cd;
}
