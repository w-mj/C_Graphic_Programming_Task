#define PI 3.141592653
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "animation.h"
#include "sdl.h"
#include "xml.h"
#include "weapon.h"
#include "monster.h"


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Color color = {0, 0, 0, 0xff};
unsigned int score;


struct {
    int x, y;
    int h, w;
    int speed;
    char weapon[128];
    int helth;
} playerStatus;

struct {
    int x, y;
    int h, w;
    int targetX, targetY;
    int speed;
    int moveMethod;
    int attackMethod;
    int attackFrequency; // 每几毫秒攻击一次
    int health;
    double angle;
    bool exist;
} monsters[15];

struct {
    int x, y;
    int h, w;
    float angle; // 倾斜角 
    bool exist;
    weaponList statu;
} bullet[20];

bool isSuperpose(SDL_Rect * zone1, SDL_Rect * zone2)
{
    int offset = 10;
    zone1 ->x -= 5;
    zone1 ->y -= 5;
    zone1 ->w -= 5;
    zone1 ->h -= 5;
    zone2 ->x -= 5;
    zone2 ->y -= 5;
    zone2 ->w -= 5;
    zone2 ->h -= 5;

    if ((zone1 -> x > zone2 -> x && zone1 -> x < zone2 -> x + zone2 -> w && zone1 -> y > zone2 -> y && zone1 -> y < zone2 -> y + zone2 -> h) ||
        (zone1 -> x +  zone1 -> w > zone2 -> x && zone1 -> x +  zone1 -> w< zone2 -> x + zone2 -> w && zone1 -> y > zone2 -> y && zone1 -> y < zone2 -> y + zone2 -> h) ||
        (zone1 -> x > zone2 -> x && zone1 -> x < zone2 -> x + zone2 -> w && zone1 -> y + zone1 -> h > zone2 -> y && zone1 -> y + zone1 -> h< zone2 -> y + zone2 -> h) ||
        (zone1 -> x +  zone1 -> w> zone2 -> x && zone1 -> x +  zone1 -> w< zone2 -> x + zone2 -> w && zone1 -> y + zone1 -> h> zone2 -> y && zone1 -> y + zone1 -> h< zone2 -> y + zone2 -> h))
        return true;
    else 
        return false;
       
}
void crashCheck(void)
{
    SDL_Rect player, monster, bulletzone;
    player.x = playerStatus.x;
    player.y = playerStatus.y;
    player.w = playerStatus.w;
    player.h = playerStatus.h;
    for (int i = 0; i < 15; i++) {
        if (monsters[i].exist == false)
            continue;
        monster.x = monsters[i].x;
        monster.y = monsters[i].y;
        monster.w = monsters[i].w;
        monster.h = monsters[i].h;
        if (isSuperpose(&player, &monster)) {
            playerStatus.helth -= 1;
            monsters[i].health -= 1;
            if (monsters[i].health <= 0)
                monsters[i].exist = false;
        }
        for (int j = 0; j < 20; j++) {
            if (bullet[j].exist == false)
                continue;
            bulletzone.x = bullet[j].x;
            bulletzone.y = bullet[j].y;
            bulletzone.h = bullet[j].h;
            bulletzone.w = bullet[j].w;
            if (isSuperpose(&bulletzone, &monster)) {
                score += 5;
                monsters[i].health -= bullet[j].statu -> power;
                bullet[j].exist = false;
                if (monsters[i].health <= 0)
                    monsters[i].exist = false;
            }
        }
        if (monsters[i].x < 0 || monsters[i].x > 800 || monsters[i].y < 0 || monsters[i].y > 600)
            monsters[i].exist = false;
    }
}

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
            //fprintf(stderr, "%d %d %d %d %f\n", x, y, targetX, targetY, bullet[i].angle);
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
            bullet[i].w = bullet[i].statu -> bullet -> imageLocation.w;
            bullet[i].h = bullet[i].statu -> bullet -> imageLocation.h;
        }
    }
}

frameList createMonster(const char *name, monsterList m, int i)
{
    while (m != NULL) {
        if (strcmp(name, m->name) == 0) {
            // fprintf(stderr, "dig\n");
            monsters[i].exist = true;
            monsters[i].speed = m -> speed;
            monsters[i].moveMethod = m -> moveMethod;
            monsters[i].attackMethod = m -> attackMethod;
            monsters[i].attackFrequency = m -> attackFrequency;
            monsters[i].health = m -> health;
            monsters[i].x = 800;
            monsters[i].y = rand() % 300;
            if (monsters[i].attackMethod == 1) {
                monsters[i].targetX = 300;
                monsters[i].targetY = rand() % 600;
            }
            monsters[i].angle = atan2((double)monsters[i].y-monsters[i].targetY ,monsters[i].x - (double)monsters[i].targetX);
            return addAnimation(m -> animation, "default");
        }
        m = m -> next;
    }
}
void moveMonsters(void)
{
    for (int i = 0; i < 15; i++) {
        if (monsters[i].exist) {
            if (monsters[i].moveMethod == 1) {
                monsters[i].x -= monsters[i].speed * cos(monsters[i].angle);
                monsters[i].y -= monsters[i].speed * sin(monsters[i].angle);
                //  if (i == 0)
                // printf("%lf  %d  %d  \n", monsters[i].angle, monsters[i].x, monsters[i].y);
            }
        }
    }
}
void initGame(void)
{
    playerStatus.x = 0;
    playerStatus.y = 322;
    score = 0;
    strcpy(playerStatus.weapon, "gun");
    playerStatus.helth = 1;
    playerStatus.speed= 5;
    for (int i = 0; i < 20; i++) {
        bullet[i].exist = false;
    }
    for (int i = 0; i < 15; i++) {
        monsters[i].exist = false;
    }
    srand(time(NULL));
}

