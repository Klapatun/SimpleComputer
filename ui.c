#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>

#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"

#define TMP_STR_LEN 20

/* display Borders */
void displayBorders()
{
	int term = open(TERM, O_RDWR);
	if (term == -1) {
		fprintf(stderr, "%s\n", "displayBorders: terminal error ");
		close(term);
		return -1;
	}
	
	mt_clrscr();
	
	bc_box(1, 1, 12, 61);	
	bc_box(1, 62, 3, 22);	
	bc_box(4, 62, 3, 22);	
	bc_box(7, 62, 3, 22);	
	bc_box(10, 62, 3, 22);	
	bc_box(13, 1, 10, 46);	
	bc_box(13, 47, 10, 37);
		
	mt_gotoXY(1, 30);
	write(term, " Memory ", strlen(" Memory "));
	mt_gotoXY(1, 67);
	write(term, " accumulator ", strlen(" accumulator "));
	mt_gotoXY(4, 63);
	write(term, " instructionCounter ", strlen(" instructionCounter "));
	mt_gotoXY(7, 68);
	write(term, " Operation ", strlen(" Operation "));
	mt_gotoXY(10, 69);
	write(term, " Flags ", strlen(" Flags "));
	mt_gotoXY(13, 48);
	write(term, " Keys: ", strlen(" Keys: "));
	
	mt_gotoXY(14, 48);
	write(term, "l  - load", strlen("l  - load"));
	mt_gotoXY(15, 48);
	write(term, "s  - save", strlen("s  - save"));
	mt_gotoXY(16, 48);
	write(term, "r  - run", strlen("r  - run"));
	mt_gotoXY(17, 48);
	write(term, "t  - step", strlen("t  - step"));
	mt_gotoXY(18, 48);
	write(term, "e  - edit cell", strlen("e  - edit cell"));	
	mt_gotoXY(19, 48);
	write(term, "i  - reset", strlen("i  - reset"));
	mt_gotoXY(20, 48);
	write(term, "F5 - accumulator", strlen("F5 - accumulator"));
	mt_gotoXY(21, 48);
	write(term, "F6 - instructionCounter", strlen("F6 - instructionCounter"));
		
	mt_gotoXY(23, 1);
	close(term);
	
}

/* display Accumulator */
int displayAccumulator()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "displayAccumulator: terminal error");
        close (term);
        return -1;
    }

    char str_1[TMP_STR_LEN];

    mt_gotoXY(2, 69);
    write(term, "+0000", 5);
    if (ax >= 0) {
		sprintf(str_1, "+%04X    ", ax);
		mt_gotoXY(2, 69);
	} else {
		sprintf(str_1, "-%04X    ", -1 * ax);
		mt_gotoXY(2, 69);
	}

    write(term, str_1, strlen(str_1));

    close (term);
    mt_gotoXY(23, 1);
    return 0;
}

/* display Memory */
void displayMemory()
{
	int term = open(TERM, O_RDWR);
	if (term == -1) {
		fprintf(stderr, "%s\n", "displayBorders: terminal error");
		close(term);
		return -1;
	}
	
	int value, comm, oper, k;
	
	mt_gotoXY(2, 2);
	
	int j = 2, i = 0;
	
	for (i = 1; i <= 100; ++i) {
		
		sc_memoryGet(i - 1, &value);

		k    = value >> 14 & 0x1;
		oper = value & 0x7F;
		comm = value >> 7 & 0x7F;
		
		char buf[50];
		
		if (k)
			sprintf(buf, i % 10 ? " %04X " : " %04X", value & 0x1FFF);
		else		
			sprintf(buf, i % 10 ? "%c%02X%02X " : "%c%02X%02X", (k == 0) ? '+' : ' ', comm, oper);
		
		write(term, buf, strlen(buf));
		
		if (i % 10 == 0 && i != 0) {
			++j;
			mt_gotoXY(j, 2);
		}
		
	}
	
    char str_1[50];
    mt_gotoXY(mem_ptr / 10 + 2, mem_ptr % 10 * 6 + 2);
    
    sc_memoryGet(mem_ptr, &value);
    
    k    = value >> 14 & 0x1;
    oper = value & 0x7F;
    comm = value >> 7 & 0x7F;
    
    if (k)
		sprintf(str_1, " %04X", value & 0x1FFF);
    else
		sprintf(str_1, "%c%02X%02X", !k ? '+':' ', comm, oper);
    
    mt_setbgcolor(Green);
    write(term, str_1, strlen(str_1));
    mt_setbgcolor(Default);
    mt_gotoXY(23, 1);
    
    close(term);
}

