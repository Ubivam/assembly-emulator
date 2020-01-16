#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <memory>
#include "memory.h"
#include "asm/macros.h"
#include "asm/def.h"
#include "instruction_code_table.h"

class Cpu
{
public:
	Cpu();
	static std::shared_ptr<Cpu> getInstance();
public:
	void cpuInit();
	void cpuWorkLoop();
	void setActiveMemory(std::shared_ptr<Memory> m);

public:
	void instructionFeatch();
	void addrPhase();
	void execPhase();
	void interruptPhase();

private:
	uint8_t instReg[7];
	uint16_t r[8];
	uint16_t AB, BB;
	uint16_t psw;
	uint16_t IVTP;
	uint16_t IMR;
	bool is_halt;
private:

	static std::shared_ptr<Cpu> mInstance;
	std::shared_ptr<Memory> mem;
private:
	uint8_t _op1_low, _op1_high, _op2_low, _op2_high;
	uint8_t _adr1, _adr2;
	uint8_t _reg1, _reg2;
	bool _low_high1, _low_high2;
	bool _is_byte_size;
	bool _is_single_op;
	uint8_t _op_code;

	//Interrupts
	bool intr_req1;
	bool intr_req2;
	bool intr_req3;
	bool intr_ack1;
	bool intr_ack2;
	bool intr_ack3;
};


#endif