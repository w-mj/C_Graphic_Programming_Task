#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "operator.h"
#include <stdbool.h>
#include "node.h"

struct QUE {
    node q[200];
    int max;
    int s;
    int e;
};
typedef struct QUE* queue;

bool initQueue(queue a);
bool addQueue(queue q, node n);
bool getQueue(queue q, node tar);
bool isQueueEmpty(queue q);
void emptyQueue(queue q);
void freeQueue(queue a);

#endif
