#include "../h/cpu.h"
#include <vector>

std::shared_ptr<Cpu> Cpu::mInstance = std::shared_ptr<Cpu>(nullptr);

Cpu::Cpu()
{
	cpuInit();
}

std::shared_ptr<Cpu> Cpu::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = std::make_shared<Cpu>();
	}
	return mInstance;
}

void Cpu::cpuInit()
{
	is_halt = false;
	AB = 0;
	BB = 0;
	IVTP = 0;
	IMR = 0;
	AB = BB = 0;
	for (int i = 0; i < 8; i++)
	{
		r[7] = 0;
	}
	psw = 0;
}

void Cpu::cpuWorkLoop()
{
	jumpToResetHandler();
	while (!is_halt)
	{
		instructionFeatch();
		addrPhase();
		execPhase();
		interruptPhase();
	}
}

void Cpu::setActiveMemory(std::shared_ptr<Memory> m)
{
	mem = m;
}

void Cpu::interruptRequest(uint8_t intr, bool& ack, uint8_t entry)
{
	if (entry = 1)
	{
		_op_code = 0x01;
	}
}

void Cpu::jumpToResetHandler()
{
	auto a1 = mem->readLocation(0);
	auto a2 = mem->readLocation(1);
	r[7] = a2 << 8 | a1;
}

void Cpu::instructionFeatch()
{
	//TODO: SHARED RESOURCE ACCESS
	instReg[0] = mem->readLocation(r[7]++);
	_op_code = instReg[0] >> 3;
	if (_op_code == 0x1 || _op_code == 0x18 || _op_code == 0x19)
		return;
	_is_byte_size = instReg[0] & 0x02;
	//TODO: SHARED RESOURCE ACCESS
	instReg[1] = mem->readLocation(r[7]++);
	_adr1 = instReg[1] >> 5;
	_reg1 = (instReg[1] & 0x1E) >> 1;
	_low_high1 = instReg[1] & 0x1;

	_is_single_op = InstructionCodeTable::isSignleOperand(_op_code);

	auto op_size = 1;
	switch (_adr1)
	{
	case REGDIR:
	case REGIND:
		op_size = 1;
		break;
	case REGINDPOM:
		op_size = 2;
		break;
	case REGINDPOM16:
		op_size = 3;
		break;
	case IMMED:
		if (_is_byte_size)
		{
			op_size = 2;
		}
		else
		{
			op_size = 3;
		}
		break;
	case MEMDIR:
		if (_is_byte_size)
		{
			op_size = 2;
		}
		else
		{
			op_size = 3;
		}
		break;
	}
	if (op_size > 1)
	{
		//TODO: SHARED RESOURCE ACCESS
		instReg[2] = mem->readLocation(r[7]++);
		_op1_low = instReg[2];
	}
	if (op_size > 2)
	{
		//TODO: SHARED RESOURCE ACCESS
		instReg[3] = mem->readLocation(r[7]++);
		_op1_high = instReg[3];
	}
	if (!_is_single_op)
	{
		//TODO: SHARED RESOURCE ACCESS
		instReg[4] = mem->readLocation(r[7]++);

		_adr2 = instReg[4] >> 5;
		_reg2 = (instReg[4] & 0x1E) >> 1;
		_low_high2 = instReg[4] & 0x1;
		op_size = 1;
		switch (_adr2)
		{
		case REGDIR:
		case REGIND:
			op_size = 1;
			break;
		case REGINDPOM:
			op_size = 2;
			break;
		case REGINDPOM16:
			op_size = 3;
			break;
		case IMMED:
			if (_is_byte_size)
			{
				op_size = 2;
			}
			else
			{
				op_size = 3;
			}
			break;
		case MEMDIR:
			if (_is_byte_size)
			{
				op_size = 2;
			}
			else
			{
				op_size = 3;
			}
			break;
		}
		if (op_size > 1)
		{
			//TODO: SHARED RESOURCE ACCESS
			instReg[5] = mem->readLocation(r[7]++);
			_op2_low = instReg[5];
		}
		if (op_size > 2)
		{
			//TODO: SHARED RESOURCE ACCESS
			instReg[6] = mem->readLocation(r[7]++);
			_op2_high = instReg[6];
		}
	}
}

