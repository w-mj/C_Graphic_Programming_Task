#include "tree.h"
#include <stdlib.h>
#include "node.h"

tree initTree(void)
{
    tree t;
    t =  (tree)malloc(sizeof(struct TREE));
    t -> n = initNode();
    t -> left = NULL;
    t -> right = NULL;
}

void freeTree(tree t)
{
    if (t -> left != NULL)
        freeTree(t -> left);
    if (t -> right != NULL)
        freeTree(t -> right);

    freeNode( t -> n);
    free(t);
}
