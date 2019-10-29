/*simple_basic.c*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "simple_basic.h"
#include "variables.h"
#include "opz.h"


struct line_t {
	//int line_bas;
	int line_asm;
};

static int rem_basic(char* comment, char* asm_file_name);
static int input_basic(char value_name, const char* asm_file_name);
static int output_basic(char value_name, const char* asm_file_name);
static int goto_basic(int str_addr, const char* asm_file_name);
static int if_goto_basic(char* condition_str, int addr, struct line_t* line, const char* asm_file_name);
static int let_basic(char* opz_str, int size_str, const char* asm_file_name);
static int end_basic(const char* asm_file_name);

//static int value_cheker(char value_name);
static int condition_check(char* condition_str, int* operand_1, int* operand_2, char* token);




//static int last_stack_idx;
static int asm_num_str;
static int bas_num_str;


int basic_processing (const char* basic_file, const char* asm_file) {

	char basic_cmd[50] = {0};
	char command[7] = {0};
	char value[40] = {0}; 

	struct line_t line[100] = {0};
	//int line_cnt = 0;

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
			//line[bas_num_str]->line_bas = bas_num_str;
			line[bas_num_str].line_asm = asm_num_str;
			rem_basic(&basic_cmd[4], asm_file);
			bas_num_str++;
		}
		else if (!strncmp(command, "INPUT", 5)) {
			
			char tmp[30] = {0};
			sscanf(&basic_cmd[6], "%s ", tmp);

			int tmp_int = strlen(tmp);
			if(tmp_int != 1) {
				fprintf( stderr, "Error: line number %d! \n", bas_num_str); //ИСПРАВИТЬ num_str не то
				return -1;
			}

			line[bas_num_str].line_asm = asm_num_str;
			input_basic(tmp[0], asm_file);

			bas_num_str++;
		}
		else if (!strncmp(command, "OUTPUT", 6)) {

			char tmp[30] = {0};
			sscanf(&basic_cmd[7], "%s ", tmp);

			int tmp_int = strlen(tmp);
			if(tmp_int != 1) {
				fprintf( stderr, "Error: line number %d! \n", bas_num_str); //ИСПРАВИТЬ num_str не то
				return -1;
			}
			line[bas_num_str].line_asm = asm_num_str;
			output_basic(tmp[0], asm_file);

			bas_num_str++;

		}
		else if (!strncmp(command, "GOTO", 4)) {

			char tmp_char[10] = {0};
			sscanf(&basic_cmd[5], "%s\n", tmp_char);
			line[bas_num_str].line_asm = asm_num_str;
			goto_basic(atoi(tmp_char), asm_file);
			bas_num_str++;
		}
		else if (!strncmp(command, "IF", 2)) {
			char condition_str[20] = {0};
			char addr_str[3] = {0};

			sscanf(basic_cmd, "IF %s GOTO %s\n", condition_str, addr_str);

			int addr = atoi(addr_str);
			//char* oper_1[3] = {0};
			//char* oper_2[3] = {0};
			//char* token[3] = {0};

			line[bas_num_str].line_asm = asm_num_str;
			if_goto_basic(condition_str, addr, line, asm_file);
			

			bas_num_str++;
		}
		else if (!strncmp(command, "LET", 3)) {
			char exp[50] = {0};

			int num = opz(&basic_cmd[4], exp);

			line[bas_num_str].line_asm = asm_num_str;
			let_basic(exp, num, asm_file);

			bas_num_str++;
		}
		else if (!strncmp(command, "END", 3)) {
			line[bas_num_str].line_asm = asm_num_str;
			end_basic(asm_file);
			bas_num_str++;
		}
		else {

		}

		//int value_size = (int)strlen(&basic_cmd[command_size]);
		//memcpy(value, &basic_cmd[command_size], strlen(value_size));

	}

	fclose(file_bas);
}



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
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "READ", addr_val);
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
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "WRITE", addr_val);
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
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JUMP", str_addr);
	/*}
	else{
		sprintf(asm_cmd, "0%x %s %x\n", num_str, "JUMP", str_addr);
	}*/
	

	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);

	fclose(file_sa);
	return 0;
}

