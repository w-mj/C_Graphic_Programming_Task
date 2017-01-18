#ifndef _NODE_H_
#define _NODE_H_
#include "operator.h"

struct element {
    enum operators operate;
    double operand;
};
typedef struct element *node;

#endif
