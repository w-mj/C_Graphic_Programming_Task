#include <SDL/SDL.h>

SDL_Surface * loadImage(char * filename)
{
    SDL_Surface * Image = NULL;
    SDL_Surface * optimizedImage = NULL;
    Image = SDL_LoadBMP(filename);
    if (Image != NULL) {
        optimizedImage = SDL_DisplayFormat(Image);
        SDL_FreeSurface(Image);
    }
    return optimizedImage;
}

void apply(SDL_Surface * source, SDL_Surface * destination, int x, int y)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, destination, &offset);
}
int main(int argc, char * argv[])
{
    SDL_Surface * screen = NULL;
    SDL_Surface * message = NULL;
    SDL_Surface * background = NULL;
    SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(1600, 1200, 32, SDL_SWSURFACE);
    if (screen != NULL) {
        SDL_WM_SetCaption("HelloWorld", NULL);
        message = loadImage("hello.bmp");
        background = SDL_LoadBMP("bg.bmp");
        apply(background, screen, 0, 0);
        apply(background, screen, 800, 0);
        apply(background, screen, 0, 600);
        apply(background, screen, 800, 600);
        //SDL_BlitSurface(background, NULL, screen, NULL);
    }
    SDL_Flip(screen);
    SDL_Delay(10000);

    SDL_FreeSurface( message );
    SDL_FreeSurface( background );
    SDL_Quit();

    return 0;
}
