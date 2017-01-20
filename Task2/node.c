#include "node.h"
#include <stdlib.h>

node initNode(void)
{
    node a;
    a = (node)malloc(sizeof(struct element));
    return a;
}

void freeNode(node a)
{
    free (a);
}
