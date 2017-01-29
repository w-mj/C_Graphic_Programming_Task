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

frameList initFrameList(int n)
{
    frameList start = (frameList)malloc(sizeof(struct FRAME_LIST));
    start -> picture = NULL;
    start -> nextFrame = NULL;
    frameList now = start;
    for (int i= 1; i < n; i++ ) {
        now -> nextFrame = (frameList)malloc(sizeof(struct FRAME_LIST));
        now = now -> nextFrame;
        now -> picture = NULL;
    }
    now -> nextFrame = NULL;
    return start;
}
// 可以用于改变一个图像，或者向帧链表里新增图像
void changePic(frameList frame, pictureNode pic, int s, int n)
{
    for (int i = 0; i < s; i++) { frame = frame -> nextFrame; }
    
    picList pics;
    bool copied;
    while (frame != NULL && s != n) {
        pics = frame -> picture;
        while (1) {
            copied = false;
            //如果某一个图像的名字与目标相等，则复制，并break
            if (strcmp(pics -> pic -> pictureName, pic -> pictureName) == 0) {
                copyPic(pics -> pic, pic); 
                copied = true;
                break;
            } else { 
                // 如果还有下一个，走到下一个，否则直接退出
                if (pics -> nextPic != NULL)
                    pics = pics -> nextPic;
                else {
                    copied = false;
                    break;
                }
            }
        }
        // 没有复制，新建节点后复制
        if ( !copied ) {
            pics -> nextPic = (picList)malloc(sizeof(struct PIC_LIST));
            pics = pics -> nextPic;
            copyPic(pics -> pic, pic);
        }
        frame = frame -> nextFrame;
        s++;
    } 
}

void removePic(frameList frame, const char * name)
{
    picList pics, last, del;
    while (frame != NULL) {
        pics = frame -> picture;
        last = NULL;
        while ( pics != NULL) {
            if (strcmp( name, pics -> pic -> pictureName) == 0) {
                if (last == NULL) {
                    del = pics;
                    frame -> picture = del -> nextPic;
                    free(del);
                } else {
                    last -> nextPic = del -> nextPic;
                    free(del);
                }
            } else {
                last = pics;
                pics = pics -> nextPic;
            }
        }
    }
}

void freeFrame(frameList frame)
{
    picList pics, del;
    frameList delf;
    while (frame != NULL) {
        pics = frame -> picture;
        while (pics != NULL) {
            del = pics;
            pics = del -> nextPic;
            free(del);
        }
        delf = frame;
        frame = delf -> nextFrame;
        free(delf);
    }
}
