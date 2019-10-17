#include <stdio.h>
#include <stdlib.h>

#include "mySimpleComputer.h"

/* Массив команд компьютера */
int commands[] = { 	
0x10, 0x11, 0x20, 0x21, 0x30, 0x31, 0x32,
0x33, 0x40, 0x41, 0x42, 0x43, 0x51, 0x52,
0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
0x60, 0x61, 0x63, 0x64, 0x65, 0x66, 0x67,
0x68, 0x69, 0x70, 0x71, 0x72, 0x73, 0x74,
0x02, 0x75, 0x02, 0x76
};

/* Функции для работы с памятью */

int sc_memoryInit()
{
	int i = 0;
	for(i = 0; i < MEMSIZE; ++i)
		ram[i] = 0;
	
	ax		= 0;
	ip		= 0;
	command = 0;
	operand = 0;
	mem_ptr = 0;
	
	return 0;
}

int sc_memorySet(int address, int value)
{
	if(address < MEMSIZE && address >= 0) {
		ram[address] = value;
	} else {
		sc_regSet(OM, 1); // Поднимаем флаг "out of memory"
		return -1;
	}
	return 0;
}

int sc_memoryGet(int address, int *value)
{
	if(address < MEMSIZE && address >= 0) {
		*value = ram[address];
	} else {
		sc_regSet(OM, 1); // Поднимаем флаг "out of memory"
		return -1;
	}
	return 0;
}

int sc_memorySave(char *filename)
{
	FILE *file = fopen(filename, "wb");
	
	if (!file) {
		fprintf(stderr, "Схоронялка памяти: File not open! \n");
		return -1;
	}
	
	fwrite(ram, MEMSIZE, sizeof(int), file);
	fclose(file);
	return 0;
}

int sc_memoryLoad(char *filename)
{
	FILE *file = fopen(filename, "rb");
	
	if (!file) {
		fprintf( stderr, "Загружалка памяти: File not open! \n");
		return -1;
	}
	
	fread(ram, MEMSIZE, sizeof(int), file);
	fclose(file);
	return 0;
}

int sc_regInit()
{
	FLAGS = 0;
	return 0;
}

int sc_regSet(int flag, int value)
{
	if (value < 0 || value > 1) {
		fprintf(stderr, "sc_regSet: bad value!");
		return -1;
	}
	
	if (flag < 0 || flag > 6) {
		fprintf(stderr, "sc_regSet: bad flag!");
		return -1;
	}
		
	if (value)
		FLAGS |= value << (flag - 1);
	else
		FLAGS &= ~( 1 << (flag - 1));

	return 0;
}

int sc_regGet (int flag, int *value)
{	
	if (flag > 0 && flag <= 6)
	{
		*value = (FLAGS >> (flag - 1)) & 0x1;
	} else {
		fprintf(stderr, "sc_regGet: bad flag or value = NULL!");
		return -1;
	}
		
	return 0;
}

// Производим кодирование
int sc_commandEncode (int command, int operand, int *value)
{
	int i, f = 0;
	
	for (i = 0; i < COMMANDS_COUNT; ++i)
		if (commands[i] == command)	{
			f = 1;
			break;
		}

	if (f) {
		*value = (command & 0x7F) << 7 | (operand & 0x7F);
		sc_regSet(BC, 0);
		return 0;
	}

	sc_regSet(BC, 1);
	return -1;	
}

// Производим декодирование
int sc_commandDecode (int value, int *command, int *operand)
{
	if (!value || value >> 14 || !command || !operand) {
		sc_regSet(BC, 1);
		return -1;
	}

	*operand = value & 0x7F;
	*command = (value >> 7) & 0x7F;
	return 0;
}

// Упс, что-то пошло не так
int err_msg(const char *file, int line)
{
    if (!file || !line)
        return -1;

    fprintf(stderr, "Обшибка в файле \'%s\' на строчке: %d \n", file, line);

    return 0;
}

/* run timer */
void timerStart()
{
	setitimer (ITIMER_REAL, &nval, &oval);
}

