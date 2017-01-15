#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdbool.h>

SDL_Surface * loadImage(char *filename)
{
    SDL_Surface * image = NULL, * optimizedImage = NULL;
    image = IMG_Load(filename);
    if (image != NULL) {
        optimizedImage = SDL_DisplayFormat(image);
        SDL_FreeSurface(image);
    }
    return optimizedImage;
}

SDL_Surface * init(int width, int hight)
{
    SDL_Surface * screen = NULL;
    if( SDL_Init(SDL_INIT_EVERYTHING) != -1)
        screen = SDL_SetVideoMode(width, hight, 32, SDL_SWSURFACE);
    return screen;
}

void applyImage(SDL_Surface * image, SDL_Surface * screen, int x, int y)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(image, NULL, screen, &offset);
}

int main(int argc, char * args[])
{
    SDL_Surface * screen = init(800, 600);
    SDL_Surface * image = loadImage("bg.bmp");
    SDL_Event event;
    bool quit = false;
    applyImage(image, screen, 0, 0);
    SDL_Flip(screen);
    while (quit == false) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
    }
    SDL_FreeSurface(screen);
    SDL_FreeSurface(image);
    return 0;
}
