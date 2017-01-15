#include <SDL/SDL.h>

int main(void)
{
    SDL_Surface* hello = NULL;
    SDL_Surface* screen =NULL;

    SDL_Init( SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(640 , 480, 32, SDL_SWSURFACE);

    hello = SDL_LoadBMP("bg.bmp");

    SDL_BlitSurface( hello, NULL, screen, NULL);

    SDL_Flip(screen);

    SDL_Delay(10000);

    SDL_FreeSurface(hello);
    SDL_Quit();
    return 0;
}
