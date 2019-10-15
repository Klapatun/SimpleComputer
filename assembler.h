/*Assembler*/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

//#include <stdio.h>


enum
{
	READ = 0X10,
	WRITE,

	LOAD = 0X20,
	STORE,

	ADD	=0X30,
	SUB,
	DIVIDE,
	MUL,

	JUMP = 0X40,
	JNEG,
	JZ,
	HALT,

	NOT = 0X51,
	AND,
	OR,
	XOR,
	JNS,
	JC,
	JNC,
	JP,
	JNP,
	CHL = 0X60,
	SHR,
	RCL,
	RCR,
	NEG,
	ADDC,
	SUBC,
	LOGLC,
	LOGRC,
	RCCL,
	RCCR = 0X70,
	MOVA,
	MOVR,
	MOVCA,
	MOVCR
};

int asm_processing(void);


#endif /*ASSEMBLER_H*/
