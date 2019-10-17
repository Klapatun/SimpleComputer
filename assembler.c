/*assembler.c*/
#include <stdio.h>
#include <stdint.h>
#include "mySimpleComputer.h"
#include "assembler.h"


static int asm_encoding(char* command);

int asm_processing(char* asm_file, char* obj_file){

	FILE *file_sa = fopen(asm_file, "rt");
	
	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	int addr = 0, oper = 0x0, value;
	char* cmd[7] = {0};
	char* asm_cmd[50] = {0};
	char* coment[30] = {0};

	int ram_dump[MEMSIZE] = {0};

	memcpy(ram_dump, ram, MEMSIZE);

	while(fgets(asm_cmd, 50, file_sa)) {
		sscanf(asm_cmd, "%x %s %x ;%s", &addr, cmd, &oper, coment);

		int num_cmd = asm_encoding(cmd);

		if(num_cmd) {
			//ram_tmp[addr] = cell;
			if (num_cmd != 0x01) {
				sc_commandEncode(num_cmd, oper, &value);
    		}
    		else {
    			value = oper;
    		}
    		
    		sc_memorySet(addr, value);
		}
		else {
			//Надо будет придумать, что тут будет делаться
			fprintf( stderr, "Сommand incorrect: %s! \n", asm_cmd);
			return -1;
		}
	}

	sc_memorySave(obj_file);
	memcpy(ram, ram_dump, MEMSIZE);

	fclose(file_sa);
/*
	uint16_t b[10] = {0};

	FILE* fb = fopen(obj_file, "rb");

	if (!fb) {
		fprintf( stderr, "File .o not open! \n");
		return -1;
	}

	fread(b, sizeof(uint16_t), 2, fb);

	fclose(fb);
*/
	return 0;
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
	else if (!strncmp(command, "=", 1)) {
		return VALUE;
	}
	else {
		return 0;
	}
}

