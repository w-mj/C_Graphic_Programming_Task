#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "animation.h"
#include "sdl.h"
#include "source.h"
#include "xml.h"


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

struct {
    int x, y;
    int speed;
    int weapon;
    int helth;
} playerStatus;

struct {
    int x, y;
    int targetX, targetY;
    int speed;
    int moveMethod;
    int attackMethod;
    int attackFrequency; // 每几毫秒攻击一次
    int health;
} monsters[15];

struct {
    int x, y;
    int targetX, targetY;
    int speed;
    int power;
    int range;
} bullet[20];
//得到当前图层的位置
void getLocation(int *x, int *y, int i)
{
    if (i == 1) {
        *x = playerStatus.x;
        *y = playerStatus.y;
    }
    else if (i >= 5 && i < 20) {
        *x = monsters[i - 5].x;
        *y = monsters[i - 5].y;
    }
    else {
        *x = 0;
        *y = 0;
    }
        
}



void drawbg(int x, SDL_Texture *bg)
{
    SDL_Rect loca = {x, 0, 800, 600};
    SDL_RenderCopy(renderer, bg, NULL, &loca);
}

void initGame(void)
{
    playerStatus.x = 0;
    playerStatus.y = 0;
    playerStatus.weapon = 1;
    playerStatus.helth = 10;
    playerStatus.speed= 10;
}

int main(int argc, char *argv[])
{
    initSDL(window, renderer);
    initGame();
    SDL_Event event;
    SDL_Texture *bg = loadImg("bg.bmp", renderer);
    xmlTree player = loadXML("character.xml");
    frameList frames[50]; //1图层为玩家，[5,20)图层为敌人，49图层为准星
    for (int i = 0; i < 50; i++)
        frames[i] = NULL;
    int moveSpeed;
    
    bool quit = false;
    while (!quit) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_d: 
                        moveSpeed = playerStatus.speed;
                        addAnimation(frames[1], player, "moveRight");
                        break;
                    case SDLK_a:
                        moveSpeed = -playerStatus.speed;
                        break;
                }
            }
            else 
                continue;
        }
        playerStatus.x += moveSpeed;

        drawbg(0, bg);
        for (int i = 0; i < 50; i++) {
            if (frames[i] != NULL) {
                if (frames[i] -> picture != NULL) {
                    // 显示当前图片
                    getLocation(&frames[i]->picture->displayLocation.x, &frames[i]->picture->displayLocation.y, i); //显示之前先获得位置
                    SDL_RenderCopy(renderer, frames[i]->picture->imgSource, &frames[i] -> picture -> imageLocation, &frames[i] -> picture -> displayLocation);
                    // 显示完一张图片后，计数器加一，如果count等于delay，说明这个图片显示完了，切换到下一帧
                    frames[i] -> count++;
                    if (frames[i] -> count == frames[i] -> delay) {
                        frames[i] -> count = 0;
                        // 如果还有下一帧的话，显示下一帧，否则删除掉这个动画
                        if (frames[i] -> nextFrame != NULL) {
                            frames[i] = frames[i] -> nextFrame;
                        } else {
                            freeFrame(frames[i]);
                            frames[i] = NULL;
                        }
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
        
    }
    SDL_DestroyTexture(bg);
    destroySDL(window, renderer);
    return 0;
}
