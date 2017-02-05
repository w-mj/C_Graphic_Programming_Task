#include <stdlib.h>
#include "animation.h"
#include "xml.h"
#include "sdl.h"
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
    for (int i = 0; i < 15; i++) {
        ret -> frames[i] = NULL;
    }
    memset(ret, 0, sizeof(struct ANIMATION_LIST));
    pictureNode pic = (pictureNode)malloc(sizeof(struct PIC));
    xmlTree animation = NULL, frame = NULL;
    frameList start, end;

    char * filename = getValue(xml, "AnimatedActor/SpriteSheet/Path");
    char * temp;
    ret -> img = loadImg(filename, renderer);

    xml  = getSubTree(xml, "AnimatedActor/Animations");
    for (int i = 0; i < xml -> subLableNumber; i++) {
        animation = xml -> subLable[i];
        temp = getValue(animation , "Name");
        strcpy(ret -> name[i], temp);  // 复制动画名
        start = ret -> frames[i];
        end = ret -> frames[i];
        for (int j = 0; j < animation -> subLableNumber; j++) {
            frame = animation -> subLable[j];
            temp = getValue(frame, "XPosition");
            pic -> displayLocation.x = atoi(temp);
            temp = getValue(frame, "YPosition");
            pic -> displayLocation.y = atoi(temp);
            temp = getValue(frame, "XCrop");
            pic -> imageLocation.x = atoi(temp);
            temp = getValue(frame, "YCrop");
            pic -> imageLocation.y = atoi(temp);
            temp = getValue(frame, "Width");
            pic -> imageLocation.w = pic -> displayLocation.w = atoi(temp);
            temp = getValue(frame, "Height");
            pic -> imageLocation.h = pic -> displayLocation.h = atoi(temp);
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
    }
    return ret;
}

void addAnimation(frameList frame, xmlTree xml, const char *name)
{

}
