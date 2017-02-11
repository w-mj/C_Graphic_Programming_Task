#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "xml.h"
#include "animation.h"
#include <SDL2/SDL.h>

struct MONSTER_LIST {
    char name[128];
    int speed;
    int moveMethod;
    int attackMethod;
    int attackFrequency;
    int health;
    animationList animation;
    struct MONSTER_LIST *next;
};

typedef struct MONSTER_LIST *monsterList;

monsterList createMonsterList(xmlTree rootxml, SDL_Renderer *renderer);
void destroyMonsterList(monsterList ml);

#endif
