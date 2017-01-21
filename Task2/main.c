#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "calculator.h"
#include "tree.h"
#include "timer.h"
const int FPS = 30;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
TTF_Font *font = NULL;

void initSDL(void)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
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
    font = TTF_OpenFont("fzssjt.ttf",25);
    if (font == NULL) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(1);
    }
}
void destroy(void)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    TTF_Quit();
    SDL_Quit();
}
void showText(const char *text, SDL_Rect *location)
{
    SDL_Color textColor = {0, 0, 0, 0xff};
    SDL_Texture *textTexture = NULL;
    SDL_Surface *textSurface = NULL; //文本
    textSurface = TTF_RenderText_Solid(font, text, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);//渲染字体
    SDL_RenderSetViewport( renderer, location );//调整viewport
    SDL_RenderCopy(renderer, textTexture, NULL, NULL);
    SDL_RenderSetViewport( renderer, NULL );
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}
int abs(int x)
{
    return x>0?x:-x;
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
    SDL_Rect textLoca = {0, 0, 16, 20}; // 12号字，每个字16像素
    SDL_Rect debug = {0, 0, 32, 20};
    char text[100];
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
            int offsetx = X - 400;
            int offsety = Y - 300;
            sprintf(text, "%3d", X);
            showText(text, &debug);
            //画坐标轴
            if (Y >= 0 && Y <= 600){
                SDL_RenderDrawLine(renderer, 0, Y, 800, Y);
                textLoca.y = Y + 2;
                for (int i = 0;  i <= 800; i+= 80) {
                    sprintf(text, "%10g", (i - 400 -  offsetx  + offsetx % 80) / k); 
                    if (strlen(text) >= 8) { text[8] = 0; }
                    textLoca.x =  i+ (offsetx % 80);
                    textLoca.w = 8 * strlen(text);
                    showText(text, &textLoca);
                }
            }
            if (Y >= 0 && X <= 800){
                SDL_RenderDrawLine(renderer, X, 0, X, 600);
                textLoca.x = X + 2;
                for (int i = 0;  i <= 600; i+= 60) {
                    if ( i -300 - offsety + offsety % 60 == 0) continue;
                    sprintf(text, "%10g", -(i - 300 -  offsety  + offsety % 60) / k); 
                    if (strlen(text) >= 10) { text[10] = 0; }
                    textLoca.y =  i+ (offsety % 60);
                    textLoca.w = 8 * strlen(text);
                    showText(text, &textLoca);
                }
            }
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
                    zoomSpeed = 1.3;
                } else if(event.key.keysym.sym == SDLK_x) {
                    zoomSpeed = 0.7;
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
            else 
                continue;
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
