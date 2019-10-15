/*
**
**
*/
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
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

    class mySimpleComputer {
    private:

        const uint8_t MEMSIZE = 100;
        const uint8_t COMMANDS_COUNT = 44;
        int ram[100];

        uint8_t FLAGS;
        enum {
            OF = 1,				// overflow flag
            ZF, 				// zero flag
            IF,     			// ignoring flag
            OM, 				// out of memory
            BC, 				// bad command
            IR          		// is run
        };

        int commands[37] = {
            0x10,
            0x11,
            0x20,
            0x21,
            0x30,
            0x31,
            0x32,
            0x33,
            0x40,
            0x41,
            0x42,
            0x43,
            0x51,
            0x52,
            0x53,
            0x54,
            0x55,
            0x56,
            0x57,
            0x58,
            0x59,
            0x60,
            0x61,
            0x63,
            0x64,
            0x65,
            0x66,
            0x67,
            0x68,
            0x69,
            0x70,
            0x71,
            0x72,
            0x73,
            0x74,
            0x75,
            0x76
        };

        int accumulator;            //аккумулятор
        int instructionCounter;     //предыдущая команда
        int operand;				// операнд
        int command;				// команда

        int f_key, f_ign;

        struct itimerval nval, oval;

    public:

        int mem_ptr;				// указатель на ячейку памяти

        int sc_memoryInit()
        {
            int i = 0;
            for(i = 0; i < MEMSIZE; ++i)
                ram[i] = 0;

            accumulator             = 0;
            instructionCounter		= 0;
            command                 = 0;
            operand                 = 0;
            mem_ptr                 = 0;

            return 0;
        }

        int sc_memorySet(int address, int value)
        {
            if( (address < MEMSIZE) && (address >= 0) ) {
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

            cerr << "Обшибка в файле" << file << "," << "на строчке: " << line;

            return 0;
        }

        /* run timer */
        void timerStart()
        {
            setitimer (ITIMER_REAL, &nval, &oval);
        }

    };

    class MyTerm {
        private:
            const char* CLEAR = "\E[2J";
            const char* GOTOXY = "\E[%d;%dH";
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
                char buff[50] = {0};

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


        const char* SPACE           = " ";
        const char* ACS_CKBOARD     = "a";

        const char* LT              = "l"; // left top
        const char* LB              = "m"; // left bottom
        const char* RT              = "k"; // right top
        const char* RB              = "j"; // right bottom
        const char* VL              = "x"; // vert line
        const char* HL              = "q"; // horis line

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
            bc_printA(LT);
            for(int i=0; i < y2-2; ++i) {
                bc_printA(HL);
            }

            bc_printA(RT);
            for(int i=1; i <= x2-2; ++i) {
                mt_gotoXY(x1+i, y1);
                bc_printA(VL);
                mt_gotoXY(x1+i, y1+y2-1);
                bc_printA(VL);
            }

            mt_gotoXY(x1+x2-1, y1);
            bc_printA(LB);
            for(int i=0; i < y2-2; ++i) {
                bc_printA(HL);
            }

            bc_printA(RB);

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
                        bc_printA(ACS_CKBOARD);
                    } else {
                        bc_printA(SPACE);
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
    TermLinux::MyBigChars bigChar;

    int term = open("/dev/tty", O_RDWR);
        if (term == -1) {
            cerr <<"displayBorders: terminal error! \n";
            close(term);
            return -1;
        }

        l.mt_clrscr();

        bigChar.bc_box(1, 1, 12, 61);
        bigChar.bc_box(1, 62, 3, 22);
        bigChar.bc_box(4, 62, 3, 22);
        bigChar.bc_box(7, 62, 3, 22);
        bigChar.bc_box(10, 62, 3, 22);
        bigChar.bc_box(13, 1, 10, 46);
        bigChar.bc_box(13, 47, 10, 37);

        l.mt_gotoXY(1, 30);
        write(term, " Memory ", strlen(" Memory "));
        l.mt_gotoXY(1, 67);
        write(term, " accumulator ", strlen(" accumulator "));
        l.mt_gotoXY(4, 63);
        write(term, " instructionCounter ", strlen(" instructionCounter "));
        l.mt_gotoXY(7, 68);
        write(term, " Operation ", strlen(" Operation "));
        l.mt_gotoXY(10, 69);
        write(term, " Flags ", strlen(" Flags "));
        l.mt_gotoXY(13, 48);
        write(term, " Keys: ", strlen(" Keys: "));

        l.mt_gotoXY(14, 48);
        write(term, "l  - load", strlen("l  - load"));
        l.mt_gotoXY(15, 48);
        write(term, "s  - save", strlen("s  - save"));
        l.mt_gotoXY(16, 48);
        write(term, "r  - run", strlen("r  - run"));
        l.mt_gotoXY(17, 48);
        write(term, "t  - step", strlen("t  - step"));
        l.mt_gotoXY(18, 48);
        write(term, "e  - edit cell", strlen("e  - edit cell"));
        l.mt_gotoXY(19, 48);
        write(term, "i  - reset", strlen("i  - reset"));
        l.mt_gotoXY(20, 48);
        write(term, "F5 - accumulator", strlen("F5 - accumulator"));
        l.mt_gotoXY(21, 48);
        write(term, "F6 - instructionCounter", strlen("F6 - instructionCounter"));

        l.mt_gotoXY(23, 1);
        close(term);


        {
            TermLinux::mySimpleComputer Simple;
            int term = open("/dev/tty", O_RDWR);

            if(term == -1) {
                cerr << "displayMemory: Terminal Error!\n";
                close(term);
                return -1;
            }

            int value, comm, oper, k;

            l.mt_gotoXY(2, 2);

            int j = 2;

            for (int i=1; i <= 100; ++i) {
                Simple.sc_memoryGet(i-1, &value);

                k = (value >> 14) & 0x1;
                oper = value & 0x7F;
                comm = value >> 7 & 0x7F;

                char buff[50];

                if(k) {
                    sprintf(buff, i%10 ? " %04X " : " %04X", value & 0x1FFF );
                }
                else {
                    sprintf(buff, i%10 ? "%c%02X%02X " : "%c%02X%02X", (k == 0) ? '+' : ' ', comm, oper);
                }

                write(term, buff, strlen(buff));

                if(i%10 == 0 && i != 0) {
                    ++j;
                    l.mt_gotoXY(j,2);
                }

            }

            char str_1[50];
            l.mt_gotoXY(Simple.mem_ptr/10+2, Simple.mem_ptr%10*6+2);

            Simple.sc_memoryGet(Simple.mem_ptr, &value);

            k = value >> 14 & 0x1;
            oper = value & 0x7F;
            comm = value >> 7 & 0x7F;

            if(k) {
                sprintf(str_1, " %04X", value & 0x1FFF);
            }
            else {
                sprintf(str_1, "%c%02X%02X", !k ? '+' : ' ', comm, oper);
            }

            l.mt_setbgcolor(TermLinux::GREEN);
            write(term, str_1, strlen(str_1));
            l.mt_setbgcolor(TermLinux::DEFAULT);
            l.mt_gotoXY(23, 1);

            close(term);

        }


    return 0;
}
