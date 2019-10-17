#ifndef mySimpleComputer_h
#define mySimpleComputer_h

#include <sys/time.h>

#define MEMSIZE 100
#define COMMANDS_COUNT 44

#define OF 1				// overflow flag
#define ZF 2 				// zero flag
#define IF 3 				// ignoring flag
#define OM 4 				// out of memory
#define BC 5 				// bad command
#define IR 6 				// is run

int ram[MEMSIZE]; 			// массив памяти
int FLAGS;					// регистр флагов
int ax;						// аккумулятор
int ip;						// предыдущая команда
int operand;				// операнд
int command;				// команда
int mem_ptr;				// указатель на ячейку памяти
int f_key, f_ign;					
struct itimerval nval, oval;

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);
int sc_regInit();
int sc_regSet(int flag, int value);
int sc_regGet (int flag, int *value);
int sc_commandEncode (int command, int operand, int *value);
int sc_commandDecode (int value, int *command, int *operand);
void timerStart();

#endif // mySimpleComputer_h
