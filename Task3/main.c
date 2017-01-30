#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "animation.h"
#include "sdl.h"
#include "source.h"

struct {
    int sence;
    int charactor;
    int health;
    int speed;
    int damge;
    int keys;
    int coins;
    int booms;
} status;

void intiGame(void)
{
    status.sence = BASEMENT;
}
int main(int argc, char *argv[])
{
    init();
    SDL_Event event;
    frameList frames = initFrameList(30); // 设置帧列表，30fps
    bool quit = false;
    while (!quit) {

    }
    freeFrame(frames);
    destroy();
    return 0;
}
