#include "operator.h"
#include <stdbool.h>

bool inArray(const enum operators ops[MAX_SAME_PRIORITY_OPERATOR],const enum operators op)
{
    for ( int i = 0; i < MAX_SAME_PRIORITY_OPERATOR; i++) 
        if (op == ops[i])
            return true;
    return false;
}

int comparePriority(const enum operators op1,const  enum operators op2)
{
    bool  n1, n2;
    int a = 0;
    while ( 1 ) {
        n1 = inArray(priority[a] , op1);
        n2 = inArray(priority[a] , op2);
        if (!( n1 || n2)) 
            a++;
        else if ( n1 && n2 )
            return 0;
        else if (n1 && !n2)
            return 1;
        else if (!n1 && n2)
            return -1;
    }
    return 987;
}
