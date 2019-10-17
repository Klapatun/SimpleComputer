#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "mySimpleComputer.h"
#include "read_key.h"
#include "u_io.h"
#include "ui.h"
#include "myTerm.h"
#include "alu_cu.h"


int alu(int command, int operand)
{	
	int fOM, fZF, fBC;
	int value = 0;
	
	switch (command) {
				
		case 0x30: //ADD		Выполняет сложение слова в аккумуляторе и слова из указанной ячейки памяти (результат в аккумуляторе)
			if (sc_memoryGet(operand, &value) == 0) {
				if (value > 0x1FFF) {
                    value = value & 0x1FFF; 
                }
                ax = ax + value;
                if (ax > 0x1FFF) {
                    ax = ax & 0x1FFF; 
                    sc_regSet(OF, 1); 
                }
            }
			break;
		case 0x31: //SUB		Вычитает из слова в аккумуляторе слово из указанной ячейки памяти (результат в аккумуляторе)
			if (sc_memoryGet(operand, &value) == 0) {
				if (value > 0x1FFF) {
                    value = value & 0x1FFF; 
                }
                ax = ax - value;
                if (ax > 0x1FFF) {
                    ax = ax & 0x1FFF; 
                    sc_regSet(OF, 1); 
                }
            }
			break;
		case 0x32: //DIVIDE		Выполняет деление слова в аккумуляторе на слово из указанной ячейки памяти (результат в аккумуляторе)
			if (sc_memoryGet(operand, &value) == 0) {
				if (value > 0x1FFF) {
                    value = value & 0x1FFF; 
                }
                if (value != 0) {
                    ax = ax / value;
                    if (ax > 0x1FFF) {
                        ax = ax & 0x1FFF; 
                        sc_regSet(OF, 1); 
                    }
                } else sc_regSet(ZF, 1); 
            }
			break;			
		case 0x33: //MUL		Вычисляет произведение слова в аккумуляторе на слово из указанной ячейки памяти(результат в аккумуляторе)
			if (sc_memoryGet(operand, &value) == 0) {
				if (value > 0x1FFF) {
                    value = value & 0x1FFF; 
                }
                ax = ax * value;
                if (ax > 0x1FFF) {
                    ax = ax & 0x1FFF; 
                    sc_regSet(OF, 1); 
                }
            }
			break;
			
		case 0x51: //NOT		Двоичная инверсия слова в аккумуляторе и занесение результата в указанную ячейку памяти
				ax = ~ax;
				sc_memorySet(operand, ax);
			break;

		case 0x52: //AND		Логическая операция И между содержимым аккумулятора и словом по указанному адресу (результат в аккумуляторе)
			if (sc_memoryGet(operand, &value) == 0) {
				if (value > 0x1FFF) {
                    value = value & 0x1FFF; 
                }
                ax = ax & value;
                if (ax > 0x1FFF) {
                    ax = ax & 0x1FFF; 
                    sc_regSet(OF, 1); 
                }
            }
			break;
		
		case 0x53: //OR 		Логическое ИЛИ между аккумулятором и содержимым по адресу операнда (результат в акумуляторе) 
			if (sc_memoryGet(operand, &value) == 0) {
				if (value > 0x1FFF) {
                    value = value & 0x1FFF; 
                }

                ax = ax | value;
                if (ax > 0x1FFF) {
                    ax = ax & 0x1FFF; 
                    sc_regSet(OF, 1); 
                }
			}
			break;

		case 0x54: //XOR 		Логическое исключающее ИЛИ между аккумулятором и содержимым по адресу операнда (результат в акумуляторе) 
			if (sc_memoryGet(operand, &value) == 0) {
				if (value > 0x1FFF) {
                    value = value & 0x1FFF; 
                }

                ax = ax ^ value;
                if (ax > 0x1FFF) {
                    ax = ax & 0x1FFF; 
                    sc_regSet(OF, 1); 
                }
			}
			break;

		default:
            sc_regSet(BC, 1);   
            alarm(0);         
			break;			
	}
	
	sc_regGet(OM, &fOM);
	sc_regGet(ZF, &fZF);
	sc_regGet(BC, &fBC);
	
	if (fOM || fZF || fBC) {
		sc_regSet(IF, 1);
		return -1;
	}
	else
		return 0;
}

