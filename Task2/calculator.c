#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "operator.h"
#include "queue.h"
#include "debug.h"
#include "stack.h"
#include "tree.h"
#include "node.h"


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
    node tempNode = initNode();
    const char * str = expression;
    for (int i = 0; i < length; i++) {
        //如果某一个元素是数字或者是负号（字符串的第一个字符是减号或者减号前面是其他符号）
        if (isdigit( str[i] ) || (str[i] == '-' && (i == 0 || (!isdigit(str[i - 1]) && str[i - 1] != 'x'))) ) { 
            number = atof(&str[i]);
            int j = i;
            while ( isdigit(str[j]) || str[j] == '.' || str[j] == '-') j++;
            // number = myAtof(expression + i, j - i ); 
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
        else if (str[i] == 'l' && str[i + 1] == 'o' && str[i + 2] == 'g') {
            tempNode->operate = Log;
            i += 2;
        }
        else if (str[i] == 'x') {
            tempNode -> operate = Variable;
        }
        addQueue(infixQueue, tempNode);
    }
    freeNode(tempNode);
}
// 《数据结构与算法分析》P54
void transPostfix(const queue infixQueue, queue postfixQueue)
{
    stack s = initStack();
    node qn = initNode();
    node sn = initNode();
    while (!isQueueEmpty(infixQueue)) {
        getQueue(infixQueue, qn);
       if (qn -> operate == Operand || qn ->operate == Variable) {
            if (qn -> operate == LBracket) printf("ding1\n");
            addQueue(postfixQueue, qn);
        }
       else {
           if (qn -> operand == RBracket )  // 右括号的优先级最低
            {                               //如果发现右括号，则把直到左括号之间的全部符号都转移到队列中
               while (1) {
                    popStack(s, sn);
                    if (sn -> operand != LBracket) {
            if (sn -> operate == LBracket) printf("ding2\n");

                        addQueue(postfixQueue, sn);
                    }
                    else 
                        break;
               }
            } else {  // 如果是一个普通的符号，把栈里优先级比它高或者与它相等的符号存入队列
               while ( !isStackEmpty(s) ) {
                    popStack(s, sn);
                    int flag =  comparePriority( sn -> operate , qn -> operate);
                    //printf("####%d\n", flag);
                    if (sn -> operate != LBracket && (flag == 1 || flag == 0) ) {
                        if (sn -> operate == LBracket) printf("ding3\n");
                        addQueue(postfixQueue, sn);
                    } else {
                        pushStack(s, sn); // 优先级比这个符号低，放回栈中
                        break;
                    }
                }
                if (qn -> operand != RBracket ) pushStack(s , qn); // 无论如何都把这个符号放进栈里（除了右括号）
            }
       }
    }
    // 中缀表达式已经空了，把栈里所有的符号都入队
    while (!isStackEmpty(s)) {
        popStack(s, sn);
        if (sn ->operate == LBracket || sn->operate == RBracket )
            continue;
        if (sn -> operate == LBracket) printf("ding4\n");
        addQueue(postfixQueue, sn);
    }
    freeStack(s);
    freeNode(qn);
    freeNode(sn);
}

tree treeStack[200];
int ntreeStack = 0;
void init()
{
    for (int i = 0; i < 200; i++) {
        treeStack[i]->n = initNode();
    }
    ntreeStack = 0;
}
void push(tree n)
{
    //printf("Push tree stack ... ");
    //print(n->n);
    //printf("\n");    
    treeStack[ntreeStack] = n;    
    ntreeStack++;
}
tree pop(void)
{
    if (ntreeStack == 0) {
        fprintf(stderr, "Tree Stack is empty\n");
        return NULL;
    }
    //printf("Pop tree stack ... ");
    //print(treeStack[ntreeStack - 1]->n);
    //printf("\n");    
    ntreeStack--;
    return treeStack[ntreeStack];
}
void del(void)
{
    for (int i = 0 ; i < 20; i++) {
        freeNode(treeStack[i]->n);
    }
}
tree createTreeNode(node no)
{
    tree t = (tree) malloc(sizeof (struct TREE) );
    t -> n = initNode();
    t -> left  = NULL;
    t -> right = NULL;
    t -> n ->operand = no -> operand;
    t -> n ->operate = no -> operate;
    return t;
}

void conjection(tree left, tree target, tree right )
{
    target -> left = left;
    target -> right = right;
}

bool doubleOperandOperate(node n)
{
    enum operators o = n -> operate;
    if ( o == Plus || o == Minus || o == Multiple || o == Divide || o == Times)
        return true;
    else 
        return false;
}
tree transTree(const char *expression)
{
    queue infix = initQueue();
    queue postfix = initQueue();
    node n = initNode();
    tree tempTree1, tempTree2, tempTree3;
    tempTree1 = initTree();
    tempTree2 = initTree();
    tempTree3 = initTree();
    //printf("%p\n", infix);
    transInfix(expression, infix);
    showQueue(infix);
    transPostfix(infix, postfix);
    showQueue(postfix);
    
    while (!isQueueEmpty(postfix)) {
        getQueue (postfix, n);
        if ( n->operate == Operand || n->operate == Variable ) {
            //printf("dingdingdind\n");
            //printf("Push in\n");
            push ( createTreeNode(n) ); //如果是操作数或变量，建立一个树节点并入栈
        } else {
            if ( doubleOperandOperate( n) ) { // 如果是二元运算符，从栈里弹两个东西出来并将其连接后入栈
                tempTree1 = pop(); 
                tempTree2 = pop();
                tempTree3 = createTreeNode (n);
                conjection( tempTree2, tempTree3, tempTree1);
                push(tempTree3);
            } else { // 如果是一元操作符，弹一个出来链接入栈
                tempTree2 = pop();
                tempTree3 = createTreeNode (n);
                conjection( tempTree2, tempTree3, NULL );
                push(tempTree3);
            }
        }
    }
    // 当最终操作完成后，树的首地址留在栈的第一个位置
    tree t = pop();
    freeNode(n);
    return t;
}

double calculate(const tree expression, double x)
{
    switch (expression -> n -> operate) {
        case Plus:
            return calculate(expression -> left ,x) + calculate(expression -> right, x);
        case Minus:
            return calculate(expression -> left,x) - calculate(expression -> right,x);
        case Multiple:
            return calculate(expression -> left,x) * calculate(expression -> right,x);
        case Divide:
            return calculate(expression -> left,x) / calculate(expression -> right,x);
        case Times:
            return pow( calculate(expression -> left,x) , calculate(expression -> right,x));
        case Sqrt:
            return sqrt( calculate(expression -> left,x));
        case Sin:
            return sin( calculate(expression -> left,x));
        case Cos:
            return cos( calculate(expression -> left,x));
        case Tan:
            return tan( calculate(expression -> left,x));
        case Log:
            return log( calculate(expression -> left, x));
        case Operand:
            return expression -> n -> operand;
        case Variable:
            return x;
        default :
            fprintf(stderr ,"Bad expression.\n");
    }
}

