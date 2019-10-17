#ifndef myBigChars_h
#define myBigChars_h

#include "myTerm.h"

#define ENTER_ALT_CHARSET_MODE "\E(0"
#define EXIT_ALT_CHARSET_MODE  "\E(B"
#define ESC_STR_LEN strlen(ENTER_ALT_CHARSET_MODE)

#define SPACE       " "
#define ACS_CKBOARD "a"

#define LT "l" // left top
#define LB "m" // left bottom
#define RT "k" // right top
#define RB "j" // right bottom
#define VL "x" // vert line
#define HL "q" // horis line

int bc_printa(const char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar (const int *src, int x, int y, enum Colors font, enum Colors bground);
int bc_setbigcharpos (int *big, int value);
int bc_getbigcharpos(const int *big, int *value);
int bc_bigcharwrite(const int *big);
int bc_bigcharread(int *big);

#endif // myBigChars_h
