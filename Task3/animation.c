#include <stdlib.h>
#include "animation.h"
#include "xml.h"
#include "common.h"
#include <stdbool.h>
void copyPic(pictureNode target, const pictureNode source)
{
    target -> imgSource = source -> imgSource;
    target -> imageLocation.x = source -> imageLocation.x;
    target -> imageLocation.y = source -> imageLocation.y;
    target -> imageLocation.h = source -> imageLocation.h;
    target -> imageLocation.w = source -> imageLocation.w;
    target -> displayLocation.x = source -> displayLocation.x;
    target -> displayLocation.y = source -> displayLocation.y;
    target -> displayLocation.h = source -> displayLocation.h;
    target -> displayLocation.w = source -> displayLocation.w;
}

frameList initFrameList(void)
{
    frameList start = (frameList)malloc(sizeof(struct FRAME_LIST));
    memset(start, 0, sizeof(struct FRAME_LIST));
    start -> picture = (pictureNode)malloc(sizeof(struct PIC));
    start -> nextFrame = NULL;
    start -> lastFrame = NULL;
    return start;
}


void freeFrame(frameList frame)
{
    frameList goNext = frame;
    frameList goLast = frame -> lastFrame;
    frameList del;
    if (frame -> lastFrame != NULL)
        (frame -> lastFrame) -> nextFrame = NULL;
    while (goNext != NULL) {
        del = goNext;
        goNext = del -> nextFrame;
        free(del -> picture);
        free(del);
    }
    while(goLast != NULL) {
        del = goLast;
        goLast = del -> lastFrame;
        free(del -> picture);
        free(del);
    }
}
animationList createAnimationList(xmlTree xml, SDL_Renderer *renderer)
{
    animationList ret = (animationList)malloc(sizeof(struct ANIMATION_LIST));
    if (ret == NULL)
        exit(10);
    for (int i = 0; i < 15; i++) {
        ret -> frames[i] = NULL;
    }
    memset(ret, 0, sizeof(struct ANIMATION_LIST));
    pictureNode pic = (pictureNode)malloc(sizeof(struct PIC));
    xmlTree animation = NULL, frame = NULL;
    frameList start, end;

    char * filename = getValue(xml, "AnimatedActor/SpriteSheet/Path");
    char * temp , *defaultName;
    ret -> img = loadImg0(filename, renderer);
    if (ret -> img == NULL) {
        fprintf(stderr, "%s\n" ,SDL_GetError());
        exit(23);
    }
    fprintf(stderr, "%p\n", ret -> img);

    xml  = getSubTree(xml, "AnimatedActor/Animations");
    defaultName = getValue(xml, "DefaultAnimation");
    for (int i = 0; i < xml -> subLableNumber; i++) {
        animation = xml -> subLable[i];
        temp = getValue(animation , "Name");
        if (strcmp(temp, defaultName) == 0)
            ret -> defaultAnimation = i;

        strcpy(ret -> name[i], temp);  // 复制动画名
        start = ret -> frames[i];
        end = ret -> frames[i];
        for (int j = 0; j < animation -> subLableNumber; j++) {
            frame = animation -> subLable[j];
            setPic(pic, frame);
            pic -> imgSource = ret -> img;

            frameList fl = initFrameList();
            copyPic(fl -> picture, pic);
            temp = getValue(frame, "Delay");
            fl -> delay = atoi(temp);
            if (start == NULL ) {
                start = end = fl;
            }
            else {
                fl -> lastFrame = end;
                //fl -> nextFrame = start;
                end -> nextFrame = fl;
                //start -> lastFrame = fl;
                end = fl;
            }
        }
        temp = getValue(animation, "Loop");
        if (strcmp(temp, "True") == 0) {
            start -> lastFrame = end;
            end -> nextFrame = start;
        } else {
            start -> lastFrame = NULL;
            end -> nextFrame = NULL;
        }
        ret -> frames[i] = start;
        ret -> aniNumber += 1;
    }
    fprintf(stderr, "ret0 = %p\n", ret -> frames[0]);
    fprintf(stderr, "ret1 = %p\n", ret -> frames[1]);
    return ret;
}

frameList addAnimation(animationList aniList, const char *name)
{
    if(strcmp(name , "default") == 0)
        return aniList -> frames[aniList -> defaultAnimation] ;
    else 
        for (int i = 0; i < aniList -> aniNumber; i++) {
            if (strcmp(name, aniList -> name[i]) == 0) {
                return aniList -> frames[i];
            }
        }
    return NULL;

}
