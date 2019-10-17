#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "myBigChars.h"
#include "myTerm.h"
#include "read_key.h"
#include "ui.h"
#include "mySimpleComputer.h"
#include "u_io.h"
#include "read_key.h"
#include "alu_cu.h"
#include "assembler.h"

void processing(void);

int main()
{
    signal (SIGALRM, signalhandler);
    signal (SIGUSR1, reset);

    nval.it_interval.tv_sec  = 1;
    nval.it_interval.tv_usec = 0;
    nval.it_value.tv_sec  = 1;
    nval.it_value.tv_usec = 0;	
	
    sc_regInit();
    termInit();
    ax = 0x0;
    ip = 0x0;

    enum Keys key = K_UNKNOWN;
	
	displayBorders();
    displayUI();
    
    f_key = 0;
    f_ign = 0;
    
    while (key != K_Q) {
		
		readKey (&key);
		
		if (key == K_T) {
			sc_regSet(IR, 0);
			alarm(0); 
			f_key = 0;
			cu();
		}
		
		if (!f_key) {
			if (key == K_R    )  {
				sc_regSet(IR, 1);
				sc_regSet(IF, 0);
			}
			
			sc_regGet(IR, &f_key);
			
			if (f_key) {
				timerStart();        
			}
			
			if (key == K_F5   ) user_input_ax();
			if (key == K_F6   ) user_input_ip();
			if (key == K_RIGHT) if (mem_ptr < 99) ++mem_ptr;
			if (key == K_LEFT ) if (mem_ptr >  0) --mem_ptr;
			if (key == K_UP   ) if (mem_ptr - 10 >=  0) mem_ptr -= 10;
			if (key == K_DOWN ) if (mem_ptr + 10 < 100) mem_ptr += 10;
			if (key == K_L	  ) sc_memoryLoad("memory.o");
			if (key == K_S	  ) sc_memorySave("memory.o");
			if (key == K_E	  ) {
				processing();
				//user_input_mem();
			}
			if (key == K_I) {
				displayBorders();
				displayUI();
				sc_memoryInit();
				sc_regInit();
			}
			displayUI ();
		}
		
	}
    
	return 0;
}



void processing(void) {

	int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "processing: terminal error");
        close (term);
        return;
    }

    char buff[50] = {0};

    write(term, "Enter > ", 8);
    read(term, buff, 50);
    //sscanf(buff, "%s %s %s", command, asm_file, obj_file);

    if (!strncmp(buff, "sat", 3)) {
    	//char command[3] = {0};
    	char asm_file[22] = {0};
    	char obj_file[22] = {0};

    	sscanf(&buff[4], "%s %s", asm_file, obj_file);

    	char extension_1[3] = {0};
    	char extension_2[3] = {0};
    	char name[22] = {0};
    	asm_processing(asm_file, obj_file);
    }
    else if (!strncmp(buff, "run", 3)) {
    	char file_name[30] = {0};
    	sscanf(&buff[4], "%s", file_name);

    	//char ram_dump[MEMSIZE];
    	//memcpy(ram_dump, ram, MEMSIZE);
    	sc_memoryLoad(file_name);

    	sc_regSet(IR, 1);
		sc_regSet(IF, 0);
		//return;


		//memcpy(ram, ram_dump, MEMSIZE);

    }
    else {
    	clearInput();
    	write(term, "Bad command", 11);
    	return;
    }

    clearInput();
    close(term);
}