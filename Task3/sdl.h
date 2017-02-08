#ifndef _SDL_H_
#define _SDL_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    *window = SDL_CreateWindow("Shoot!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(-1);
    }
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(-1);
    }
}

void destroySDL(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture *loadImg(const char * filename, SDL_Renderer *renderer)
{
    SDL_Surface *file;
    SDL_Texture *image;
    file = SDL_LoadBMP(filename);
    if (file == NULL) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(1);
    }
    image = SDL_CreateTextureFromSurface(renderer, file);
    SDL_FreeSurface(file);
    return image;
}
#endif
