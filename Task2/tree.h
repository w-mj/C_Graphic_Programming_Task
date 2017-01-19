#ifndef _TREE_H_
#define _TREE_H_
#include "node.h"
#include "operator.h"

struct TREE {
    node n;
    struct TREE *left;
    struct TREE *right;
};

typedef struct TREE *tree;


#endif
