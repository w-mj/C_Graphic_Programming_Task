#include "weapon.h"
#include "xml.h"
#include "common.h"
#include "animation.h"

weaponList createWeaponList(xmlTree xml, SDL_Renderer *renderer)
{

    weaponList start = (weaponList)malloc(sizeof(struct WEAPON_LIST));
    weaponList ret = start;
    SDL_Texture *img = loadImg0(getValue(xml, "Weapons/SpriteSheet/Path"), renderer);
    xml = getSubTree(xml, "Weapons");
    xmlTree subTree;
    char *temp;
    for (int i = 1; i < xml -> subLableNumber; i++) {
        subTree = xml -> subLable[i];
        temp = getValue(subTree, "name");
        strcpy(ret -> name, temp);
        temp = getValue(subTree, "attackType");
        ret -> attackType = atoi(temp);
        temp = getValue(subTree, "power");
        ret -> power = atoi(temp);
        temp = getValue(subTree, "speed");
        ret -> speed = atoi(temp);
        ret -> weapon = (pictureNode)malloc(sizeof(struct PIC));
        setPic(ret -> weapon, subTree -> subLable[0]); 
        ret -> weapon -> imgSource = img;
        if (ret -> attackType == 1) {
            ret -> bullet = (pictureNode)malloc(sizeof(struct PIC));
            setPic(ret -> bullet, subTree -> subLable[1]);
            ret -> bullet -> imgSource = img;
        }
        if (i != xml -> subLableNumber - 1)
            ret -> next = (weaponList)malloc(sizeof(struct WEAPON_LIST));
        else 
            ret -> next = NULL;
    }
    return start;
}

void destroyWeaponList(weaponList w)
{
    weaponList del;
    while (w != NULL) {
        del = w;
        if (del -> weapon != NULL) 
            free(del -> weapon);
        if (del -> bullet != NULL)
            free(del -> bullet);
        w = del -> next;
        free(del);
    }
}
