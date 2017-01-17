#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

const int SCREENWIDTH = 1000;
const int SCREENHEIGHT = 600;
const int CLOCKAREA = 600;
const int BPP = 32;
const int SECHANDLENGTH = 300;
const int MINHANDLENGTH = 200;
const int HOURHANDLENGTH = 150;
const int PI = 3.1415926;

SDL_Window *window = NULL;
SDL_Surface *screen =NULL;
SDL_Renderer * renderer = NULL;
SDL_Color textColor = {0, 0, 0};

int init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return -1;
    window = SDL_CreateWindow("Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
        return -2;
    screen = SDL_GetWindowSurface(window);
    if (screen == NULL)
        return -3;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        return -4;
    TTF_Init();
    SDL_RenderClear(renderer);
    return 0;
}

void destroy()
{
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

SDL_Texture *loadImage(const char * filename, bool colorkey)
{
    SDL_Surface *file;
    SDL_Texture *image;
    file = SDL_LoadBMP(filename);
    if (file == NULL)
        printf("WrongFile");
    if (colorkey)
        SDL_SetColorKey(file, SDL_TRUE, SDL_MapRGB(file->format, 0xff, 0xff, 0xff));
    image = SDL_CreateTextureFromSurface(renderer, file);

    SDL_FreeSurface(file);
    return image;
}

void drawSecondHand(int sec)
{
    double theta = 6 * sec;
    theta = theta / 180.0 * PI; //角度化弧度
    int x = (CLOCKAREA / 2) + SECHANDLENGTH * sin( theta);
    int y = (CLOCKAREA / 2) - SECHANDLENGTH * cos( theta);
    SDL_RenderDrawLine( renderer, CLOCKAREA / 2, CLOCKAREA / 2, x, y );
}

void drawMinuteHand(int minute)
{
    double theta = 6 * minute;
    theta = theta / 180.0 * PI; //角度化弧度
    int x = (CLOCKAREA / 2) + MINHANDLENGTH * sin( theta);
    int y = (CLOCKAREA / 2) - MINHANDLENGTH * cos( theta);
    SDL_RenderDrawLine( renderer, CLOCKAREA / 2, CLOCKAREA / 2, x, y );
}
void drawHourHand(int hour)
{
    double theta = 30 * hour;
    theta = theta / 180.0 * PI; //角度化弧度
    int x = (CLOCKAREA / 2) + HOURHANDLENGTH * sin( theta);
    int y = (CLOCKAREA / 2) - HOURHANDLENGTH * cos( theta);
    //printf("%d\n", hour);
    SDL_RenderDrawLine( renderer, CLOCKAREA / 2, CLOCKAREA / 2, x, y );
}
int main(int argc, char *argv[])
{
    SDL_Event event;
    SDL_Texture *bg = NULL; //背景图片
    SDL_Texture *clock = NULL; //表盘
    SDL_Surface *text = NULL; //文本
    SDL_Texture *textTexture = NULL;
    TTF_Font *font = NULL;  //字体
    bool quit = false;
    int flag;
    struct tm *nowTime;
    time_t tempTimer;
    SDL_Rect clockArea = {0 , 0, CLOCKAREA, CLOCKAREA}; //钟表区域
    SDL_Rect textArea =  {CLOCKAREA, 0, SCREENWIDTH - CLOCKAREA, SCREENWIDTH};
    if ((flag = init(screen)) != 0)
        return flag;

    bg = loadImage("bg.bmp", false);
    clock = loadImage("clock2.bmp", true);
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
    font = TTF_OpenFont("lazy.ttf", 28);
    //if (font == NULL )
        //return 123;

    while( !quit ) {
        while (SDL_PollEvent( &event )) {
            if (event.type == SDL_QUIT)
                quit = true;
        }

        SDL_RenderCopy(renderer, bg, NULL, NULL); //显示背景
        SDL_RenderCopy(renderer, clock, &clockArea, &clockArea); //显示表盘

        time(&tempTimer);
        nowTime = gmtime(&tempTimer);
        drawSecondHand(nowTime -> tm_sec);
        drawMinuteHand(nowTime -> tm_min);
        drawHourHand((nowTime -> tm_hour + 8 ) %12);//获得当前时间然后画指针

        //text = TTF_RenderText_Solid(font, "设置闹钟", textColor);
        //textTexture = SDL_CreateTextureFromSurface(renderer, text);
        //SDL_RenderCopy(renderer, textTexture, &textArea, NULL);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(text);
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(clock);
    destroy();
    return 0;
}
