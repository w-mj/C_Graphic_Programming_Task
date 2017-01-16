#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

const int SCREENWIDTH = 1000;
const int SCREENHEIGHT = 600;
const int BPP = 32;

SDL_Window *window = NULL;
SDL_Surface *screen =NULL;
SDL_Renderer * renderer = NULL;

int init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return -1;
    window = SDL_CreateWindow("CLock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
        return -2;
    screen = SDL_GetWindowSurface(window);
    if (screen == NULL)
        return -3;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        return -4;
    SDL_RenderClear(renderer);
    return 0;
}

void destroy()
{
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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
int main(int argc, char *argv[])
{
    SDL_Event event;
    SDL_Texture *bg = NULL; //背景图片
    SDL_Texture *clock = NULL; //表盘
    bool quit = false;
    SDL_Rect clockArea = {0 , 0, 600, 600};
    init(screen);


    bg = loadImage("bg.bmp", false);
    clock = loadImage("clock2.bmp", true);

    while( !quit ) {
        while (SDL_PollEvent( &event )) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
        SDL_RenderCopy(renderer, bg, NULL, NULL); //显示背景
        SDL_RenderCopy(renderer, clock, &clockArea, &clockArea); //显示表盘

        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderDrawLine( renderer, 0, SCREENHEIGHT / 2, SCREENWIDTH, SCREENHEIGHT / 2 );
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(bg);
    destroy();
    return 0;
}
