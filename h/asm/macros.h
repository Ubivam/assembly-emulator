#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <vector>
#include "def.h"

#define PRINT(a) std::cout<<a<<std::endl
#define READ(a) std::cin>>a

//Tipovi adresiranja
#define IMMED 0x0
#define REGDIR 0x1
#define REGIND 0x2
#define REGINDPOM 0x3
#define REGINDPOM16 0x4
#define MEMDIR 0x5

//Pomocni tipovi adresiranja
#define IMMED_SYM 0x6
#define REGINDPOM_SYM 0x7
#define PC_RELATIVE_SYM 0x8
#define ABS_SYM 0x9

//Tipovi instrukcija
#define ALU_INSTRUCTION 0
#define JMP_INSTRUCTION 1
#define LOAD_STORE_INSTRUCTION 2
#define STACK_INSTRUCTION 3
#define CONTROL_INSTRUCTION 4

//Pomocni flegovi za simbole i sekcije
#define LOCAL_SYMBOL true
#define GLOBAL_SYMBOL false
#define IS_SECTION true
#define NOT_SECTION false

//Programska statusna rec
#define PSW_Z 0x1
#define PSW_O 0x1 << 1
#define PSW_C 0x1 << 2
#define PSW_N 0x1 << 3
#define PSW_Tr 0x1 << 13
#define PSW_Tl 0x1 << 14
#define PSW_I 0x1 << 15

//Instrukcije
#define HALT 0x01
#define XCHG 0x02
#define INT 0x03
#define MOV 0x04
#define ADD 0x05
#define SUB 0x06
#define MUL 0x07
#define DIV 0x08
#define CMP 0x09
#define NOT 0x0A
#define AND 0x0B
#define OR 0x0C
#define XOR 0x0D
#define TEST 0x0E
#define SHL 0x0F
#define SHR 0x10
#define PUSH 0x11
#define POP 0x12
#define JMP 0x13
#define JEQ 0x14
#define JNE 0x15
#define JGT 0x16
#define CALL 0x17
#define RET 0x18
#define IRET 0x19



//Apsolutna adresa za realokaciju simbola stavlja 32 bitnu adresu simbola u memorijsku lokaciju
#define R_386_32 true
//PC Relativna adresa za realokaciju simbola stavlja 32 bitnu realtivnu adresu simbola u memorijsku lokaciju
#define R_386_PC32 false

#endif