void showScore(void)
{
    SDL_Rect zone = {0, 0, 34, 34};
    char text[20];
    sprintf(text, "%u", score);
    // fprintf(stderr, "%s\n", text);
    SDL_Surface *message = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *msg = SDL_CreateTextureFromSurface(renderer, message);
    zone.w = zone.h * strlen(text) / 2;
    SDL_RenderCopy(renderer, msg, NULL, &zone);
    SDL_FreeSurface(message);
    SDL_DestroyTexture(msg);
}

int main(int argc, char *argv[])
{
    initSDL(&window, &renderer);
    TTF_Init();
    initGame();
    SDL_Event event;
    SDL_Texture *bg = loadImg("bg.bmp", renderer);
    SDL_Texture *youDead = loadImg("dead.bmp", renderer);

    if (bg == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(2);
    }
    font = TTF_OpenFont("fzssjt.ttf", 26);
    if (font == NULL) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(3);
    }
    xmlTree character = loadXML("player.xml");
    xmlTree weaponXML = loadXML("weapon.xml");
    xmlTree monsterXML = loadXML("monsters.xml");
    animationList player = createAnimationList(character, renderer);
    weaponList weapons = createWeaponList(weaponXML, renderer);
    monsterList monsterls = createMonsterList(monsterXML, renderer);

    frameList frames[50]; //1图层为玩家，[5,20)图层为敌人
    for (int i = 0; i < 50; i++)
        frames[i] = NULL;

    int lastPresentTime = 0; 
    int times;
    int moveSpeed = 0;
    int x, y;
    SDL_Rect display;

    bool quit = false;
    bool restart = false;
    
    frames[1] = addAnimation(player, "stay");
    playerStatus.h = frames[1] -> picture -> imageLocation .h;
    playerStatus.w = frames[1] -> picture -> imageLocation .w;
    while (!quit) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN) {
                //fprintf(stderr, "keydown\n");
                switch(event.key.keysym.sym) {
                    case SDLK_d:
                        if (moveSpeed > 0)
                            break;
                        frames[1] = addAnimation(player, "moveRight");
                        moveSpeed = playerStatus.speed;
                        break;
                    case SDLK_a:
                        if (moveSpeed < 0)
                            break;
                        frames[1] = addAnimation(player, "moveRight");
                        moveSpeed = -playerStatus.speed;
                        break;
                }
            }
            else if (event.type == SDL_KEYUP) {

                //fprintf(stderr, "keyup\n");
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
        if (rand() % 10 == 0) {
            for (int i = 0; i< 15; i++)
                if (monsters[i].exist == false) {
                    frames[i + 5] = createMonster("bird", monsterls,  i);
                    monsters[i].h = frames[i + 5] -> picture -> imageLocation.h;
                    monsters[i].w = frames[i + 5] -> picture -> imageLocation.w;
                }
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
                if (i >= 5 && i < 20) // 跳过不存在的怪物
                    if (monsters[i - 5].exist == false)
                        continue;
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
        showScore();
        drawBullet();
        moveMonsters();
        crashCheck();
        if (playerStatus.helth <= 0) {
            // fprintf(stderr, "you dead\n");
            SDL_RenderCopy(renderer, youDead, NULL, NULL);
            SDL_RenderPresent(renderer);
            while(!(quit || restart)) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT)
                        quit = true;
                    else if (event.type == SDL_KEYDOWN) {
                        initGame();
                        restart = true;
                    }
                }
            }
        }
        restart = false;
        times = SDL_GetTicks();
        if (times - lastPresentTime < ( 1000 / 30))
            SDL_Delay(1000 / 30 - (times - lastPresentTime));
        SDL_RenderPresent(renderer);
        lastPresentTime = SDL_GetTicks();

    }
    destroyXMLTree(character);
    destroyXMLTree(weaponXML);
    destroyXMLTree(monsterXML);
    destroyWeaponList(weapons);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(bg);
    destroySDL(window, renderer);
    return 0;
}
