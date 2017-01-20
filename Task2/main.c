#include "calculator.h"
#include "tree.h"
#include <stdio.h>

int main(void) 
{
    double answer;
    tree t;
    char expression[] = "1+2*3+(4*5+6)*7";
    t = transTree(expression);
    answer = calculate(t, 0);
    printf("%f\n", answer);
    freeTree(t);
    return 0;
}
