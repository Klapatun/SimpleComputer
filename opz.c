/*  opz.c - файл, содержащий функции для перевода математического выражения в обратной польской записи  */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "opz.h"
#include "variables.h"


#define AVOID_SPACE(exp)    while(exp[0] == ' ') {\
                                exp++;\
                            }

static void opz_stack_free_all(struct opz_stack* stack);
static struct opz_stack* opz_stack_push(struct opz_stack* last_element, char operator);
static struct opz_stack* opz_stack_pull(struct opz_stack* stack, char* p_var);
static int opz_stack_priority(char var);
static int opz_stack_search_open_bracket(struct opz_stack* last);
static int opz_stack_search_less(struct opz_stack* last, char var_new);
static int opz_stack_add(struct opz_stack** p_stack, char variable);


static void opz_stack_free_all(struct opz_stack* stack) {
    if (stack->deeper) {
        opz_stack_free_all(stack->deeper);
    }

    free(stack);
}

static struct opz_stack* opz_stack_push(struct opz_stack* last_element, char operator) {
	
    struct opz_stack* tmp= NULL;
    
    if (last_element->variable != 0) {

        tmp = (struct opz_stack*)malloc(sizeof(struct opz_stack));
        tmp->deeper = last_element;
    }
    else {
        tmp = last_element;
    }

    tmp->variable = operator;


    return tmp;
}

static struct opz_stack* opz_stack_pull(struct opz_stack* stack, char* p_var) {
	
    struct opz_stack* tmp = stack->deeper;
    *p_var = stack->variable;

    if (tmp == NULL){
        stack->variable = 0;
        tmp = stack;
    }
    else {
        free(stack);
    }

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
    case '(':
        return 3;
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
        return 0;
    }
}

static struct opz_stack* opz_stack_init(void){
    struct opz_stack* tmp = (struct opz_stack*)malloc(sizeof(struct opz_stack));

    tmp->deeper = NULL;
    tmp->variable = 0;

    return tmp;
}

static void opz_stack_free(struct opz_stack* p_stack) {
    struct opz_stack* tmp;

    do {
        tmp = p_stack->deeper;
        free(p_stack);
        p_stack = tmp;
    }while(p_stack);
}

/*возвращает: -1 - нету открыв скобки, 
              -2 - p_stack равен нулю,
              -3 - нету такого значения,
               0 - нормально; 
               * (любое другое значение) - кол-во необходимых pull перед*/
