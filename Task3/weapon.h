#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "animation.h"
#include "xml.h"

struct WEAPON_LIST{
    int power;
    int speed;
    int attackType;
    pictureNode weapon;
    pictureNode bullet;
    char name[128];
    struct WEAPON_LIST *next;
};

typedef struct WEAPON_LIST * weaponList;

weaponList createWeaponList(xmlTree xml, SDL_Renderer *renderer);
void destroyWeaponList(weaponList w);

#endif 