static int if_goto_basic(char* condition_str, int addr, struct line_t* line, const char* asm_file_name) {
	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	if (addr < 0 || addr > 99) {
		//Ошибка
		fprintf( stderr, "if_goto_basic: Incorrect address! \n");
		fclose(file_sa);
		return -2;
	}

	int asm_addr = line[addr].line_asm;

	int oper_1 = 0, oper_2 = 0;
	char token[3] = {0};

	char asm_cmd[30] = {0};

	condition_check(condition_str, &oper_1, &oper_2, token);

	sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "LOAD", oper_1);
	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "SUB", oper_2);
	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);	

	if(!strncmp(token, ">", 1)) {
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JNS", asm_addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "<", 1)) {
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JNEG", asm_addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "=", 1)) {
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JZ", asm_addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, ">=", 2)) {
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JNS", asm_addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JZ", asm_addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "<=", 2)) {
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JNEG", asm_addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "JZ", asm_addr);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
	}
	else if (!strncmp(token, "!=", 2)) {
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str, "JZ", asm_num_str+2);
		fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
		sprintf(asm_cmd, "%x %s %x\n", asm_num_str, "JUMP", asm_addr);
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



static int let_basic(char* opz_str, int size_str, const char* asm_file_name) {

	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	if(size_str < 3 || opz_str[1] != '=') {
		fprintf( stderr, "let_basic: Some problem! \n");
		fclose(file_sa);
		return -2;
	}

	char asm_cmd[30] = {0};
	
	int stack[30] = {0};
	int idx_stack = 0;
	//int addr_tmp = 0;
	int num_tmp_var = 0;
	int ac_have = 0;

	for (int i=2; i<size_str; i++) {
		int tmp = 0;

		if (tmp = value_cheker(opz_str[i])) {
			//Переменная
			stack[idx_stack++] = tmp;

			if(ac_have && idx_stack > 1) {
				int addr_tmp = tmp_alloc_stack();	//выделяем доп.переменную
				num_tmp_var++;
				//Сохраняем в нее аккум
				sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "STORE", addr_tmp);
				fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);

				//Тут делаем смещение стека
				int addr_2 = stack[--idx_stack];
				int addr_1 = stack[--idx_stack];

				stack[idx_stack++] = addr_tmp;
				stack[idx_stack++] = addr_1;
				stack[idx_stack++] = addr_2;

				ac_have = 0; //акумм теперь можно стирать
			}

		}
		else {
			//Операция

			if(idx_stack == 0) {
				//Ошибка
				fprintf( stderr, "let_basic: Incorrect expression! \n");
				fclose(file_sa);
				return -3;
			}

			char cmd_str[7] = {0};
			int operand_1 = 0;
			int operand_2 = 0;

			//Проверка на то, что за операция
			switch(opz_str[i]) {

			case '+':
				strcpy(cmd_str, "ADD");
				break;

			case '-':
				strcpy(cmd_str, "SUB");
				break;

			case '*':
				strcpy(cmd_str, "MUL");
				break;

			case '/':
				strcpy(cmd_str, "DIVIDE");
				break;
			default:
				return -3;
				break;
			}

			if (num_tmp_var && ac_have) {	//Если есть доп переменные и аккум не пуст
				int addr_tmp = tmp_alloc_stack();	//выделяем доп.переменную
				num_tmp_var++;
				//Сохраняем в нее аккум
				sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "STORE", addr_tmp);
				fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
				
				stack[idx_stack++] = addr_tmp;
				ac_have = 0;	//Очищаем аккум

			}

			operand_2 = stack[--idx_stack];

			//Если idx_stack == 0, тогда необходимое значение в аккумуляторе 
			if (idx_stack > 0 && !ac_have) {
				operand_1 = stack[--idx_stack];
			}

			if (operand_1 != 0) {
				//Первый операнд записываю в аккум 
				sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "LOAD", operand_1);
				fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
			}

			sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, cmd_str, operand_2);
			fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);
			ac_have = 1; //в аккуме что-то есть
		}
	}

	//Выгружаем результат из аккума в переменную
	int left = value_cheker(opz_str[0]);
	sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "STORE", left);
	fwrite(asm_cmd, sizeof(char), strlen(asm_cmd), file_sa);

	//Уничтожаем доп переменные 
	while(num_tmp_var) {
		tmp_free_stack();
	}

	fclose(file_sa);
	return 0;
}

static int end_basic(const char* asm_file_name) {
	FILE *file_sa = fopen(asm_file_name, "a+t");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	char asm_cmd[30] = {0};
	
	sprintf(asm_cmd, "%x %s %x\n", asm_num_str++, "HALT", 0);
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



