/*
**
**
*/
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>

using namespace std;

/*#define ESC	"\033"*/


class TermLinux {
	private:
		const string ESC = "\033";

	public:

		void home(void) {
			cout << ESC + "[H";
		}

		void clrscr(void) {
			cout << ESC + "[2J";
		}


		TermLinux(void) {

		}
};

int main(){

	TermLinux l;

	l.home();
	l.clrscr();

	cout << "Hello\n";
    return 0;
}
