#ifndef OPERATIONAL_CODE_TABELE_H
#define OPERATIONAL_CODE_TABELE_H
#include "mnemonic.h"
#include "section.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

class OperationalCodeTable
{
private:
    static std::unordered_map<std::string, std::shared_ptr<Operation>> _map;

public:
    static void init();

    static bool find(std::string op);
    static std::shared_ptr<Operation> get(std::string op);

    static uint8_t getAddressMode(std::string &operation);
    static uint8_t getAddressModes(std::string operation);
    static bool isByteSize(std::vector<std::string> &instrction);
    static std::vector<std::string> getInstruction(std::string operation, std::vector<std::string> line);

    static uint8_t checkInstruction(std::string op, std::vector<std::string> line);
    static Instruction constructInstruction(std::vector<std::string> inst);
};
#endif