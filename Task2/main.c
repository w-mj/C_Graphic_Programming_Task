#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "calculator.h"
#include "tree.h"
#include "timer.h"
const int FPS = 30;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

void initSDL(void)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("函数绘制器", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "%s",SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
}
void destroy(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}
int main(int argc, char *argv[]) 
{
    SDL_Event event;

    if (argc == 1) {
        printf("Fatal, no input.\n");
        exit(1);
    }

    tree t;
    timer fps = initTimer();
    initSDL();
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_Rect blank = {0, 0, 800, 600};
    // 显示窗口（x1，y1）与实际坐标（x0，y0）之间的相对位置
    // x1 = x0 + X 
    // y1 = -y0 + Y
    double k = 1; // 放大率
    int X = 400, Y = 300;  
    int x1, y1;
    double ty;
    int lastx, lasty;
    int usedTime;
    bool first = true, action = true;
    t = transTree(argv[1]);
    bool quit = false;
    int verticalSpeed = 0, horizontalSpeed = 0;
    double zoomSpeed = 1.0;
    while (!quit) {
        startTimer(fps);
        if (action) { 
            //fprintf(stderr, "%f\n", k);
            action = false;
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // 清屏
            SDL_RenderFillRect(renderer, &blank);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
            //画坐标轴
            if (Y >= 0 && Y <= 600) 
                SDL_RenderDrawLine(renderer, 0, Y, 800, Y);
            if (Y >= 0 && X <= 800)
                SDL_RenderDrawLine(renderer, X, 0, X, 600);
            first = true;
            for (x1 = 0; x1 <= 800; x1++) {
                ty = calculate(t, (x1  - X) / k); // 计算出y的值
                y1 = Y - (int)(ty * k + 0.5);
                //y1 = (Y - (x1 - X)); 
                if (first) first = false;
                else {
                    //if (lastx >= 0 && lastx <= 800 && lasty >= 0 && lasty <= 600 && y1 >= 0 && y1 <= 600)
                    if (y1 > 600 ) y1 = 600;
                    if (y1 < 0 ) y1 = 0;
                    SDL_RenderDrawLine(renderer, lastx, lasty, x1, y1);
                }
                lastx = x1;
                lasty = y1;
            }
        }

        while (SDL_PollEvent( &event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    verticalSpeed = 3;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    verticalSpeed = -3;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    horizontalSpeed = -3;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    horizontalSpeed = 3;
                } else if (event.key.keysym.sym == SDLK_c) {
                    X = 400;
                    Y = 300;
                    k = 1;
                    action = true;
                } else if(event.key.keysym.sym == SDLK_z) {
                    zoomSpeed = 2;
                } else if(event.key.keysym.sym == SDLK_x) {
                    zoomSpeed = 0.5;
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    verticalSpeed = 0;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    verticalSpeed = 0;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    horizontalSpeed = 0;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    horizontalSpeed = 0;
                } else if(event.key.keysym.sym == SDLK_z) {
                    zoomSpeed = 1;
                } else if(event.key.keysym.sym == SDLK_x) {
                    zoomSpeed = 1;
                }
            }
        }

        if (horizontalSpeed + verticalSpeed + zoomSpeed!= 0) {
            Y += verticalSpeed;
            X += horizontalSpeed;
            k *= zoomSpeed;
            action = true;
        }

        usedTime = getTimer(fps); // fps
        if (usedTime < 1000/FPS) {
            SDL_Delay (1000/FPS - usedTime);
            haltTimer(fps);
        }
        SDL_RenderPresent(renderer);
    }
    freeTree(t);
    destroy();
    return 0;
}