/* display BigChar */
int displayBigChar()
{
    int big[] = {0, 0};
    int column = 2;
    int i, k, oper, comm, value;
    char str_1 [TMP_STR_LEN];

    sc_memoryGet(mem_ptr, &value);

    k    = value >> 14 & 0x1;
    oper = value & 0x7F;
    comm = value >> 7 & 0x7F;

    if (k)
		sprintf(str_1, " %04x", value & 0x1FFF);
    else
		sprintf(str_1, "%c%02X%02X", !k ? '+':' ', comm, oper);
		
    for (i = 0; i < 5; ++i, column += 9) {
        bc_setbigcharpos(big, str_1[i]);
        bc_printbigchar(big, 14, column, Default, Default);
    }

    mt_gotoXY(23, 1);
    return 0;
}

/* display Flags */
int displayFlags()
{
    int terminal = open("/dev/tty", O_RDWR);
    if (terminal == -1) {
        fprintf(stderr, "%s\n", "displayFlags: terminal error");
        close (terminal);
        return -1;
    }

    int k;
    mt_gotoXY(11, 63);
    sc_regGet(IF, &k); if (k) write(terminal, "IF ", 3); else write(terminal, "   ", 3);
    sc_regGet(ZF, &k); if (k) write(terminal, "ZF ", 3); else write(terminal, "   ", 3);
    sc_regGet(OF, &k); if (k) write(terminal, "OF ", 3); else write(terminal, "   ", 3);
    sc_regGet(OM, &k); if (k) write(terminal, "OM ", 3); else write(terminal, "   ", 3);
    sc_regGet(BC, &k); if (k) write(terminal, "BC ", 3); else write(terminal, "   ", 3);
    sc_regGet(IR, &k); if (k) write(terminal, "IR ", 3); else write(terminal, "   ", 3);

    close(terminal);
    
    mt_gotoXY(23, 1);
    
    return 0;
}

/* display Ip */
void displayIp()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "displayIp: terminal error");
        close (term);
        return -1;
    }
	
	ip = mem_ptr;
	
    char str_1[TMP_STR_LEN];

    mt_gotoXY(5, 69);
    write(term, "+0000", 5);

    if (ip > 99 || ip < 0) {sc_regSet(OM, 1); return 0;}

    sprintf(str_1, "%02X", ip);
    mt_gotoXY(5, 72);

    write(term, str_1, strlen(str_1));

    sc_regSet(OM, 0);

    close (term);
    mt_gotoXY(23, 1);
    return 0;
}

/* display Operation */
int displayOperation()
{
    char str_1 [TMP_STR_LEN];    
    int value, comm, oper, k;

    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "displayOperation: terminal error");
        close (term);
        return -1;
    }
    
    mt_gotoXY(8, 67);
    write(term, "+ 00 : 00", 5);
	
	sc_memoryGet(mem_ptr, &value);
    
    k    = value >> 14 & 0x1;
    oper = value & 0x7F;
    comm = value >> 7 & 0x7F;
    
    if (k)
		sprintf(str_1, "   %04X    ", value & 0x1FFF);
    else
		sprintf(str_1, "%c %02X : %02X", !k ? '+':' ', comm, oper);
	
    k = strlen(str_1);
    mt_gotoXY(8, 67);
    write(term, str_1, k);

    close(term);
    mt_gotoXY(23, 1);
    return 0;
}

/* clear input area */
void clearInput()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "clearInput: terminal error");
        close (term);
        return;
    }

	int i;
    mt_gotoXY (23, 1);
    for (i = 0; i < 80; ++i)
        write (term, " ", 1);
    mt_gotoXY (23, 1);

    close (term);
}

/* signalhandler */
void signalhandler (int signo)
{
	sc_regGet(IR, &f_key);
	sc_regGet(IF, &f_ign);
	
    if (ip < 99 && f_key && !f_ign) {
		cu();		
		displayUI();
	} else {		
		sc_regSet(IR, 0);
		alarm(0);     
	}
	f_key = 0;
}

/* display UI */
void displayUI()
{
    displayMemory();
    displayAccumulator();
    displayIp();
    displayOperation();
    displayFlags();
    displayBigChar();

    mt_gotoXY(23, 1);      
}

/* reset terminal */
void reset()
{
	 displayBorders();
     sc_memoryInit();
     sc_regInit();
     displayUI ();
}


