#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "mySimpleComputer.h"
#include "read_key.h"
#include "u_io.h"
#include "ui.h"
#include "myTerm.h"

void user_input_ax()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "user_input_ax: terminal error");
        close (term);
        return;
    }

    char buf[5];
    write(term, "Enter AX (hex) > ", 17);
    read(term, buf, 5);
    sscanf(buf, "%x", &ax);

    clearInput();
    close(term);
}

void user_input_ip()
{
    int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "user_input_ip: terminal error");
        close (term);
        return;
    }

    char buf[5];
    write(term, "Enter IP (hex) > ", 17);
    read(term, buf, 5);
    sscanf(buf, "%x", &ip);
    if(ip >= 0 && ip < MEMSIZE)
		mem_ptr = ip;
	else
		sc_regSet(OM, 1);
    clearInput();
    close(term);
}

void user_input_mem()
{
	int term = open(TERM, O_RDWR);
    if (term == -1) {
        fprintf(stderr, "%s\n", "user_input_mem: terminal error");
        close (term);
        return;
    }
	
    char buf[5];
    int com = 0x0, oper = 0x0, value;
    sc_memoryGet(mem_ptr, &value);
    sc_commandDecode(value, &com, &oper);
    write(term, "Enter com,oper > ", 17);
    read(term, buf, 5);
    sscanf(buf, "%x%x", &com, &oper);
    sc_commandEncode(com, oper, &value);
    sc_memorySet(mem_ptr, value); 
    clearInput();
    close(term);
}
