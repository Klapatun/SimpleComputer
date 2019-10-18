/*simple_basic.c*/

#include <stdio.h>
#include <stdint.h>

#include "simple_basic.h"

static int rem_basic(char* comment, const char* asm_file_name);
static int input_basic(char value_name, const char* asm_file_name);

static int value_cheker(char value_name, int data);

struct values {

	int A;
	int B;
	int C;
	int D;
	int E;
	int F;
	int G;
	int H;
	int I;
	int J;
	int K;
	int L;
	int M;
	int N;
	int O;
	int P;
	int Q;
	int R;
	int S;
	int T;
	int U;
	int V;
	int W;
	int X;
	int Y;
	int Z;
};

static int last_stack_idx;
static int num_str;


int basic_processing (const char* basic_file, const char* asm_file) {

	char basic_cmd[50] = {0};
	char command[7] = {0};
	char value[40] = {0}; 

	last_stack_idx = 0x63;

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
			rem_basic(&basic_cmd[4], asm_file);
			num_str++;
		}
		else if (!strncmp(command, "INPUT", 5)) {
			
			char tmp[10] = {0};
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

		}
		else if (!strncmp(command, "GOTO", 4)) {

		}
		else if (!strncmp(command, "IF", 2)) {

		}
		else if (!strncmp(command, "LET", 3)) {

		}
		else if (!strncmp(command, "END", 3)) {

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
	FILE *file_sa = fopen(asm_file_name, "at");

	if (!file_sa) {
		fprintf( stderr, "File .sa not open! \n");
		return -1;
	}

	int addr_val = value_cheker(value_name, 0);
	if (addr_val == 0) {
		fclose(file_sa);
		return -1;
	}

	char asm_cmd[30] = {0};

	sprintf(asm_cmd, "%x %s %x", 46, "READ", addr_val);

	fwrite(asm_cmd, 10, sizeof(int), file_sa);


	fclose(file_sa);
	return 0;
}

static int output_basic(char* value_name, const char* asm_file_name) {

}

static int goto_basic(char* str_addr, const char* asm_file_name) {

}

static int if_basic(char* value, const char* asm_file_name) {

}

static int let_basic(char* value, const char* asm_file_name) {

}

static int end_basic(const char* asm_file_name) {

}

/*
** возвращает адресс переменной
*/
static int value_cheker(char value_name, int data) {

	static struct values user_values;

	switch(value_name) {
		case 'A':

			if (!(user_values.A)) {

				user_values.A = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.A;
			
			break;

		case 'B':
		
			if (!(user_values.B)) {

				user_values.B = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.B;

			break;

		case 'C':
			
			if (!(user_values.C)) {

				user_values.C = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.C;

			break;

		case 'D':
			
			if (!(user_values.D)) {

				user_values.D = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.D;

			break;

		case 'E':
			
			if (!(user_values.E)) {

				user_values.E = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.E;

			break;

		case 'F':
			
			if (!(user_values.F)) {

				user_values.F = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.F;

			break;

		case 'G':
			
			if (!(user_values.G)) {

				user_values.G = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.G;

			break;
			
		case 'H':
			
			if (!(user_values.H)) {

				user_values.H = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.H;

			break;

		case 'I':
			
			if (!(user_values.I)) {

				user_values.I = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.I;

			break;

		case 'J':

			if (!(user_values.J)) {

				user_values.J = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.J;

			break;

		case 'K':

			if (!(user_values.K)) {

				user_values.K = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.K;

			break;

		case 'L':

			if (!(user_values.L)) {

				user_values.L = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.L;

			break;

		case 'M':

			if (!(user_values.M)) {

				user_values.M = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.M;

			break;

		case 'N':

			if (!(user_values.N)) {

				user_values.N = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.N;

			break;

		case 'O':
		
			if (!(user_values.O)) {

				user_values.O = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.O;

			break;

		case 'P':

			if (!(user_values.P)) {

				user_values.P = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.P;

			break;

		case 'Q':

			if (!(user_values.Q)) {

				user_values.Q = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.Q;

			break;
		case 'R':

			if (!(user_values.R)) {

				user_values.R = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.R;

			break;

		case 'S':

			if (!(user_values.S)) {

				user_values.S = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.S;

			break;

		case 'T':

			if (!(user_values.T)) {

				user_values.T = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.T;

			break;

		case 'U':

			if (!(user_values.U)) {

				user_values.U = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.U;

			break;

		case 'V':

			if (!(user_values.V)) {

				user_values.V = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.V;

			break;

		case 'W':

			if (!(user_values.W)) {

				user_values.W = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.W;

			break;

		case 'X':

			if (!(user_values.X)) {

				user_values.X = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.X;

			break;

		case 'Y':

			if (!(user_values.Y)) {

				user_values.Y = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.Y;

			break;

		case 'Z':

			if (!(user_values.Z)) {

				user_values.Z = last_stack_idx;
				last_stack_idx--;
			}

			return user_values.Z;

			break;

		default:
			return 0;
	}
}
