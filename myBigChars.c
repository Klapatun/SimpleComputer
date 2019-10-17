#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"

const char CHARS[]    = "+0123456789abcdef";
const int  CHARS_SIZE = 17;

/* prints string in an alternate symbol table */
int bc_printa(const char *str)
{
    int term = open(TERM, O_RDWR);

    if (term == -1) {
        fprintf(stderr, "%s\n", "bc_printa: terminal error!");
        close(term);
        return -1;
    } else {
        write(term, ENTER_ALT_CHARSET_MODE, ESC_STR_LEN);
        write(term, str, strlen(str));
        write(term, EXIT_ALT_CHARSET_MODE, ESC_STR_LEN);
    }

    close(term);
    return 0;
}

/* print box */
int bc_box(int x1, int y1, int x2, int y2)
{
	int term = open(TERM, O_RDWR);

    if (term == -1) {
        fprintf(stderr, "%s\n", "bc_box: terminal error!");
        close(term);
        return -1;
    }
	
	int i = 0;
	if(x1 == 0)
		x1 = 1;
	if(y1 == 0)
		y1 = 1;
	
	
	mt_gotoXY(x1,y1);
	bc_printa(LT);
	for (i = 0; i < y2 - 2; ++i)
		bc_printa(HL);
	
	bc_printa(RT);
	for (i = 1; i <= x2 - 2; ++i) {
		mt_gotoXY(x1 + i, y1);
		bc_printa(VL);
		mt_gotoXY(x1 + i, y1 + y2 - 1);
		bc_printa(VL);
	}
	
	mt_gotoXY(x1 + x2 - 1, y1);
	bc_printa(LB);
	for (i = 0; i < y2 - 2; ++i)
		bc_printa(HL);
		
	bc_printa(RB);
	
	close(term);
	return 0;
}

/* print big char 8x8 in (x, y) position */
int bc_printbigchar(const int *src, int x, int y,
					enum Colors font,
					enum Colors bground)
{
    int term = open(TERM, O_RDWR);

    if (term == -1) {
        fprintf(stderr, "%s\n", "bc_printbigchar: terminal error!");
        close(term);
        return -1;
    }

    mt_gotoXY(x, y);
    mt_setfgcolor(font);
    mt_setbgcolor(bground);

    int i, j, t;
    for (i = 0; i < 64; i += 8) {
        if (i == 32) ++src;
        t = (*src) >> i;
        for (j = 7; j >= 0; --j) {
            if (t >> j & 0x1)
                bc_printa(ACS_CKBOARD);
            else
                bc_printa(SPACE);
        }
        mt_gotoXY(++x, y);
    }

    close(term);
    return 0;
}

/* set big char position to value */
int bc_setbigcharpos(int *big, int value)
{
    int term = open(TERM, O_RDWR);

    if (term == -1) {
        fprintf(stderr, "%s\n", "bc_setbigcharpos: terminal error!");
        close(term);
        return -1;
    }

    if (!big) {
        fprintf(stderr, "%s\n", "bc_setbigcharpos: not work!");
        close(term);
        return -1;
    }

    switch (value) {
        case '+':
            big[0] = 0xFF181800;
            big[1] = 0x001818FF;
            break;
        case '0':
            big[0] = 0x818181FF;
            big[1] = 0xFF818181;
            break;
        case '1':
            big[0] = 0x120A0602;
            big[1] = 0x2020202;
            break;
        case '2':
            big[0] = 16843263U;
            big[1] = 4286611711U;
            break;
        case '3':
            big[0] = 4278256127U;
            big[1] = 4278255873U;
            break;
        case '4':
            big[0] = 4286677377U;
            big[1] = 16843009;
            break;
        case '5':
            big[0] = 2155905279U;
            big[1] = 4278256127U;
            break;
        case '6':
            big[0] = 2155905279U;
            big[1] = 4286677503U;
            break;
        case '7':
            big[0] = 134480639;
            big[1] = 538976272;
            break;
        case '8':
            big[0] = 4286677503U;
            big[1] = 4286677377U;
            break;
        case '9':
            big[0] = 4286677503U;
            big[1] = 4278255873U;
            break;
        case 'A':
            big[0] = 2172748158U;
            big[1] = 2172748287U;
            break;
        case 'B':
            big[0] = 4269900286U;
            big[1] = 4269900286U;
            break;
        case 'C':
            big[0] = 2155905279U;
            big[1] = 4286611584U;
            break;
        case 'D':
            big[0] = 2172748286U;
            big[1] = 4269900161U;
            break;
        case 'E':
            big[0] = 4286611711U;
            big[1] = 4286611711U;
            break;
        case 'F':
            big[0] = 4286611711U;
            big[1] = 2155905152U;
            break;
        default:
            big[0] = 0;
            big[1] = 0;
            break;
    }
    close(term);
    return 0;
}

/* write big char value on position (x,y) to *value */
int bc_getbigcharpos(const int *big, int *value)
{
    int i, arr[2];
    for (i = 0; i < CHARS_SIZE; ++i) {
        bc_setbigcharpos (arr, CHARS[i]);
        if (arr[0] == big[0] &&
            arr[1] == big[1]   )
        {
            *value = CHARS[i];
            return 0;
        }
    }
    return -1;
}

/* write to file big chars */
int bc_bigcharwrite(const int *big) 
{
	FILE *out = fopen("bigchar.out", "w");
	if (!out) {
		printf("Can not open file 'bigchar.out' for write!\n");
		return -1;
	}

	fprintf(out, "%ld %ld", big[0], big[1]);
	fclose(out);
	return 0;
}

/* read from file big chars */
int bc_bigcharread(int *big) 
{
	FILE *in = fopen("bigchar.bin", "r");
	if(!in) {
		printf("Can not open file 'bigchar.bin' for read!\n");
		return -1;
	}

	fscanf(in, "%ld%ld", &big[0], &big[1]);
	fclose(in);
	return 0;
}
