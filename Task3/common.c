#include "common.h"
#include "animation.h"
#include "xml.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void setPic(pictureNode pic, xmlTree frame)
{
    char *temp;
    temp = getValue(frame, "XPosition");
    pic -> displayLocation.x = atoi(temp);
    free(temp);
    temp = getValue(frame, "YPosition");
    pic -> displayLocation.y = atoi(temp);
    free(temp);
    temp = getValue(frame, "XCrop");
    pic -> imageLocation.x = atoi(temp);
    free(temp);
    temp = getValue(frame, "YCrop");
    pic -> imageLocation.y = atoi(temp);
    free(temp);
    temp = getValue(frame, "Width");
    pic -> imageLocation.w = pic -> displayLocation.w = atoi(temp);
    free(temp);
    temp = getValue(frame, "Height");
    pic -> imageLocation.h = pic -> displayLocation.h = atoi(temp);
    free(temp);
}

SDL_Texture *loadImg0(const char * filename, SDL_Renderer *renderer)
{
    SDL_Surface *file;
    SDL_Texture *image;
    file = SDL_LoadBMP(filename);
    if (file == NULL) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(1);
    }
    SDL_SetColorKey(file, SDL_TRUE, SDL_MapRGB(file -> format, 0xff, 0xff, 0xff));
    image = SDL_CreateTextureFromSurface(renderer, file);
    SDL_FreeSurface(file);
    return image;
}
