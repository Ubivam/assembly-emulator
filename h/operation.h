#ifndef OPERATION_H
#define OPERATION_H

#include <unordered_map>
#include <string>
#include "asm/def.h"
#include "asm/macros.h"

class Operation
{
public:
	Operation(std::string &name, uint8_t code, uint8_t modes, uint8_t type, uint8_t _inst_type = 0, bool singleOperand = false);

public:
	bool isValid(uint8_t mode) const;

public:
	uint8_t getCode() const;
	std::string getName() const;
	uint8_t getModes() const;
	uint8_t getTypes() const;
	uint8_t getInstType() const;
	bool getIsSingleOperand() const;

private:
	std::string _name;
	uint8_t _code;
	uint8_t _modes;
	uint8_t _type;
	uint8_t _inst_type;
	bool _single_operand;
};
#endif