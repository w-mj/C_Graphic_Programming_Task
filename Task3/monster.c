#include "monster.h"
#include "xml.h"
#include "animation.h"
#include "common.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>


monsterList createMonsterList(xmlTree rootxml, SDL_Renderer *renderer)
{
    char *temp;
    if (rootxml -> subLableNumber == 0)
        return NULL;
    monsterList start = (monsterList)malloc(sizeof(struct MONSTER_LIST));
    start -> next = NULL;
    monsterList leap = start;
    for (int i = 0; i < rootxml -> subLableNumber; i++) {
        temp = getValue(rootxml -> subLable[0] -> subLable[i], "Path");
        //fprintf(stderr, "&&&&%s\n", temp);
        xmlTree xml = loadXML(temp);
        free(temp);

        temp = getValue(xml -> subLable[0], "name");
        strcpy(leap -> name, temp);
        free(temp);
        temp = getValue(xml -> subLable[0], "speed");
        leap -> speed = atoi(temp);
        free(temp);
        temp = getValue(xml -> subLable[0], "moveMethod");
        leap -> moveMethod = atoi(temp);
        free(temp);
        temp = getValue(xml -> subLable[0], "attackMethod");
        leap -> attackMethod = atoi(temp);
        free(temp);
        temp = getValue(xml -> subLable[0], "attackFrequency");
        leap -> attackFrequency = atoi(temp);
        free(temp);
        temp = getValue(xml -> subLable[0], "health");
        leap -> health = atoi(temp);
        free(temp);
        
        leap -> animation = createAnimationList(xml, renderer);
        if (i != rootxml -> subLableNumber - 1) {
            leap -> next = (monsterList)malloc(sizeof(struct MONSTER_LIST));
            leap = leap -> next;
            leap -> next = NULL;
        }
    }
    return leap;
}

void destroyMonsterList(monsterList ml)
{
    while(ml != NULL) {
        destroyAnimationList(ml -> animation);
        monsterList  del = ml;
        ml = del -> next;
        free(del);
    }
}
