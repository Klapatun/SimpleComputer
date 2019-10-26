/*simple_basic.c*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "simple_basic.h"
#include "variables.h"
#include "opz.h"

//static int rem_basic(char* comment, const char* asm_file_name);
static int input_basic(char value_name, const char* asm_file_name);
static int output_basic(char value_name, const char* asm_file_name);
static int goto_basic(int str_addr, const char* asm_file_name);
static int if_goto_basic(char* condition_str, int addr, const char* asm_file_name);
static int end_basic(const char* asm_file_name);

//static int value_cheker(char value_name);
static int condition_check(char* condition_str, int* operand_1, int* operand_2, char* token);




//static int last_stack_idx;
static int num_str;


int basic_processing (const char* basic_file, const char* asm_file) {

	char basic_cmd[50] = {0};
	char command[7] = {0};
	char value[40] = {0}; 

	//last_stack_idx = 0x63;

	FILE *file_bas = fopen(basic_file, "rt");

	if (!file_bas) {
		fprintf( stderr, "File .sbas not open! \n");
		return -1;
	}

	FILE *file_sa = fopen(asm_file, "wt");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	fclose(file_sa);

	while(fgets(basic_cmd, 50, file_bas)) {
		sscanf(basic_cmd, "%s ", command);
		
		if (!strncmp(command, "REM", 3)) {
			//rem_basic(&basic_cmd[4], asm_file);
			num_str++;
		}
		else if (!strncmp(command, "INPUT", 5)) {
			
			char tmp[30] = {0};
			sscanf(&basic_cmd[6], "%s ", tmp);

			int tmp_int = strlen(tmp);
			if(tmp_int != 1) {
				fprintf( stderr, "Error: line number %d! \n", num_str); //ИСПРАВИТЬ num_str не то
				return -1;
			}

			input_basic(tmp[0], asm_file);

			num_str++;
		}
		else if (!strncmp(command, "OUTPUT", 6)) {

			char tmp[30] = {0};
			sscanf(&basic_cmd[7], "%s ", tmp);

			int tmp_int = strlen(tmp);
			if(tmp_int != 1) {
				fprintf( stderr, "Error: line number %d! \n", num_str); //ИСПРАВИТЬ num_str не то
				return -1;
			}

			output_basic(tmp[0], asm_file);

			num_str++;

		}
		else if (!strncmp(command, "GOTO", 4)) {

			char tmp_char[10] = {0};
			sscanf(&basic_cmd[5], "%s\n", tmp_char);
			goto_basic(atoi(tmp_char), asm_file);
			num_str++;
		}
		else if (!strncmp(command, "IF", 2)) {
			char condition_str[20] = {0};
			char addr_str[3] = {0};

			sscanf(basic_cmd, "IF %s GOTO %s\n", condition_str, addr_str);

			int addr = atoi(addr_str);
			//char* oper_1[3] = {0};
			//char* oper_2[3] = {0};
			//char* token[3] = {0};

			if_goto_basic(condition_str, addr, asm_file);
			

			num_str++;
		}
		else if (!strncmp(command, "LET", 3)) {
			char exp[50] = {0};

			int num = opz(&basic_cmd[4], exp);



			num_str++;
		}
		else if (!strncmp(command, "END", 3)) {
			end_basic(asm_file);
			num_str++;
		}
		else {

		}

		//int value_size = (int)strlen(&basic_cmd[command_size]);
		//memcpy(value, &basic_cmd[command_size], strlen(value_size));

	}

	fclose(file_bas);
}


/*
static int rem_basic(char* comment, char* asm_file_name) {

	FILE *file_sa = fopen(asm_file_name, "at");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	char asm_comment[30] = {0};

	sprintf(asm_comment, ";%s", comment);
	int num = strlen(asm_comment);
	int num_2 = fwrite(asm_comment, sizeof(int), num, file_sa);

	fclose(file_sa);
	return 0;
}
*/
static int input_basic(char value_name, const char* asm_file_name) {
	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	int addr_val = value_cheker(value_name);
	if (addr_val == 0) {
		fclose(file_sa);
		return -1;
	}

	char asm_cmd[30] = {0};

	//if(num_str > 0x0F) {
		sprintf(asm_cmd, "%x %s %x\n", num_str, "READ", addr_val);
	/*}
	else {
		sprintf(asm_cmd, "0%x %s %x\n", num_str, "READ", addr_val);
	}*/

	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);


	fclose(file_sa);
	return 0;
}

