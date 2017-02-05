#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <SDL2/SDL_image.h>
#include "xml.h"

struct PIC {
    SDL_Texture * imgSource;
    SDL_Rect imageLocation;
    SDL_Rect displayLocation;
};
typedef struct PIC * pictureNode;

struct FRAME_LIST {
    pictureNode picture;
    int delay;
    int count;
    struct FRAME_LIST *nextFrame;
    struct FRAME_LIST *lastFrame;
};
typedef struct FRAME_LIST *frameList;

// 读取xml文件，把某个东西的所有动画储存起来
struct ANIMATION_LIST {
    SDL_Texture * img;
    char name[15][128];
    int defaultAnimation;
    frameList frames[15];
};
typedef struct ANIMATION_LIST *animationList;

// 每一个frameList作为某一个动画的一组帧，
// 每一个动画具有一个独立的图层，依次显示每个图层中的一帧
// 下一次该图层播放nextFeame的内容
// 如果frameList是循环链表，则循环播放动画
frameList initFrameList(void);
animationList createAnimationList(xmlTree xml, SDL_Renderer *renderer);
void addSprite(frameList frame, pictureNode pic, int delay);
void freeFrame(frameList frame);
void addAnimation(frameList frame, xmlTree xml, const char * name);

#endif
