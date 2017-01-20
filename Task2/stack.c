#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "node.h"

stack initStack(void)
{
    stack a;
    a = (stack)malloc(sizeof (struct STACK));
    a -> n = 0;
    a -> max = 200;
    for (int i = 0; i < 200; i++) {
        a -> e[i] = initNode();
    }
    return a;
}

bool pushStack(stack s, node n)
{
   if ( s->n == s->max ) {
       fprintf(stderr, "Stack is full.\n");
       return false;
    }
    s -> e[s -> n] -> operate = n -> operate;
    s -> e[s -> n] -> operand = n -> operand;
    s -> n += 1;
    return true;
}

bool popStack(stack s, node tar)
{
    if (s -> n == 0) {
        fprintf(stderr, "Stack is empty.\n");
        return false;
    }
    s -> n -= 1;
    tar -> operate = s -> e[s -> n] -> operate;
    tar -> operand = s -> e[s -> n] -> operand;
    return true;
}

bool isStackEmpty(stack s)
{
    if (s -> n == 0)
        return true;
    else
        return false;
}

void emptyStack(stack s)
{
    s -> n = 0;
}

void freeStack(stack s)
{
    for (int i = 0 ;i < 200; i++) {
        freeNode( s -> e[i] );
    }
    free(s);
}
