#include "../h/instruction_code_table.h"

std::unordered_map <uint8_t, std::shared_ptr<Operation>> InstructionCodeTable::_map;

void InstructionCodeTable::init()
{
    std::string instruction = "halt";
    _map.insert(std::make_pair<>(0x01, std::make_shared<Operation>(instruction, 0x01, REGDIR, CONTROL_INSTRUCTION, 0, true)));
    instruction = "xchg";
    _map.insert(std::make_pair<>(0x02, std::make_shared<Operation>(instruction, 0x02, REGDIR, ALU_INSTRUCTION)));
    instruction = "int";
    _map.insert(std::make_pair<>(0x03, std::make_shared<Operation>(instruction, 0x03, REGDIR, JMP_INSTRUCTION, 0, true)));
    instruction = "mov";
    _map.insert(std::make_pair<>(0x04, std::make_shared<Operation>(instruction, 0x04, MEMDIR | REGDIR | REGINDPOM | REGINDPOM16, LOAD_STORE_INSTRUCTION)));

    //ALU INSTRUCTIONS
    instruction = "add";
    _map.insert(std::make_pair<>(0x05, std::make_shared<Operation>(instruction, 0x05, REGDIR, ALU_INSTRUCTION)));
    instruction = "sub";
    _map.insert(std::make_pair<>(0x06, std::make_shared<Operation>(instruction, 0x06, REGDIR, ALU_INSTRUCTION)));
    instruction = "mul";
    _map.insert(std::make_pair<>(0x07, std::make_shared<Operation>(instruction, 0x07, REGDIR, ALU_INSTRUCTION)));
    instruction = "div";
    _map.insert(std::make_pair<>(0x08, std::make_shared<Operation>(instruction, 0x08, REGDIR, ALU_INSTRUCTION)));
    instruction = "cmp";
    _map.insert(std::make_pair<>(0x09, std::make_shared<Operation>(instruction, 0x09, REGDIR, ALU_INSTRUCTION)));
    instruction = "not";
    _map.insert(std::make_pair<>(0x0A, std::make_shared<Operation>(instruction, 0x0A, REGDIR, ALU_INSTRUCTION, 0, true)));
    instruction = "and";
    _map.insert(std::make_pair<>(0x0B, std::make_shared<Operation>(instruction, 0x0B, REGDIR, ALU_INSTRUCTION)));
    instruction = "or";
    _map.insert(std::make_pair<>(0x0C, std::make_shared<Operation>(instruction, 0x0C, REGDIR, ALU_INSTRUCTION)));
    instruction = "xor";
    _map.insert(std::make_pair<>(0x0D, std::make_shared<Operation>(instruction, 0x0D, REGDIR, ALU_INSTRUCTION)));
    instruction = "test";
    _map.insert(std::make_pair<>(0x0E, std::make_shared<Operation>(instruction, 0x0E, REGDIR, ALU_INSTRUCTION)));
    instruction = "shl";
    _map.insert(std::make_pair<>(0x0F, std::make_shared<Operation>(instruction, 0x0F, REGDIR, ALU_INSTRUCTION)));
    instruction = "shr";
    _map.insert(std::make_pair<>(0x10, std::make_shared<Operation>(instruction, 0x10, REGDIR, ALU_INSTRUCTION)));
    //Stack Instructions
    instruction = "push";
    _map.insert(std::make_pair<>(0x11, std::make_shared<Operation>(instruction, 0x11, REGDIR, STACK_INSTRUCTION, 0, true)));
    instruction = "pop";
    _map.insert(std::make_pair<>(0x12, std::make_shared<Operation>(instruction, 0x12, REGDIR, STACK_INSTRUCTION, 0, true)));

    //Jump Instructions

    instruction = "jmp";
    _map.insert(std::make_pair<>(0x13, std::make_shared<Operation>(instruction, 0x13, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "jeq";
    _map.insert(std::make_pair<>(0x14, std::make_shared<Operation>(instruction, 0x14, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "jne";
    _map.insert(std::make_pair<>(0x15, std::make_shared<Operation>(instruction, 0x15, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "jgt";
    _map.insert(std::make_pair<>(0x16, std::make_shared<Operation>(instruction, 0x16, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "call";
    _map.insert(std::make_pair<>(0x17, std::make_shared<Operation>(instruction, 0x17, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "ret";
    _map.insert(std::make_pair<>(0x18, std::make_shared<Operation>(instruction, 0x18, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "iret";
    _map.insert(std::make_pair<>(0x19, std::make_shared<Operation>(instruction, 0x19, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    return;
}



bool InstructionCodeTable::isSignleOperand(uint8_t code)
{
    return _map.find(code)->second->getIsSingleOperand();
}
