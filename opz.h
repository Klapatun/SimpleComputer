/* opz.h */

#ifndef OPZ_H
#define OPZ_H

struct opz_stack {
	struct opz_stack* deeper;
	char variable;
};

int opz (char* expression_before, char* expression_after);

#endif /*OPZ_H*/