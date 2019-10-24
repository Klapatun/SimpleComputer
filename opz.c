/*  opz.c - файл, содержащий функции для перевода математического выражения в обратной польской записи  */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "opz.h"
#include "variables.h"


#define AVOID_SPACE(exp)    while(exp[0] == ' ') {\
                                exp++;\
                            }

static struct opz_stack* opz_stack_init(void) {
    return opz_stack_push(NULL, 0);
}
static void opz_stack_free_all(struct opz_stack* stack) {
    if (stack->deeper) {
        opz_stack_free_all(stack->deeper);
    }

    free(stack);
}

static struct opz_stack* opz_stack_push(struct opz_stack* last_element, char operator) {
	
    struct opz_stack* tmp = (struct opz_stack*)malloc(sizeof(struct opz_stack));

    tmp->deeper = last_element;
    tmp->variable = operator;


    return tmp;
}

static struct opz_stack* opz_stack_pull(struct opz_stack* stack, char* p_var) {
	
    struct opz_stack* tmp = stack->deeper;
    *p_var = stack->variable;

    free(stack);

    return tmp;
}

static int opz_stack_priority(char var) {
    switch (var)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    default:
        return 0;
    }
}

static int opz_stack_search_open_bracket(struct opz_stack* last) {
    if(last->variable != '(') {

        if(last->deeper == NULL) {
            return -1;
        }

        return opz_stack_search_open_bracket(last->deeper) + 1;
    }
    else {
        return 1;
    }
}


static int opz_stack_search_less(struct opz_stack* last, char var_new) {

    if(opz_stack_priority(last->variable) >= opz_stack_priority(var_new)) {

        if(last->deeper == NULL) {
            return 1;
        }

        return opz_stack_search_less(last->deeper, var_new) + 1;

    }
    else {
        return 1;
    }
}

/*возвращает: -1 - нету открыв скобки, 
               0 - нормально; 
               * (любое другое значение) - кол-во необходимых pull перед*/
static int opz_stack_add(struct opz_stack* stack, char variable) {

    //если стек пустой
    if(stack == NULL) {
        stack = opz_stack_push(NULL, variable);
        return 0;
    }

    //Если это ")",ищем, сколько надо будет сделать pull
    if(variable == ')') {
        return opz_stack_search_open_bracket(stack);
    }

    //Сравниваем приоритет операций
    int var_new = opz_stack_priority(variable);

    if (var_new == 0) {
        //Значит это открывающая скобка "("
        stack = opz_stack_push(stack, variable);
        return 0;
    }

    int var_stack = opz_stack_priority(stack->variable);

    if (var_stack >= var_new) {
        //Выдаем сколько надо вытащить из стека
        return opz_stack_search_less(stack, var_new);
    }
    else {
        //помещаем в стек
        stack = opz_stack_push(stack, variable);
        return 0;
    }

}


int opz (char* expression_before, char* expression_after) { //обратная польская запись
	
    //Существует 3 фазы:
    //1 - фаза, в которой устанавливается переменная, в которую записывается результат, если слева стоит не переменная, то ошибка
    //2 - фаза, в которой должен устанавливается знак "=", если в эту фазу встретится что-то еще, кроме пробела, будет ошибка
    //3 - фаза, в которой происходит само преобразование в опз
    int phase = 0;

    int num_after = 0;
    int exp_before_size = strlen(expression_before);

    struct opz_stack* stack = NULL;

    for (int i=0; i<exp_before_size; i++) {

        switch (expression_before[i])
        {
        case ' ':
            
            break;

        case '=':

            //первая фаза или третья
            if(phase == 0 || phase == 2) {
                //Ошибка
                return 0;
            }

            //вторая фаза - время для "=", записываем в expression_after
            expression_after[num_after++] = expression_before[i];

            break;

        case '(':
            if (!opz_stack_add(stack, '(')) { //если вернули не 0,то что-то не так
                //Ошибка
                return 0;
            }
            break;

        case ')':
            int res = opz_stack_add(stack, ')');

            if (res == -1) {    //нету открыв скобки
                //Ошибка
                return 0;
            }
            else if (res == 0) {    //если ноль, то косяк
                //Ошибка
                return 0;
            }
            else {  //а тут нормально
                //Надо считать данные
            }
            break;
        case '+':

            break;

        case '-':

            break;

        case '*':

            break;

        case '/':

            break;

        default:

            //первая фаза
            if(phase == 0) {
                //Если это не переменная, выдаем ошибку
                if(!value_cheker(expression_before[i])) {
                    //Ошибка, слева не переменная
                    return 0;
                }
                expression_after[num_after++] = expression_before[i];
                break;
            }

            //вторая фаза
            if(phase == 1) {
                //Так как во второй фазе возможно появление только =, то ошибка
                return 0;
            }

            //если сюда дошло, значит это третья фаза
            if(value_cheker(expression_before[i])) {
                //если сюда зашло, значит это переменная
                expression_after[num_after++] = expression_before[i];
                break;
            }
            //а это число
            char number[6] = {0};
            
            for(int j=0; j <= 5; j++) {
                if(expression_before[i+j] >= '0' && expression_before[i+j] <= '9') {
                    expression_after[num_after++] = expression_before[i+j];
                }
                else {
                    if(j && j<5) {
                        break;
                    }
                    //Ошибка, 1. некорректное значение 2. значение j = 5, значит число превышает
                    return 0;
                    
                }
            }

            break;
        }
    }


/*
    //Если вдруг решили поставить много пробелов
    AVOID_SPACE(expression_before);

    //Проверяем, есть ли такая переменная
    //Если нету, то печально, числам присваивать ничего нельзя
    int addr_left = value_cheker(expression_before[0]);

    if (!addr_left) {
        //Ошибка, такой переменной не существует
        return 0;
    }

    expression_after[count++] = expression_before[0];
    expression_before++;

    //Если вдруг решили поставить много пробелов
    AVOID_SPACE(expression_before);


    //Поиск знака равно
    if (expression_before[0] != '=') {
        //Ошибка, должен быть знак =
        return 0;
    }
    expression_after[count++] = expression_before[0];
    expression_before++;

    //Если вдруг решили поставить пробелы
    AVOID_SPACE(expression_before);




	for (int i=0; i<strlen(expression_before); i++) {

	}
*/
	return strlen(expression_after);
}