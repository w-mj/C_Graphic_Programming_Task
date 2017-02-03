#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <SDL2/SDL_image.h>

struct PIC {
    char pictureName[128];
    SDL_Texture * imgSource;
    SDL_Rect imageLocation;
    SDL_Rect displayLocation;
};
typedef struct PIC * pictureNode;

struct FRAME_LIST {
    pictureNode picture;
    int delay;
    struct FRAME_LIST *nextFrame;
};
typedef struct FRAME_LIST *frameList;

// 说明：
// 设定为30fps，每一帧图像33ms
// 在需要的地方生成30元素的帧数组，每一个数组元素是picList
// 显示时每过33ms显示下一个图像，每次显示时遍历整个picList并显示
frameList initFrameList(void);
// 修改frame中的s到n帧为pic，如果没有相应的图片则新建 [s, n)
void changePic(frameList frame, pictureNode pic, int delay);
void freeFrame(frameList frame);

#endif