static int opz_stack_add(struct opz_stack** p_stack, char variable) {
    struct opz_stack* tmp;

    if(*p_stack == NULL) {
        tmp = opz_stack_init();
        tmp->deeper = NULL;
        tmp->variable = 0;
        *p_stack = tmp;
        tmp = NULL;
    }

    tmp = *p_stack;

    //если стек пустой
    if(tmp->variable == 0) {
        tmp->variable = variable;
        //p_stack = opz_stack_push(NULL, variable);
        return 0;
    }

    //Если это ")",ищем, сколько надо будет сделать pull
    if(variable == ')') {
        return opz_stack_search_open_bracket(tmp);
    }

    //Сравниваем приоритет операций
    int var_new = opz_stack_priority(variable);

    int var_stack = opz_stack_priority(tmp->variable);

    if (var_new == 0 || var_stack == 0) {
        //Ошибка, переменная неизвестна
        return -3;
    }

    if (var_stack >= var_new) {
        //помещаем в стек
        //struct opz_stack* tmp; //= p_stack;
        *p_stack = opz_stack_push(tmp, variable);

        return 0;
    }
    else {
        //Выдаем сколько надо вытащить из стека
        return opz_stack_search_less(*p_stack, var_new);
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

    int res = 0;
    char var = 0;

    struct opz_stack* stack = opz_stack_init();

    for (int i=0; i<exp_before_size; i++) {

        switch (expression_before[i])
        {
        case ' ':
            
            break;

        case '=':

            //первая фаза или третья
            if(phase == 0 || phase == 2) {
                //Ошибка
                opz_stack_free(stack);
                return 0;
            }

            //вторая фаза - время для "=", записываем в expression_after
            expression_after[num_after++] = expression_before[i];
            phase++;
            break;

        case '(':
            if (opz_stack_add(&stack, '(')) { //если вернули не 0,то что-то не так
                //Ошибка
                opz_stack_free(stack);
                return 0;
            }
            break;

        case ')':
            
            res = opz_stack_add(&stack, ')');

            if (res == -1 || res == 0 || res == -3 || res == -2) {    
                //нету открыв скобки / ')' не должен записываться в стек / косяк с приоритетом / stack == 0
                //Ошибка
                opz_stack_free(stack);
                return 0;
            }
            else {  //а тут нормально
                //Надо считать данные
                char var = 0;
                //Выгружает из стека значения и присваивает expression_after
                for(; res>0; res--) {
                    stack = opz_stack_pull(stack, &var);
                    if(var != '(') {
                        expression_after[num_after++] = var;
                        expression_after[num_after++] = ',';
                    }
                }
            }
            break;
        case '+':
            if (phase == 0 || phase == 1) {
                //Ошибка
                opz_stack_free(stack);
                return 0;
            }

            //Третья фаза
            //int res = opz_stack_add(stack, '+');
            res = opz_stack_add(&stack, '+');
            var = 0;
            for(;res > 0;res--) {  //надо вытащить из стека
                stack = opz_stack_pull(stack, &var);
                expression_after[num_after++] = var;
                expression_after[num_after++] = ',';
            }
            //залить в стек
            if (var) {
                opz_stack_add(&stack, '+');
            }

            break;

        case '-':
            if (phase == 0 || phase == 1) {
                //Ошибка
                opz_stack_free(stack);
                return 0;
            }

            //Третья фаза
            int res = opz_stack_add(&stack, '-');
            char var = 0;
            for(;res > 0;res--) {  //надо вытащить из стека
                stack = opz_stack_pull(stack, &var);
                expression_after[num_after++] = var;
            }
            //залить в стек
            if (var) {
                opz_stack_add(&stack, '-');
            }

            break;

        case '*':
            if (phase == 0 || phase == 1) {
                //Ошибка
                opz_stack_free(stack);
                return 0;
            }

            //Третья фаза
            res = opz_stack_add(&stack, '*');
            var = 0;
            for(;res > 0;res--) {  //надо вытащить из стека
                stack = opz_stack_pull(stack, &var);
                expression_after[num_after++] = var;
            }
            //залить в стек
            if (var) {
                opz_stack_add(&stack, '*');
            }

            break;

        case '/':
            if (phase == 0 || phase == 1) {
                //Ошибка
                opz_stack_free(stack);
                return 0;
            }

            //Третья фаза
            res = opz_stack_add(&stack, '/');
            var = 0;
            for(;res > 0;res--) {  //надо вытащить из стека
                stack = opz_stack_pull(stack, &var);
                expression_after[num_after++] = var;
            }
            //залить в стек
            if (var) {
                opz_stack_add(&stack, '/');
            }

            break;

        default:

            //первая фаза
            if(phase == 0) {
                //Если это не переменная, выдаем ошибку
                if(!value_cheker(expression_before[i])) {
                    //Ошибка, слева не переменная
                    opz_stack_free(stack);
                    return 0;
                }
                expression_after[num_after++] = expression_before[i];
                phase++;
                break;
            }

            //вторая фаза
            if(phase == 1) {
                //Так как во второй фазе возможно появление только =, то ошибка
                opz_stack_free(stack);
                return 0;
            }

            //если сюда дошло, значит это третья фаза
            if(value_cheker(expression_before[i])) {
                //если сюда зашло, значит это переменная
                expression_after[num_after++] = expression_before[i];
                expression_after[num_after++] = ',';
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

                    if (expression_before[i+j] == '\n' || expression_before[i+j] == 0) {
                        while (stack->variable != 0) {
                            stack = opz_stack_pull(stack, &var);
                            expression_after[num_after++] = var;
                        }
                        opz_stack_free(stack);
                        break;
                    }

                    //Ошибка, 1. некорректное значение 2. значение j = 5, значит число превышает
                    opz_stack_free(stack);
                    return 0;
                    
                }
            }
            expression_after[num_after++] = ',';
            break;
        }
    }
/*
    while (stack != NULL) {
        stack = opz_stack_pull(stack, &var);
        expression_after[num_after++] = var;
    }
*/
	return strlen(expression_after);
}