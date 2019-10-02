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
                    cerr << "mt_clrscr: Terminal Error!";
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
                    cerr << "mt_gotoXY: Terminal Error!";
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
                    cerr << "mt_getscreensize: Error!";
                    return -1;
                }

                return 0;
            }

            int mt_setfgcolor (uint8_t color) {

                int term = open("/dev/tty", O_RDWR);

                if (term == -1) {
                    cerr << "mt_setfgcolor: Terminal Error!";
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
                    cerr << "mt_setbgcolor: Terminal Error!";
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

    class MyBigChars {

    private:


    public:

        int bc_printa(const char* str) {
            return 0;
        }

        int bc_box(int x1, int y1, int x2, int y2) {
            return 0;
        }
        /*
        int bc_printbigchar(const int* src, int x, int y, enum Colors font, enum Colors bgoaund) {
            return 0;
        }
        */
        int bc_setbigcharpos(int *big, int value) {
            return 0;
        }

        int bc_getbigcharpos(const int *big, int* value) {
            return 0;
        }

        int bc_bigcharwrite(const int* big) {
            return 0;
        }

        int bc_bigcharread(int* big) {
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
