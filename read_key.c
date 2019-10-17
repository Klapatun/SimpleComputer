#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include "mySimpleComputer.h"
#include "read_key.h"
#include "u_io.h"
#include "myTerm.h"

#define BUFSIZE 50

struct termios const __term_state;

void termInit()
{
    termSave(&__term_state);
}

void getDefaultTermSettings(struct termios *termState)
{
    *termState = __term_state;
}

struct termios termState;

int readKey (enum Keys *key)
{
    int term = open("/dev/tty", O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "readKey: terminal error!");
        close (term);
        return -1;
    }

    char buf;

	struct termios termState = __term_state;
    termRegime(&termState, 0, 0, 1, 0, 1);
    read (term, &buf, 1);

    switch (buf) {
    case 'q':
        *key = K_Q;
        break;
    case 'l':
        *key = K_L;
        break;
    case 's':
        *key = K_S;
        break;
    case 'r':
        *key = K_R;
        break;
    case 't':
        *key = K_T;
        break;
    case 'i':
        *key = K_I;
        break;
    case 'e':
		*key = K_E;
		break;
    case ESC:        
        read (term, &buf, 1);
        read (term, &buf, 1);
        switch (buf) {
        case 'A':
            *key = K_UP;
            break;
        case 'B':
            *key = K_DOWN;
            break;
        case 'C':
            *key = K_RIGHT;
            break;
        case 'D':
            *key = K_LEFT;
            break;
        case '1':
            read (term, &buf, 1);
            if (buf == '5') {
                read (term, &buf, 1);
                if (buf == '~') *key = K_F5;
            }
            if (buf == '7') {
                read (term, &buf, 1);
                if (buf == '~') *key = K_F6;
            }
            break;
        default:
            *key = K_UNKNOWN;
        }
        break; // case ESC
    default:
        *key = K_UNKNOWN;
        break;
    }
    termRegime(&termState, 1, 1, 1, 1, 1);
    close (term);
    return 0;
}

int termSave (struct termios *termState)
{
    return tcgetattr(1, termState);
}

int termRestore (struct termios const *currState)
{
    return tcsetattr(1, TCSADRAIN, &termState);
}

int termRegime (struct termios const *currState,
                int regime, int vtime, int vmin, int echo, int sigint)
{
    if (regime > 1 || sigint > 1 || echo > 1 ||
        regime < 0 || sigint < 0 || echo < 0)
    {
        return 1;
    }

    if (vtime < 0 || vmin < 0)
        return 1;

    struct termios newTermState = *currState;

    if (regime)
        newTermState.c_lflag |= ICANON;
    else
        newTermState.c_lflag &= ~ICANON;

    if (echo)
        newTermState.c_lflag |= ECHO;
    else
        newTermState.c_lflag &= ~ECHO;

    if (sigint)
        newTermState.c_lflag |= ISIG;
    else
        newTermState.c_lflag &= ~ISIG;

    newTermState.c_cc [VMIN]  = vmin;
    newTermState.c_cc [VTIME] = vtime;
    tcsetattr(0, TCSANOW, &newTermState);

    return 0;
}
