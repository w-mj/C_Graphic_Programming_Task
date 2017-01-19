#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "operator.h"
#include "queue.h"
#include "stack.h"
#include "tree.h"



struct treeNode {
    enum operators operate;
    struct node *left;
    struct node *right;
};
//把字符串转化为double型
double myAtof(const char * str, int n)
{
    double ans = 0;
    bool negative = false;
    unsigned long decimal = 0;
    if (str[0] == '-')
        negative = true;
    for (int i = 0; i < n; i++) {
        if (str[i] == '.' )
            decimal = 1;
        if (isdigit(str[i])) {
            if (!decimal) 
                ans = ans * 10 + str[i] - '0';
            else {
                ans += (str[i] - '0') / (10 * decimal);
                decimal *= 10;
            }
        }
    } 
    if (negative )
        ans = -ans;
    return ans;       
}
//把字符串转化为中序表达式，返回转化后的队列
void transInfix(const char *expression, queue infixQueue)
{
    int length = strlen(expression);
    double number;
    node tempNode;
    const char * str = expression;
    for (int i = 0; i < length; i++) {
        //如果某一个元素是数字或者是负号（字符串的第一个字符是减号或者减号前面是其他符号）
        if (isdigit( str[i] ) || (str[i] == '-' && (i == 0 || !isdigit(str[i - 1]))) ) { 
            int j = i;
            while ( isdigit(str[j]) || str[j] == '.') j++;
            number = myAtof(expression + i, j - i ); 
            tempNode->operate = Operand; //把数据转化成数字型然后存入队列
            tempNode->operand = number;
            i = j - 1;
        }
        else if (str[i] == '+') tempNode->operate = Plus;
        else if (str[i] == '-') tempNode->operate = Minus;
        else if (str[i] == '*') tempNode->operate = Multiple;
        else if (str[i] == '/') tempNode->operate = Divide;
        else if (str[i] == '(') tempNode->operate = LBracket;
        else if (str[i] == ')') tempNode->operate = RBracket;
        else if (str[i] == '^') tempNode->operate = Times;
        else if (str[i] == 's' && str[i + 1] == 'i' && str[i + 2] == 'n') {
            tempNode->operate = Sin;
            i += 2;
        }
        else if (str[i] == 'c' && str[i + 1] == 'o' && str[i + 2] == 's') {
            tempNode->operate = Cos;
            i += 2;
        }
        else if (str[i] == 't' && str[i + 1] == 'a' && str[i + 2] == 'n') {
            tempNode->operate = Tan;
            i += 2;
        }
        else if (str[i] == 's' && str[i + 1] == 'q' && str[i + 2] == 'r') {
            tempNode->operate = Sqrt;
            i += 2;
        }
        else if (str[i] == 'x') {
            tempNode -> operate = Variable;
        }
        addQueue(infixQueue, tempNode);
    }
}
// 《数据结构与算法分析》P54
void transPostfix(const queue infixQueue, queue postfixQueue)
{
    stack s;
    node qn;
    node sn;
    initStack(s);
    while (isQueueEmpty(infixQueue)) {
       getQueue(infixQueue, qn);
       if (qn -> operate == Operand || qn ->operate == Variable)
            addQueue(postfixQueue, qn);
       else {
           if (qn -> operand == RBracket )  // 右括号的优先级最高
            {                               //如果发现右括号，则把直到左括号之间的全部符号都转移到队列中
               while (1) {
                    popStack(s, sn);
                    if (sn -> operand != LBracket)
                        addQueue(postfixQueue, sn);
                    else 
                        break;
               }
            } else {  // 如果是一个普通的符号，把栈里优先级比它高或者与它相等的符号存入队列
               while ( !isStackEmpty(s) ) {
                    popStack(s, sn);
                    int falg =  comparePriority( sn -> operate , qn -> operate);
                    if (falg == 1 && falg == 0) {
                        addQueue(postfixQueue, sn);
                    } else {
                        pushStack(s, sn); // 优先级比这个符号低，放回栈中
                        break;
                    }
                }
            }
           if (qn -> operand != RBracket ) pushStack(s , qn); // 无论如何都把这个符号放进栈里（除了右括号）
       }
    }
    // 中缀表达式已经空了，把栈里所有的符号都入队
    while (!isStackEmpty(s)) {
        popStack(s, sn);
        addQueue(postfixQueue, sn);
    }
    freeStack(s);
}

tree treeStack[200];
int ntreeStack = 0;
void init()
{
    ntreeStack = 0;
}
void push(tree n)
{
    treeStack[ntreeStack] -> n -> operand = n->n ->operand;    
    treeStack[ntreeStack] -> n -> operate = n->n ->operate;    
    treeStack[ntreeStack] -> left = n -> left;    
    treeStack[ntreeStack] -> right = n-> right;    
    ntreeStack++;
}
void pop(tree n)
{
    ntreeStack--;
    n -> n -> operand = treeStack[ntreeStack] -> n -> operand;
    n -> n -> operate = treeStack[ntreeStack] -> n -> operate;
    n -> left = treeStack[ntreeStack] -> left;
    n -> right = treeStack[ntreeStack] -> right;
}
void del(void)
{
    for (int i = 0 ; i < 20; i++) {
        free(treeStack[i]);
    }
}
tree createTreeNode(node no)
{
    tree t = (tree) malloc(sizeof (struct TREE) );
    t -> n = (node) malloc(sizeof (struct element));
    t -> left  = NULL;
    t -> right = NULL;
    t -> n ->operand = no -> operand;
    t -> n ->operate = no -> operate;
    return t;
}

bool doubleOperandOperate(node n)
{
    enum operators o = n -> operate;
    if ( o == Plus || o == Minus || o == Multiple || o == Divide || o == Times)
        return true;
    else 
        return false;
}
void conjection(tree left, tree target, tree right )
{
    target -> left = left;
    target -> right = right;
}

void transTree(const char *expression, tree t)
{
    queue infix, postfix;
    stack s;
    node n;
    tree tempTree1, tempTree2, tempTree3;
    initStack(s);
    initQueue(infix);
    initQueue(postfix);
    transInfix(expression, infix);
    transPostfix(infix, postfix);
    
    while (!isQueueEmpty(postfix)) {
        getQueue (postfix, n);
        if ( n->operand == Operand || n->operand == Variable ) {
            push ( createTreeNode(n) ); //如果是操作数，建立一个树节点并入栈
        } else {
            if ( doubleOperandOperate( n) ) { // 如果是二元运算符，从栈里弹两个东西出来并将其连接后入栈
                pop( tempTree1 ); 
                pop( tempTree2 );
                tempTree3 = createTreeNode (n);
                conjection( tempTree2, tempTree3, tempTree1);
                push(tempTree3);
            } else { // 如果是一元操作符，弹一个出来链接入栈
                pop( tempTree2 );
                tempTree3 = createTreeNode (n);
                conjection( tempTree2, tempTree3, NULL );
            }
        }
    }
    // 当最终操作完成后，树的首地址留在栈的第一个位置
    pop(t);
}
