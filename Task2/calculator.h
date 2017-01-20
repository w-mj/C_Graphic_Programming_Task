#ifndef __CALCULATOR_H_
#define __CALCULATOR_H_
#include "tree.h"

tree transTree(const char *expression);
double calculate(const tree expression, double x);

#endif
