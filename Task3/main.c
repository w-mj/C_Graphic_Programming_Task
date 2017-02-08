#define PI 3.141592653
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "animation.h"
#include "sdl.h"
#include "xml.h"
#include "weapon.h"


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

struct {
    int x, y;
    int speed;
    char weapon[128];
    int helth;
} playerStatus;

struct {
    int x, y;
    int targetX, targetY;
    int speed;
    int moveMethod;
    int attackMethod;
    int attackFrequency; // 每几毫秒攻击一次
    int health;
} monsters[15];

struct {
    int x, y;
    float angle; // 倾斜角 
    bool exist;
    weaponList statu;
} bullet[20];

void createBullet(int x, int y, int targetX,int  targetY, weaponList w)
{
    int i;
    for (i = 0; i < 20; i++) 
        if (bullet[i].exist == false)
            break;
    if (i == 20)
        return;
    while (w != NULL) {
        if (strcmp(w -> name, playerStatus.weapon) == 0) {
            bullet[i].x = x;
            bullet[i].y = y;
            if (targetX == x)
                bullet[i].angle = 90;
            else 
                bullet[i].angle = atan2(((double)targetY - y) , ((double)targetX - x)) * 180 / PI; // 反正切求角度并转化成角度制
            fprintf(stderr, "%d %d %d %d %f\n", x, y, targetX, targetY, bullet[i].angle);
            bullet[i].statu = w; // 主要是记录这个子弹的图像
            bullet[i].exist = true;
            return ;
        }
    }
    fprintf(stderr, "No such a bullet");
    return;
}
//得到当前图层的位置
void getLocation(int *x, int *y, int i)
{
    if (i == 1) {
        *x = playerStatus.x;
        *y = playerStatus.y;
    }
    else if (i >= 5 && i < 20) {
        *x = monsters[i - 5].x;
        *y = monsters[i - 5].y;
    }
    else {
        *x = 0;
        *y = 0;
    }

}


void drawbg(int x, SDL_Texture *bg)
{
    SDL_Rect loca = {x, 0, 800, 600};
    SDL_RenderCopy(renderer, bg, NULL, &loca);
}

void drawBullet()
{
    SDL_Rect rect = {0, 0, 0, 0};
    for (int i = 0; i < 20; i++) {
        if (bullet[i].exist) {
            if (bullet[i].x > 820 || bullet[i].y > 620 || bullet[i].x < -20 || bullet[i].y < -20) {
                bullet[i].exist = false;
                continue;
            }
            rect.x = bullet[i].x;
            rect.y = bullet[i].y;
            rect.w = 48;
            rect.h = 48;
            SDL_Point pp = {0, 0};
            SDL_RenderCopyEx(renderer, bullet[i].statu-> bullet -> imgSource, &bullet[i].statu -> bullet -> imageLocation, &rect, bullet[i].angle, &pp, SDL_FLIP_NONE);
            bullet[i].x += bullet[i].statu -> speed * cos(bullet[i].angle / 180 * PI);
            bullet[i].y += bullet[i].statu -> speed * sin(bullet[i].angle / 180 * PI);
        }
    }
}

void initGame(void)
{
    playerStatus.x = 0;
    playerStatus.y = 322;
    strcpy(playerStatus.weapon, "gun");
    playerStatus.helth = 10;
    playerStatus.speed= 5;
    for (int i = 0; i < 20; i++) {
        bullet[i].exist = false;
    }
}

int main(int argc, char *argv[])
{
    initSDL(&window, &renderer);
    initGame();
    SDL_Event event;
    SDL_Texture *bg = loadImg("bg.bmp", renderer);
    if (bg == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(2);
    }
    xmlTree character = loadXML("player.xml");
    xmlTree weaponXML = loadXML("weapon.xml");
    animationList player = createAnimationList(character, renderer);
    weaponList weapons = createWeaponList(weaponXML, renderer);

    frameList frames[50]; //1图层为玩家，[5,20)图层为敌人
    for (int i = 0; i < 50; i++)
        frames[i] = NULL;

    int lastPresentTime = 0, lastPressTime = 0; 
    int times;
    int moveSpeed = 0;
    int x, y;
    SDL_Rect display;

    bool quit = false;
    while (!quit) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN) {
                lastPressTime = SDL_GetTicks();
                fprintf(stderr, "keydown\n");
                switch(event.key.keysym.sym) {
                    case SDLK_d:
                        if (moveSpeed > 0)
                            break;
                        frames[1] = addAnimation(player, "moveRight");
                        moveSpeed = playerStatus.speed;
                        break;
                    case SDLK_a:
                        frames[1] = addAnimation(player, "moveRight");
                        moveSpeed = -playerStatus.speed;
                        break;
                }
            }
            else if (event.type == SDL_KEYUP) {

                fprintf(stderr, "keyup\n");
                switch(event.key.keysym.sym) {
                    case SDLK_d:
                        moveSpeed = 0;
                        frames[1] = addAnimation(player, "stay");
                        break;
                    case SDLK_a:
                        moveSpeed = 0;
                        frames[1] = addAnimation(player, "stay");
                        break;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    getLocation(&x, &y, 1);
                    createBullet(x, y, event.button.x, event.button.y, weapons ); // 点击鼠标，创建一颗子弹
                }
            }
            else
                continue;
        }

        playerStatus.x += moveSpeed;
        if (playerStatus.x >= 800)
            playerStatus.x -= moveSpeed;
        if (playerStatus.x <= 0)
            playerStatus.x = 0;
        //SDL_RenderClear(renderer);
        drawbg(0, bg);
        for (int i = 0; i < 50; i++) {
            if (frames[i] != NULL) {
                if (frames[i] -> picture != NULL) {
                    // 显示当前图片
                    getLocation(&x, &y, i);
                    display.x = frames[i]->picture->displayLocation.x + x;
                    display.y = frames[i]->picture->displayLocation.y + y; //显示之前先获得位置
                    display.w = frames[i]->picture->displayLocation.w;
                    display.h = frames[i]->picture->displayLocation.h;
                    SDL_RenderCopy(renderer, frames[i]->picture->imgSource, &frames[i] -> picture -> imageLocation, &display);
                    // 显示完一张图片后，计数器加一，如果count等于delay，说明这个图片显示完了，切换到下一帧
                    frames[i] -> count++;
                    if (frames[i] -> count == frames[i] -> delay) {
                        frames[i] -> count = 0;
                        // 如果还有下一帧的话，显示下一帧，否则删除掉这个动画
                        if (frames[i] -> nextFrame != NULL) {
                            frames[i] = frames[i] -> nextFrame;
                        } else {
                            frames[i] = NULL;
                        }
                    }
                }
            }
        }
        drawBullet();
        times = SDL_GetTicks();
        if (times - lastPresentTime < ( 1000 / 30))
            SDL_Delay(1000 / 30 - (times - lastPresentTime));
        SDL_RenderPresent(renderer);
        lastPresentTime = SDL_GetTicks();

    }
    destroyXMLTree(character);
    destroyWeaponList(weapons);
    SDL_DestroyTexture(bg);
    destroySDL(window, renderer);
    return 0;
}
