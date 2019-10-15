/*assembler.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "myBigChars.h"
#include "myTerm.h"
#include "read_key.h"
#include "ui.h"
#include "mySimpleComputer.h"
#include "u_io.h"
#include "read_key.h"
#include "alu_cu.h"

#include <string.h>
#include "assembler.h"


static int asm_encoding(char* command);

int asm_processing(void){
	
	int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "asm_processing: terminal error");
        close (term);
        return;
    }

    char buff[12] = {0};
    int addr = 0, oper = 0x0, value;
    char* com[7] = {0};
    sc_memoryGet(mem_ptr, &value);
    write(term, "Enter asm command > ", 20);
    read(term, buff, 12);
    sscanf(buff, "%x %s %x", &addr, &com, &oper);
    sc_commandEncode(asm_encoding(com), oper, &value);
    sc_memorySet(mem_ptr, value);
    clearInput();
    close(term);
}

static int asm_encoding(char* command) {

	if (!strncmp(command, "READ", 4)) {
		return READ;
	}
	else if (!strncmp(command, "WRITE", 5)) {
		return WRITE;
	}
	else if (!strncmp(command, "LOAD", 4)) {
		return LOAD;
	}
	else if (!strncmp(command, "STORE", 5)) {
		return STORE;
	}
	else if (!strncmp(command, "ADD", 3)) {
		return ADD;
	}
	else if (!strncmp(command, "SUB", 3)) {
		return SUB;
	}
	else if (!strncmp(command, "DIVIDE", 6)) {
		return DIVIDE;
	}
	else if (!strncmp(command, "MUL", 3)) {
		return MUL;
	}
	else if (!strncmp(command, "JUMP", 4)) {
		return JUMP;
	}
	else if (!strncmp(command, "JNEG", 4)) {
		return JNEG;
	}
	else if (!strncmp(command, "JZ", 2)) {
		return JZ;
	}
	else if (!strncmp(command, "HALT", 4)) {
		return HALT;
	}
	else if (!strncmp(command, "NOT", 3)) {
		return NOT;
	}
	else if (!strncmp(command, "AND", 3)) {
		return AND;
	}
	else if (!strncmp(command, "OR", 2)) {
		return OR;
	}
	else if (!strncmp(command, "XOR", 3)) {
		return XOR;
	}
	else if (!strncmp(command, "JNS", 3)) {
		return JNS;
	}
	else if (!strncmp(command, "JC", 2)) {
		return JC;
	}
	else if (!strncmp(command, "JNC", 3)) {
		return JNC;
	}
	else if (!strncmp(command, "JP", 2)) {
		return JP;
	}
	else if (!strncmp(command, "JNP", 3)) {
		return JNP;
	}
	else if (!strncmp(command, "CHL", 3)) {
		return CHL;
	}
	else if (!strncmp(command, "SHR", 3)) {
		return SHR;
	}
	else if (!strncmp(command, "RCL", 3)) {
		return RCL;
	}
	else if (!strncmp(command, "RCR", 3)) {
		return RCR;
	}
	else if (!strncmp(command, "NEG", 3)) {
		return NEG;
	}
	else if (!strncmp(command, "ADDC", 4)) {
		return ADDC;
	}
	else if (!strncmp(command, "SUBC", 4)) {
		return SUBC;
	}
	else if (!strncmp(command, "LOGLC", 5)) {
		return LOGLC;
	}
	else if (!strncmp(command, "LOGRC", 5)) {
		return LOGRC;
	}
	else if (!strncmp(command, "RCCL", 4)) {
		return RCCL;
	}
	else if (!strncmp(command, "RCCR", 4)) {
		return RCCR;
	}
	else if (!strncmp(command, "MOVA", 4)) {
		return MOVA;
	}
	else if (!strncmp(command, "MOVR", 4)) {
		return MOVR;
	}
	else if (!strncmp(command, "MOVCA", 5)) {
		return MOVCA;
	}
	else if (!strncmp(command, "MOVCR", 5)) {
		return MOVCR;
	}
	else {
		return 0;
	}
}

