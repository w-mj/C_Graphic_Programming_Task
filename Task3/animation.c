#include <stdlib.h>
#include "animation.h"
#include <stdbool.h>

void copyPic(pictureNode target, const pictureNode source)
{
    strcpy(target -> pictureName, source -> pictureName);
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
    start -> picture = NULL;
    start -> nextFrame = NULL;
    return start;
}
// 可以用于改变一个图像，或者向帧链表里新增图像
void addSprite(frameList frame, pictureNode pic, int delay)
{
    if (frame -> picture!= NULL) {
        while(frame -> nextFrame != NULL)
            frame = frame -> nextFrame;
        frame -> nextFrame = (frameList)malloc(sizeof(struct FRAME_LIST));
        frame = frame -> nextFrame;
    }
    frame -> delay = delay;
    frame -> picture = (pictureNode)malloc(sizeof(struct PIC));
    copyPic(frame -> picture, pic);
    frame -> nextFrame = NULL;
}


void freeFrame(frameList frame)
{
    frameList del;
    del = frame;
    frame = frame -> nextFrame;
    while (frame != NULL) {
        if (frame == del) { // 如果是循环链表，则把循环链表变成普通链表
            frame = frame -> nextFrame;
            del -> nextFrame = NULL;
            break;
        }
        frame = frame -> nextFrame;
    }
    if (frame == NULL) // 不是循环链表，最终fream必等于NULL，给它复原成第一个元素地址
        frame = del;

    while (frame != NULL) {
        del = frame;
        frame = frame -> nextFrame;
        free(del -> picture);
        free(del);
    }
}
