/*
**
**
*/
#include <string.h>
#include <sys/ioctl.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

/*#define ESC	"\033"*/


class TermLinux {
	private:
		const char* CLEAR = "\E[2J";
		const char* GOTOXY = "\E%d;%dH";
		const char* SETFCOLOR = "\E[3%dm";
		const char* SETBCOLOR = "\E[4%dm";
		const char* ESCSPACE = "\E[1C";
		const char* ESC = "\E";
	public:

		void home(void) {
			cout << ESC << "[H";
		}

		int mt_clrscr(void) {

			int term = open("/dev/tty", O_RDWR);

			if (term == -1) {
				cerr << "Terminal Error!";
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
				cerr << "Terminal Error!";
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
			return 0;
		}

		//int mt_setfgcolor (enum colors) {
		//	return 0;
		//}

		//int mt_setbgcolor (enum colors) {
		//	return 0;
		//}
		
		TermLinux(void) {

		}
};

int main(){

	TermLinux l;

	l.mt_clrscr();

	cout << "Hello\n";
    return 0;
}
