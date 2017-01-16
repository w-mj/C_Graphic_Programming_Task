#ifndef _TIMER_H_
#define _TIMER_H_

#include <SDL/SDL.h>
#include <stdbool.h>


struct MyTimer {
    Uint32 startTick;
    Uint32 pausedTime;
    Uint32 pauseTick;
    bool paused;
    bool running;
};

typedef struct MyTimer* timer;

int initTimer(timer t);
int startTimer(timer t);
int pausedTimer(timer t);
int remuseTimer(timer t);
int haltTimer(timer t);
Uint32 getTimer(timer t);

#endif
