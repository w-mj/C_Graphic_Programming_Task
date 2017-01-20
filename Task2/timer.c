#include "timer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

timer initTimer(void)
{
    timer t = (timer)malloc(sizeof(struct MyTimer));
    t -> startTick = 0; 
    t -> pausedTime = 0;
    t -> pauseTick = 0;
    t -> paused = false;
    t -> running = false;
    return t;
}

int startTimer(timer t)
{
    if (t -> running)
        return -1;
    t -> startTick = SDL_GetTicks();
    t -> running = true;
    return 0;
}

int pausedTimer(timer t)
{
    if ( t -> paused == true) 
        return -1;
    if ( t -> running == false)
        return -2;
    t -> paused = true;
    t -> pauseTick = SDL_GetTicks();
    return 0;
}

int remuseTimer(timer t)
{
    if ( t -> paused == false)
        return -1;
    t -> paused = false;
    t -> pausedTime += SDL_GetTicks() - ( t -> pauseTick );
    t -> pauseTick = 0;
    return 0;
}

int haltTimer(timer t)
{
    t -> startTick = 0; 
    t -> pausedTime = 0;
    t -> pauseTick = 0;
    t -> paused = false;
    t -> running = false;
    return 0;
}

Uint32 getTimer(timer t)
{
    if (t -> running == false )
        return 0;
    if ( t -> paused == true) 
        remuseTimer(t);
    return SDL_GetTicks() - ( t -> startTick ) - (t -> pausedTime); 
}

int freeTimer(timer t)
{
    free(t);
    return 0;
}
