#include "debug.h"
#include "node.h"
#include "queue.h"
#include "stack.h"
#include <stdio.h>

void print(node x)
{
    switch(x -> operate)
    {
        case Plus:
            putchar('+');break;
        case Minus:
            putchar('-');break;
        case Multiple:
            putchar('*');break;
        case Divide:
            putchar('/');break;
        case Times:
            putchar('^');break;
        case Sqrt:
            printf("sqrt");break;
        case Sin:
            printf("sin");break;
        case Cos:
            printf("cos");break;
        case Tan:
            printf("tan");break;
        case Operand:
            printf("%f", x -> operand);break;
        case Variable:
            putchar('x');break;
        case LBracket:
            putchar('(');break;
        case RBracket:
            putchar(')');break;
    }
}

void showQueue(queue q)
{
    for (int i = q -> s; i < q -> e; i++) {
        print(q -> q[i]);
    }
    printf("\n");
}

void showStack(stack s)
{
    for (int i = 0; i < s -> n; i++) {
        print(s -> e[i]);
    }
    printf("\n");
}
