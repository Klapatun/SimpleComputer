/*
**
**
*/
#include <string.h>
#include <sys/ioctl.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <termios.h>

using namespace std;

/*#define ESC	"\033"*/


namespace TermLinux {


    enum {
        RED 	= 1,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        DEFAULT	= 9
    };

    class MyTerm {
        private:
            const char* CLEAR = "\E[2J";
            const char* GOTOXY = "\E%d;%dH";
            const char* SETFGCOLOR = "\E[3%dm";
            const char* SETBGCOLOR = "\E[4%dm";
            const char* ESCSPACE = "\E[1C";
            const char* ESC = "\E";
        public:

            int mt_clrscr(void) {

                int term = open("/dev/tty", O_RDWR);

                if (term == -1) {
                    cerr << "mt_clrscr: Terminal Error!\n";
                    close(term);
                    return -1;
                }

                write(term, CLEAR, sizeof(CLEAR));
                close(term);
                return 0;
            }

            int mt_gotoXY(int x, int y) {

                int term = open("/dev/tty", O_RDWR);

                if (term == -1) {
                    cerr << "mt_gotoXY: Terminal Error!\n";
                    close(term);
                    return -1;
                }
                char buff[50];

                sprintf(buff, GOTOXY, x, y);

                write(term, buff, strlen(buff));
                close(term);
                return 0;
            }

            int mt_getscreensize (int* rows, int* cols) {

                struct winsize ws;

                if (!ioctl (1, TIOCGWINSZ, &ws)) {
                    *rows = ws.ws_row;
                    *cols = ws.ws_col;
                }else {
                    cerr << "mt_getscreensize: Error!\n";
                    return -1;
                }

                return 0;
            }

            int mt_setfgcolor (uint8_t color) {

                int term = open("/dev/tty", O_RDWR);

                if (term == -1) {
                    cerr << "mt_setfgcolor: Terminal Error!\n";
                    close(term);
                    return -1;
                }

                char buff[50];

                sprintf(buff, SETFGCOLOR, color);
                write(term, buff, strlen(buff));

                close(term);

                return 0;
            }

            int mt_setbgcolor (uint8_t color) {

                int term = open("/dev/tty", O_RDWR);

                if (term == -1) {
                    cerr << "mt_setbgcolor: Terminal Error!\n";
                    close(term);
                    return -1;
                }

                char buff[50];

                sprintf(buff, SETBGCOLOR, color);
                write(term, buff, strlen(buff));

                close(term);
                return 0;
            }

    };

    class MyBigChars: public MyTerm {

    private:
        const char* CHARS    = "+0123456789abcdef";
        const int  CHARS_SIZE = 17;

        const char* ENTER_ALT_CHARSET_MODE = "\E(0";
        const char* EXIT_ALT_CHARSET_MODE = "\E(B";


        const char LT = 'l';
        const char LB = 'm';
        const char RT = 'k';
        const char RB = 'j';
        const char VL = 'x';
        const char HL = 'q';

        const char SPACE = ' ';
        const char ACS_CKBOARD = 'a';

    public:

        int bc_printA(const char* str) {

            int term = open("/dev/tty", O_RDWR);

            if (term == -1) {
                cerr << "bc_printa: Terminal Error!\n";
                close(term);
                return -1;
            } else {
                write(term, ENTER_ALT_CHARSET_MODE, strlen(ENTER_ALT_CHARSET_MODE));
                write(term, str, strlen(str));
                write(term, EXIT_ALT_CHARSET_MODE, strlen(EXIT_ALT_CHARSET_MODE));
            }

            close(term);
            return 0;
        }

        int bc_box(int x1, int y1, int x2, int y2) {

            int term = open("/dev/tty", O_RDWR);

            if(term == -1) {
                cerr << "bc_box: Terminal Error!\n";
                close(term);
                return -1;
            }

            if(x1 == 0) {
                x1 = 1;
            }
            if(y1 == 0) {
                y1 = 1;
            }

            mt_gotoXY(x1,y1);
            bc_printA(&LT);
            for(int i=0; i < y2-2; ++i) {
                bc_printA(&HL);
            }

            bc_printA(&RT);
            for(int i=0; i <= x2-2; ++i) {
                mt_gotoXY(x1+i, y1);
                bc_printA(&VL);
                mt_gotoXY(x1+i, y1+y2-1);
                bc_printA(&VL);
            }

            mt_gotoXY(x1+x2-1, y1);
            bc_printA(&LB);
            for(int i=0; i < y2-2; ++i) {
                bc_printA(&HL);
            }

            bc_printA(&RB);

            close(term);
            return 0;
        }

        int bc_printbigchar(const int* src, int x, int y, uint8_t font, uint8_t bgoaund) {

            int term = open("/dev/tty", O_RDWR);

            if(term == -1) {
                cerr << "bc_printbigchar: Terminal Error!\n";
                close(term);
                return -1;
            }

            mt_gotoXY(x,y);
            mt_setfgcolor(font);
            mt_setbgcolor(bgoaund);

            for(int i=0; i < 64; i +=8 ) {
                if(i == 32) {
                    ++src;
                }
                int tmp = (*src) >> i;
                for(int j=7; j >= 0; --j) {
                    if(tmp >> j & 0x1) {
                        bc_printA(&ACS_CKBOARD);
                    } else {
                        bc_printA(&SPACE);
                    }
                    mt_gotoXY(++x, y);
                }
            }

            close(term);
            return 0;
        }

        int bc_setbigcharpos(int *big, int value) {
            int term = open("/dev/tty", O_RDWR);

            if (term == -1) {
                cerr << "bc_setbigcharpos: Terminal Error!\n";
                close(term);
                return -1;
            }

            if (!big) {
                cerr << "bc_setbigcharpos: Not Work!\n";
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

        int bc_getbigcharpos(const int *big, int* value) {

            int arr[2];
            for (int i = 0; i < CHARS_SIZE; ++i) {
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

        int bc_bigcharwrite(const int* big) {
            FILE *out = fopen("bigchar.out", "w");
            if (!out) {
                printf("Can not open file 'bigchar.out' for write!\n");
                return -1;
            }

            fprintf(out, "%ld %ld", big[0], big[1]);
            fclose(out);
            return 0;
        }

        int bc_bigcharread(int* big) {
            FILE *in = fopen("bigchar.bin", "r");
            if(!in) {
                printf("Can not open file 'bigchar.bin' for read!\n");
                return -1;
            }

            fscanf(in, "%ld%ld", &big[0], &big[1]);
            fclose(in);
            return 0;
        }
    };

}





int main(){

    TermLinux::MyTerm l;
    l.mt_setfgcolor(TermLinux::RED);
	l.mt_clrscr();

	cout << "Hello\n";
    return 0;
}
