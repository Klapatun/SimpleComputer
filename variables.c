/*variables.c*/

#include "variables.h"

static int last_stack_idx = 0x63;

/*
** возвращает адресс переменной
*/
static int value_cheker(char value_name) {

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