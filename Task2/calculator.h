#ifndef __CALCULATOR_H_
#define __CALCULATOR_H_
#include "tree.h"

void transTree(const char *expression, tree t);
double calculate(const tree expression, double x);

#endif
