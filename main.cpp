/*
**
**
*/
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

#define ESC	"\033"

int main(){

	printf(ESC "[2J"); 	//clear
	printf(ESC "[H");	//home

    return 0;
}
