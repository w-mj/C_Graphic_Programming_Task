#ifndef _COMMON_H_
#define _COMMON_H_

#include "common.h"
#include "animation.h"
#include "xml.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void setPic(pictureNode pic, xmlTree frame);
SDL_Texture *loadImg0(const char * filename, SDL_Renderer *renderer);

#endif


