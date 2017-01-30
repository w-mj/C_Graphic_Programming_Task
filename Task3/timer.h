#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdbool.h>
#include <SDL2/SDL.h>

struct MyTimer {
    Uint32 startTick;
    Uint32 pausedTime;
    Uint32 pauseTick;
    bool paused;
    bool running;
};

typedef struct MyTimer* timer;

timer initTimer(void);
int startTimer(timer t);
int pausedTimer(timer t);
int remuseTimer(timer t);
int haltTimer(timer t);
Uint32 getTimer(timer t);
int freeTimer(timer t);

#endif
