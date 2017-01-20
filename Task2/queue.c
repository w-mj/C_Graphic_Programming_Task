#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
void emptyQueue(queue q)
{
    q->s = 0;
    q->e = 0;
}
bool getQueue(queue q, node tar)
{
    if (q ->s == q->e) {
        fprintf(stderr, "Queue is empty.\n");
        return false;
    }
    tar -> operate = q->q[q->s]->operate;
    tar -> operand = q->q[q->s]->operand;
    q->s++;
    return true;
}
bool addQueue(queue x, node n)
{
    if (x -> e == x -> max) {
        fprintf(stderr, "Queue is full");
        return false;
    }
    //printf("%d\n", x->e);
    x->q[x->e]->operate = n->operate;    
    x->q[x->e]->operand = n->operand;    
    x->e++;
    return true;
}
bool isQueueEmpty(queue q) {
    return q->s == q->e;
}
queue initQueue(void)
{
    queue a;
    a = (queue)malloc(sizeof(struct QUE));
    a -> max = 200;
    a -> s = 0;
    a -> e = 0;
    for (int i = 0; i < 200; i++) {
        a -> q[i] = initNode();
    }
    return a;
}
void freeQueue(queue a)
{
    for (int i = 0; i < 200; i++) {
        freeNode(a->q[i]);
    }
    free(a);
}
