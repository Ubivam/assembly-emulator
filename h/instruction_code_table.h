#ifndef INSTRUCTION_CODE_TABLE_H
#define INSTRUCTION_CODE_TABLE_H

#include <stdint.h>
#include <memory>
#include <unordered_map>
#include "operation.h"
#include "asm/def.h"
#include "asm/macros.h"

class InstructionCodeTable {

private:
    static std::unordered_map <uint8_t, std::shared_ptr<Operation>> _map;

public:
    static void init();

    static std::shared_ptr<Operation> get (uint8_t);

    static uint8_t getAddressMode(uint8_t);

    static bool isSignleOperand(uint8_t);
};
#endif