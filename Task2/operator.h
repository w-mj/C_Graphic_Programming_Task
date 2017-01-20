#ifndef _OPERATOR_H_
#define _OPERATOR_H_
#define MAX_SAME_PRIORITY_OPERATOR 5 //最大同优先级运算符数目

enum operators
    {Plus , Minus, Multiple, Divide, Times, Sqrt, Sin, Cos, Tan, Operand, LBracket, RBracket , Variable, Log};

//比较两个运算符的优先级，如果op1的优先级比op2高，返回1，op1低于op2返回-1，相等返回0
int comparePriority(enum operators op1, enum operators op2 );

#endif
