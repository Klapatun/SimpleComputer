#ifndef myTerm_h
#define myTerm_h

#define CLEAR 		"\E[H\E[J"
#define GOTOXY 		"\E[%d;%dH"
#define SETFGCOLOR 	"\E[3%dm"
#define SETBGCOLOR 	"\E[4%dm"
#define ESCSPACE	"\E[1C"
#define ESC 		'\E'

#define TERM 		"/dev/tty"

enum Colors { Default	= 9,
              Red		= 1,
              Green 	= 2,
              Yellow	= 3,
              Blue		= 4,
              Magenta	= 5 };

int mt_clrscr(void);
int mt_gotoXY(int i, int j);
int mt_getscreensize(int *rows, int *cols);
int mt_setfgcolor(enum Colors color);
int mt_setbgcolor(enum Colors color);

#endif // myTerm_h
