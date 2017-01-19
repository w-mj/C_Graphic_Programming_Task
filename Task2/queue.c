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
bool addQueue(queue q, node n)
{
    if (q -> e == q -> max) {
        fprintf(stderr, "Queue is full");
        return false;
    }
    q->q[q->e]->operate = n->operate;    
    q->q[q->e]->operand = n->operand;    
    q->e++;
    return true;
}
bool isQueueEmpty(queue q) {
    return q->s == q->e;
}
bool initQueue(queue a)
{
    a -> max = 200;
    a ->s = 0;
    a -> e = 0;
    for (int i = 0; i < 200; i++) {
        a -> q[i] = malloc(sizeof(struct element));
    }
}
void freeQueue(queue a)
{
    for (int i = 0; i < 200; i++) {
        free(a->q[i]);
    }
    
}