void Cpu::addrPhase()
{
	uint16_t op;
	switch (_adr1)
	{
	case REGDIR:
		AB = r[_reg1];
		break;
	case REGIND:
		AB = mem->readLocation(r[_reg1]);
		break;
	case REGINDPOM:
		AB = mem->readLocation(r[_reg1] + _op1_low);
		break;
		//TODO implementiraj u predmetnom programu odvojeno generisanje regindpom16
	case REGINDPOM16:
		op = _op1_high << 8 | _op1_low;
		AB = mem->readLocation(r[_reg1] + op);
		break;
	case IMMED:
		//TODO proveri da li u predmetnom programu upisujes ovo pravilno
		if (_is_byte_size)
		{
			AB = _op1_low;
		}
		else
		{
			AB = _op1_high << 8 | _op1_low;
		}
		break;
	case MEMDIR:
		op = _op1_high << 8 | _op1_low;
		AB = mem->readLocation(op);
		break;
	}

	switch (_adr2)
	{
	case REGDIR:
		BB = r[_reg2];
		break;
	case REGIND:
		BB = mem->readLocation(r[_reg2]);
		break;
	case REGINDPOM:
		BB = mem->readLocation(r[_reg2] + _op2_low);
		break;
	case REGINDPOM16:
		op = _op2_high << 8 | _op2_low;
		BB = mem->readLocation(r[_reg2] + op);
		break;
	case IMMED:
		if (_is_byte_size)
		{
			BB = _op2_low;
		}
		else
		{
			BB = _op2_high << 8 | _op2_low;
		}
		break;
	case MEMDIR:
		op = _op2_high << 8 | _op2_low;
		BB = mem->readLocation(op);
		break;
	}
}

void Cpu::execPhase()
{
	uint16_t old;
	uint16_t temp;
	uint8_t first;
	uint8_t second;
	switch (_op_code)
	{
	case HALT:
		is_halt = true;
		break;
	case XCHG:
		//TODO
		temp = AB;
		AB = BB;
		BB = temp;
		break;
	case INT:
		first = (uint8_t)((psw & 0xFF00) >> 8);
		second = (uint8_t)(psw & 0x00FF);
		mem->writeToLocation(r[6]++, second);
		mem->writeToLocation(r[6]++, first);
		r[7] = mem->readLocation((AB % 8) * 2) | (mem->readLocation((AB + 1 % 8) * 2) << 8);
		break;
	case MOV:
		AB = BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case ADD:
		old = AB;
		AB += BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		if ((old & 0x8000) ^ (AB & 0x8000)) psw |= PSW_O;
		if (old > 0xFFFF - BB) psw |= PSW_C;
		break;
	case SUB:
		old = AB;
		AB -= BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		if ((old & 0x8000) ^ (AB & 0x8000)) psw |= PSW_O;
		if (old > 0xFFFF - BB) psw |= PSW_C;
		break;
	case MUL:
		AB *= BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case DIV:
		if (BB != 0)
		{
			AB /= BB;
		}
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case CMP:
		temp = AB - BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		if ((temp & 0x8000) ^ (AB & 0x8000)) psw |= PSW_O;
		if (AB > 0xFFFF - BB) psw |= PSW_C;
		break;
	case NOT:
		AB = ~AB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case AND:
		AB &= BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case OR:
		AB |= BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case XOR:
		AB ^= BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case TEST:
		temp = AB & BB;
		if (temp == 0) psw |= PSW_Z;
		if (temp < 0) psw |= PSW_N;
		break;
	case SHL:
		AB = AB << BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case SHR:
		AB = AB >> BB;
		if (AB == 0) psw |= PSW_Z;
		if (AB < 0) psw |= PSW_N;
		break;
	case PUSH:
		first = (uint8_t)((AB & 0xFF00) >> 8);
		second = (uint8_t)(AB & 0x00FF);
		mem->writeToLocation(r[6]++, second);
		mem->writeToLocation(r[6]++, first);
		break;
	case POP:
		first = mem->readLocation(r[6]--);
		second= mem->readLocation(r[6]--);
		AB = (first << 8) | second;
		break;
	case JMP:
		r[7] = (_op1_high << 8) | _op1_low;
		break;
	case JEQ:
		if (PSW_Z)
		{
			r[7] = (_op1_high << 8) | _op1_low;
		}
		break;
	case JNE:
		if (!PSW_Z)
		{
			r[7] = (_op1_high << 8) | _op1_low;
		}
		break;
	case JGT:
		if (!PSW_N && !PSW_N)
		{
			r[7] = (_op1_high << 8) | _op1_low;
		}
		break;
	case CALL:
		first = (uint8_t)((r[7] & 0xFF00) >> 8);
		second = (uint8_t)(r[7] & 0x00FF);
		mem->writeToLocation(r[6]++, second);
		mem->writeToLocation(r[6]++, first);
		r[7] = (_op1_high << 8) | _op1_low;
		break;
	case RET:
		first = mem->readLocation(r[6]--);
		second = mem->readLocation(r[6]--);
		r[7] = (first << 8) | second;
		break;
	case IRET:
		first = mem->readLocation(r[6]--);
		second = mem->readLocation(r[6]--);
		psw = (first << 8) | second;

		first = mem->readLocation(r[6]--);
		second = mem->readLocation(r[6]--);
		r[7] = (first << 8) | second;
		break;
	}
}

void Cpu::interruptPhase()
{
}
