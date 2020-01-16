#include "../h/operation.h"
#include "../h/instruction_code_table.h"

Operation::Operation(std::string &name, uint8_t code, uint8_t mode, uint8_t type, uint8_t inst_type, bool single_operand)
	: _name(name),
	  _code(code),
	  _modes(mode),
	  _type(type),
	  _inst_type(inst_type),
	  _single_operand(single_operand)
{
}

uint8_t Operation::getCode() const
{
	return _code;
}
std::string Operation::getName() const
{
	return _name;
}
uint8_t Operation::getModes() const
{
	return _modes;
}
uint8_t Operation::getTypes() const
{
	return _type;
}
uint8_t Operation::getInstType() const
{
	return _inst_type;
}
bool Operation::getIsSingleOperand() const
{
	return _single_operand;
}