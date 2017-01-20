#ifndef _STACK_H_
#define _STACK_H_
#include "operator.h"
#include <stdbool.h>
#include "node.h"

struct STACK {
    node e[200];
    int n;
    int max;
};
typedef struct STACK* stack;

stack initStack(void);
bool pushStack(stack s, node n);
bool popStack(stack s, node tar);
bool isStackEmpty(stack s);
void emptyStack(stack s);
void freeStack(stack s);

#endif
