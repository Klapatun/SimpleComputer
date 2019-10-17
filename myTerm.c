#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>

#include "myTerm.h"

/* clear terminal screen */
int mt_clrscr(void)
{
	int term = open(TERM, O_RDWR);
	
	if (term == -1) {
		fprintf(stderr, "mt_gotoXY: terminal error!");
		close(term);
		return -1;
	}
	
	write(term, CLEAR, sizeof(CLEAR));
	close(term);
	return 0;
}

/* set cursor position */
int mt_gotoXY(int x, int y)
{
	if (x < 0 || y < 0) {
		fprintf(stderr, "%s\n", "mt_gotoXY: negative coordinates!");
		return -1;
	}
	
	int term = open(TERM, O_RDWR);
	if (term == -1) {
		fprintf(stderr, "%s\n", "mt_gotoXY: terminal error!");
		close(term);
		return -1;
	}
	
	char buf[50];
	
	sprintf(buf, GOTOXY, x, y);
		
	write(term, buf, strlen(buf));
	
	close(term);
	return 0;
}

/* compute screen size */
int mt_getscreensize(int *rows, int *cols)
{
	struct winsize ws;
	
	if (!ioctl (1,TIOCGWINSZ,&ws)){
		*rows = ws.ws_row;
		*cols = ws.ws_col;
	} else {
		fprintf(stderr, "%s\n", "mt_getscreensize error!");
		return -1;
	}
	
	return 0;	
}

/* set font color */
int mt_setfgcolor(enum Colors color)
{	
	int term = open(TERM, O_RDWR);
	
	if (term == -1) {
		fprintf(stderr, "%s\n", "mt_setfgcolor: terminal error!");
		close(term);
		return -1;
	}
	
	char buf[50];
	
	sprintf(buf, SETFGCOLOR, color);
	write(term, buf, strlen(buf));
	
	close(term);
	return 0;
}

/* set background color */
int mt_setbgcolor(enum Colors color)
{

	int term = open(TERM, O_RDWR);
	
	if (term == -1) {
		fprintf(stderr, "%s\n", "mt_setbgcolor: terminal error!");
		close(term);
		return -1;
	}
	
	char buf[50];
	
	sprintf(buf, SETBGCOLOR, color);
	write(term, buf, strlen(buf));
	
	close(term);
	return 0;
}