static int output_basic(char value_name, const char* asm_file_name) {
	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	int addr_val = value_cheker(value_name);
	if (addr_val == 0) {
		fclose(file_sa);
		return -1;
	}

	char asm_cmd[30] = {0};

	//if(num_str > 0x0F) {
		sprintf(asm_cmd, "%x %s %x\n", num_str, "WRITE", addr_val);
	/*}
	else {
		sprintf(asm_cmd, "0%x %s %x\n", num_str, "WRITE", addr_val);
	}*/

	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);

	fclose(file_sa);
	return 0;
}

static int goto_basic(int str_addr, const char* asm_file_name) {
	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	char asm_cmd[30] = {0};

	//if (num_str > 0x0F) {
		sprintf(asm_cmd, "%x %s %x\n", num_str, "JUMP", str_addr);
	/*}
	else{
		sprintf(asm_cmd, "0%x %s %x\n", num_str, "JUMP", str_addr);
	}*/
	

	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);

	fclose(file_sa);
	return 0;
}

static int if_goto_basic(char* condition_str, int addr, const char* asm_file_name) {
	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	int oper_1 = 0, oper_2 = 0;
	char token[3] = {0};

	char asm_cmd[30] = {0};

	condition_check(condition_str, &oper_1, &oper_2, token);

	sprintf(asm_cmd, "%x %s %x\n", num_str++, "LOAD", oper_1);
	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	sprintf(asm_cmd, "%x %s %x\n", num_str++, "SUB", oper_2);
	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);	

	if(!strncmp(token, ">", 1)) {
		sprintf(asm_cmd, "%x %s %x\n", num_str++, "JNS", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "<", 1)) {
		sprintf(asm_cmd, "%x %s %x\n", num_str++, "JNEG", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "=", 1)) {
		sprintf(asm_cmd, "%x %s %x\n", num_str++, "JZ", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, ">=", 2)) {
		sprintf(asm_cmd, "%x %s %x\n", num_str++, "JNS", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
		sprintf(asm_cmd, "%x %s %x\n", num_str++, "JZ", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "<=", 2)) {
		sprintf(asm_cmd, "%x %s %x\n", num_str++, "JNEG", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
		sprintf(asm_cmd, "%x %s %x\n", num_str++, "JZ", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "!=", 2)) {
		sprintf(asm_cmd, "%x %s %x\n", num_str, "JZ", num_str+2);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
		sprintf(asm_cmd, "%x %s %x\n", num_str, "JUMP", addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else {
		fprintf( stderr, "if_goto_basic: condition incorrect! \n");
		fclose(file_sa);
		return -1;
	}

	fclose(file_sa);
	return 0;
}

static int let_basic(char* value, const char* asm_file_name) {

}

static int end_basic(const char* asm_file_name) {
	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	char asm_cmd[30] = {0};
	
	sprintf(asm_cmd, "%x %s %x\n", num_str, "HALT", 0);
	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);

	fclose(file_sa);
	return 0;
}



static int condition_check(char* condition_str, int* operand_1, int* operand_2, char* token) {
	//char condition_token[4] = {'>', '<', '=', '!'}; 
	char tmp_str[3] = {0};
	uint8_t num = 0;
	uint8_t before_token = strcspn(condition_str, "><=!");

	//Проверка есть ли в строке условие
	if(before_token == strlen(condition_str)) {
		fprintf( stderr, "condition_check: condition incorrect! \n");
		return -1;
	}

	//Пишем первый операнд
	memcpy(tmp_str, condition_str, before_token);

	if(!(*operand_1 = value_cheker(tmp_str[0]))) {
		*operand_1 = atoi(tmp_str);
	}

	num++;

	//Какое сравнение
	if (condition_str[before_token] != '=') {
		uint8_t tmp = strcspn(condition_str, "="); 
		if (strlen(condition_str) > tmp) {
			num++;
		}
	}

	//Пишем, какое условие
	memcpy(token, &condition_str[before_token], num);



	uint8_t oper_2_num = strlen(condition_str) - (before_token+num);
	//Пишем второй операнд
	memcpy(tmp_str, &condition_str[before_token+num], oper_2_num);

	if (!(*operand_2 = value_cheker(tmp_str[0]))) {
		*operand_2 = atoi(tmp_str);
	}

	return 0;
}