int cu()
{
	int term = open(TERM, O_RDWR);	

	if (term == -1) {
		fprintf(stderr, "%s\n", "cu: terminal error!");
		close (term);
		return -1;
	}
	
					
	char buf[5];
	int fOM, fZF, fBC;
	int value = 0, command = 0, operand = 0;
	int res;
	struct termios oldState, termState;
	
	sc_regSet(BC, 0);
	sc_regSet(OF, 0);
	
	if ((sc_memoryGet(mem_ptr, &value) == 0) 
    	&& (sc_commandDecode(value, &command, &operand) == 0)) {
            switch (command)
            {
                case 0x10: //READ		Ввод с терминала в указанную ячейку памяти с контролем переполнения
					termSave(&oldState);
					getDefaultTermSettings(&termState);
					tcsetattr(1, TCSANOW, &termState);
					alarm(0);
					write(term, "Enter data > ", 14);
					read(term, buf, 5);
					sscanf(buf, "%d", &res);

					if (res > 0x1FFF) {
						res = res & 0x1FFF;
						sc_regSet(OF, 1);
					}

					sc_memorySet(operand, (1 << 14) | res);
					termRestore(&oldState);
					timerStart();
					++mem_ptr;
					clearInput();
					break;
				case 0x11: //WRITE		Вывод на терминал значение указанной ячейки памяти
										
					sc_memoryGet(operand, &value);
					sprintf(buf, "%04d", value & 0x1FFF);

					write(1, buf, sizeof(buf));
					++mem_ptr;
					//clearInput();
					close(term); 
					break;
					
				case 0x20: //LOAD		Загрузка в аккумулятор значения из указанного адреса памяти
					if (sc_memoryGet(operand, &value) == 0) {
                        ax = value;
                        if (ax > 0x1FFF) {
							ax = ax & 0x1FFF;
						}
                        ++mem_ptr;
                    }
					break;
				case 0x21: //STORE		Выгружает значение из аккумулятора по указанному адресу памяти
					sc_memorySet(operand, ax);
                    ++mem_ptr;
					break;
								
				case 0x40: //JUMP		Переход к указанному адресу памяти
					mem_ptr = operand;
					break;
				case 0x41: //JNEG		Переход к указанному адресу памяти, если в аккумуляторе находится отрицательное число
					if ((ax >> 14) & 1)
						mem_ptr = operand;
                    else 
						++mem_ptr;
					break;
				case 0x42: //JZ			Переход к указанному адресу памяти, если в аккумуляторе находится ноль
					if (ax == 0)
						mem_ptr = operand;
                    else
						++mem_ptr;
					break;
				case 0x43: //HALT		Останов, выполняется при завершении работы программы
					sc_regSet(IR, 0);
					alarm(0);
					f_key = 0;
					break;

				case 0x55: //JNES		Переход по указанному адресу памяти, если в аккумуляторе положительное число
					if ((ax >> 14) & 1) {
						++mem_ptr;
					}
					else {
						mem_ptr = operand;
					}
					break;

				case 0x56: //JC			Переход по указанному адресу памяти, если при сложении произошло переполнение
					if (sc_regGet(OF, &value)) {
						mem_ptr = operand;
					}
					else {
						++mem_ptr;
					}
					break;

				case 0x57: //JNC		Переход по указанному адресу памяти, если при сложении не произошло переполне
					if (sc_regGet(OF, &value)) {
						++mem_ptr;
					}
					else {
						mem_ptr = operand;
					}
					break;

				case 0x58: //JP 		Переход по указанному адресу памяти, если результат предыдущей операции четный
					if ((ax%2) == 0) {
						mem_ptr = operand;
					}
					else {
						++mem_ptr;
					}
					break;

				case 0x59: //JNP		Переход по указанному адресу памяти, если результат предыдущей операции нечетный
					if ((ax%2) == 1) {
						mem_ptr = operand;
					}
					else {
						++mem_ptr;
					}
					break;

				default: // -> alu
                	if (alu(command, operand) == 0) 
                        ++mem_ptr;
                    
					break;
			}
    }
    
    sc_regGet(OM, &fOM);
	sc_regGet(ZF, &fZF);
	sc_regGet(BC, &fBC);
	
	close(term);
	if (fOM || fZF || fBC) {
		sc_regSet(IF, 1);
		return -1;
	}
	else
		return 0;
}
