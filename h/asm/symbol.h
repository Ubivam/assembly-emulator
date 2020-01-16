#ifndef SYMBOL_H
#define SYMBOL_H
#include "macros.h"
#include "def.h"
#include "section.h"
class Symbol
{
public:
	Symbol();
    Symbol(std::string label, std::shared_ptr<Section> section, uint16_t offset, bool isSection = false, bool isLocal = true);

public: //SETTERS
    void setLocal(bool local);
    void setIndex(uint32_t index);
    void setOffset(uint32_t _offset);

public: //GETTERS
    std::string getName() const;
    uint32_t getIndex() const;
    uint32_t getOffset() const;
    bool isLocal() const;
    bool isSection() const;
    std::shared_ptr<Section> getSection() const;

    std::string to_string() const;

public:
    friend std::ostream &operator<<(std::ostream &os, const Symbol &symbol);

private:
    std::string _name;
    uint32_t _index;
    uint32_t _offset;
    bool _local;
    bool _isSection;
    std::string _sectionName;
    std::shared_ptr<Section> _section;
};
#endif