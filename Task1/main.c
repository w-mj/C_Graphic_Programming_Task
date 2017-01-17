#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

const int SCREENWIDTH = 1000;
const int SCREENHEIGHT = 600;
const int CLOCKAREA = 600;
const int BPP = 32;
const int SECHANDLENGTH = 300;
const int MINHANDLENGTH = 200;
const int HOURHANDLENGTH = 150;
const int PI = 3.1415926;

SDL_Window *window = NULL;
SDL_Surface *screen =NULL;
SDL_Renderer * renderer = NULL;
SDL_Texture *button;

bool getDate(struct tm *setTime);
SDL_Texture *loadImage(const char * filename, bool colorkey);

bool GAPYEAR(int x){ return (x % 4 == 0 && x % 100 != 0) || x % 400 == 0;}

int init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return -1;
    window = SDL_CreateWindow("Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
        return -2;
    screen = SDL_GetWindowSurface(window);
    if (screen == NULL)
        return -3;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        return -4;
    TTF_Init();
    SDL_RenderClear(renderer);
    Mix_Init(MIX_INIT_MP3);
    button = loadImage("button.bmp", true);
    return 0;
}

void destroy()
{
    SDL_DestroyTexture(button);
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

SDL_Texture *loadImage(const char * filename, bool colorkey)
{
    SDL_Surface *file;
    SDL_Texture *image;
    file = SDL_LoadBMP(filename);
    if (file == NULL)
        printf("WrongFile");
    if (colorkey)
        SDL_SetColorKey(file, SDL_TRUE, SDL_MapRGB(file->format, 0xff, 0xff, 0xff));
    image = SDL_CreateTextureFromSurface(renderer, file);

    SDL_FreeSurface(file);
    return image;
}

void drawSecondHand(int sec)
{
    double theta = 6 * sec;
    theta = theta / 180.0 * PI; //角度化弧度
    int x = (CLOCKAREA / 2) + SECHANDLENGTH * sin( theta);
    int y = (CLOCKAREA / 2) - SECHANDLENGTH * cos( theta);
    SDL_RenderDrawLine( renderer, CLOCKAREA / 2, CLOCKAREA / 2, x, y );
}

void drawMinuteHand(int minute)
{
    double theta = 6 * minute;
    theta = theta / 180.0 * PI; //角度化弧度
    int x = (CLOCKAREA / 2) + MINHANDLENGTH * sin( theta);
    int y = (CLOCKAREA / 2) - MINHANDLENGTH * cos( theta);
    SDL_RenderDrawLine( renderer, CLOCKAREA / 2, CLOCKAREA / 2, x, y );
}
void drawHourHand(int hour)
{
    double theta = 30 * hour;
    theta = theta / 180.0 * PI; //角度化弧度
    int x = (CLOCKAREA / 2) + HOURHANDLENGTH * sin( theta);
    int y = (CLOCKAREA / 2) - HOURHANDLENGTH * cos( theta);
    //printf("%d\n", hour);
    SDL_RenderDrawLine( renderer, CLOCKAREA / 2, CLOCKAREA / 2, x, y );
}
//在location的位置显示文字, 如果bg为True，则添加一个半透明的背景
void showText(const char *text, SDL_Rect *location, TTF_Font *font, bool bg, SDL_Color textColor)
{
    SDL_Texture *textTexture = NULL;

    SDL_Surface *textSurface = NULL; //文本
    textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (bg) {
        SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF / 2);
        SDL_RenderFillRect(renderer, location);
    }
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);//渲染字体
    SDL_RenderSetViewport( renderer, location );//调整viewport
    SDL_RenderCopy(renderer, textTexture, NULL, NULL);
    SDL_RenderSetViewport( renderer, NULL );
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

int inWhichButton(int x, int y)
{
    if (x > 500 && x < 700 && y > 0 && y < 100)
        return 1;
    if (x > 800 && x < 1000 && y > 0 && y < 100)
        return 2;
    if (x > 800 && x < 1000 && y > 200 && y < 300)
        return 2;
    return 0;
}

bool sametime (struct tm * t1, struct tm * t2)
{
    if (t1 -> tm_year == t2 -> tm_year &&
        t1 -> tm_mon == t2 -> tm_mon &&
        t1 -> tm_mday == t2 -> tm_mday &&
        t1 -> tm_hour == t2 -> tm_hour &&
        t1 -> tm_min == t2 -> tm_min &&
        t1 -> tm_sec == t2 -> tm_sec
    )return true;
        else
        return false;
}
bool mousein(int x, int y, SDL_Rect area)
{
    if (x >= area.x && x <= area.x + area.w && y >= area.y && y <= area.y + area.h)
        return true;
    else
        return false;
}

void setAlarm(struct tm list[5] , int *num)
{
    int n = *num;
    char str[100];
    SDL_Rect buttonDel = {16, 327, 51, 51};
    SDL_Rect buttonAdd = {370, 357, 140, 40};
    SDL_Rect buttonOK = {370, 248, 140, 40};

    SDL_Rect subWindow = {100, 100, 800, 400};
    SDL_Rect item = {100, 0, 600, 50};
    SDL_Rect delButtonLoca = {700, 100, 50, 50};
    SDL_Rect addButtonLoca = {150, 450, 140, 40};
    SDL_Rect delButtonArea = {700, 100, 50, 300};
    SDL_Rect OKButtonLoca = {450, 450, 140, 40};

    SDL_Event event;
    bool quit = false;
    TTF_Font *font = NULL;
    SDL_Color black = { 0, 0, 0};
    font = TTF_OpenFont("fzssjt.ttf", 40);
    while (!quit) {
        showText("" , &subWindow, font, true, black);
        for (int i = 0; i < n; i++) {
            item.y = 100 + 50 * i;
            delButtonLoca.y = item.y;
            sprintf(str, "%02d:%02d:%02d  %02d/%02d/%4d", list[i].tm_hour + 8, list[i].tm_min, list[i].tm_sec, list[i].tm_mon + 1, list[i].tm_mday, list[i].tm_year + 1900);
            showText(str, &item, font, false, black);
            SDL_RenderCopy(renderer, button, &buttonDel, &delButtonLoca);
        }
        if (n <5) SDL_RenderCopy(renderer, button, &buttonAdd, &addButtonLoca);
        SDL_RenderCopy(renderer, button, &buttonOK, &OKButtonLoca);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent( & event)) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                if (mousein(x, y, addButtonLoca)) {
                        printf("%p\n", &list[n]);
                    if (getDate(&list[n]))
                        n++;
                }
                else if (mousein(x, y, OKButtonLoca)) {
                    *num = n;
                    quit = true;
                } else if (mousein(x, y, delButtonArea)) {
                    int deln = (y-100) / 50;
                    for (int i = deln; i < n; i++) {
                        memcpy(&list[i], &list[i+1], sizeof(struct tm));
                    }
                    n -= 1;
                }
            }
        }
    }
    TTF_CloseFont(font);
}

int main(int argc, char *argv[])
{
    SDL_Event event;
    SDL_Texture *bg = NULL; //背景图片
    SDL_Texture *clock = NULL; //表盘
    TTF_Font *font = NULL;  //字体
    bool quit = false, alarming = false;
    int status = 0;  //此变量用于设定当前界面状态，0为正常状态，1为设置闹钟，2为设置背景
    struct tm *nowTime;
    struct tm alarms[5]; //闹钟列表，最多有5个
    int nAlarms = 0; //闹钟的数量
    time_t tempTimer;
    SDL_Rect clockArea = {0, 0, CLOCKAREA, CLOCKAREA}; //钟表区域
    SDL_Rect textArea_SetAlarm =  {550, 0, 200, 100};
    SDL_Rect textArea_ChangePic =  {800, 0, 200, 100};
    SDL_Rect textArea_StopRing = { 800, 150, 200, 100};
    Mix_Music *ring;

    SDL_Color textColor = {0, 0, 0}; //按钮颜色为黑色
    SDL_Color red = {0xff, 0, 0};

    if (init(screen)) {
        fprintf(stderr,"%s\n",SDL_GetError());
        return 123;
    }
    bg = loadImage("bg.bmp", false);
    clock = loadImage("clock2.bmp", true);
    font = TTF_OpenFont("fzssjt.ttf", 28);
    if (font == NULL ) {
        fprintf(stderr,"%s\n",SDL_GetError());
        return 123;
    }
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    ring = Mix_LoadMUS("ring.mp3");
    if (ring == NULL) {
        fprintf(stderr,"%s\n",SDL_GetError());
        return 123;
    }

    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //支持alpha

    while( !quit ) {
        while (SDL_PollEvent( &event )) { //处理事件
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                if(mousein(x, y, textArea_SetAlarm)) setAlarm(alarms, &nAlarms);
                else if(mousein(x, y, textArea_StopRing)) if (Mix_PlayingMusic()) Mix_HaltMusic();
            }
            if (event.type == SDL_QUIT)
                quit = true;
        }

        SDL_RenderCopy(renderer, bg, NULL, NULL); //显示背景
        SDL_RenderCopy(renderer, clock, &clockArea, &clockArea); //显示表盘

        time(&tempTimer);
        nowTime = gmtime(&tempTimer);
        drawSecondHand(nowTime -> tm_sec);
        drawMinuteHand(nowTime -> tm_min);
        drawHourHand((nowTime -> tm_hour + 8 ) %12);//获得当前时间然后画指针


        showText("Set Alarm", &textArea_SetAlarm, font, true, textColor);  //显示两个按钮
        showText("Change Pic", &textArea_ChangePic, font, true, textColor);

        for (int i = 0; i < nAlarms; i++) {
            if (sametime(nowTime, &alarms[i])) {
                Mix_PlayMusic(ring, -1);
                break;
            }
        }

        if (Mix_PlayingMusic())
            showText("Stop", &textArea_StopRing, font, true, red);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(clock);
    Mix_FreeMusic(ring);
    destroy();
    return 0;
}

bool getDate(struct tm *setTime0)
{
    struct tm *setTime;
    int daysPerMonth[13] = {29 , 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    char str[20];
    struct tm *today;
    time_t t;
    SDL_Event event;
    TTF_Font *smallFont = NULL, *largeFont = NULL;
    bool quit = false, ok = false;
    int month, day, year, days, hour, minute, second;
    int week, row;
    int calenderTable[10][10]; // 把该月的每一天按日历存进这个数组，方便在鼠标点击得到结果
    int x, y;

    SDL_Color black = { 0, 0, 0};

    //各种控件的位置
    SDL_Rect calLoca = {100, 150, 400, 350};
    SDL_Rect weekBar = { 100, 100, 400, 50};
    SDL_Rect dayLoca = {0, 0, 57, 50}; // 日历中的每一个日期宽57，高50；
    SDL_Rect monthButtonLeft = {100, 450, 100, 50};
    SDL_Rect monthButtonRight = {400, 450, 100, 50};
    SDL_Rect monthBar = {200, 450, 200, 50};
    SDL_Rect hourButtonUp = {600, 100, 100, 100};
    SDL_Rect hourBar = { 600, 200, 100, 100};
    SDL_Rect hourButtonDown = {600, 300, 100, 100};
    SDL_Rect minuteButtonUp = {700, 100, 100, 100};
    SDL_Rect minuteBar = { 700, 200, 100, 100};
    SDL_Rect minuteButtonDown = {700, 300, 100, 100};
    SDL_Rect secondButtonUp = {800, 100, 100, 100};
    SDL_Rect secondBar = { 800, 200, 100, 100};
    SDL_Rect secondButtonDown = {800, 300, 100, 100};
    SDL_Rect showBar = {500, 400, 400, 50};
    SDL_Rect CancleButtonLoca = { 530, 440, 140, 40};
    SDL_Rect OKButtonLoca = { 730, 440, 140, 40};

    //打开字体
    smallFont = TTF_OpenFont("fzssjt.ttf", 40);
    largeFont = TTF_OpenFont("fzssjt.ttf", 55);

    //显示一个800x400的窗口
    SDL_Rect subWindow = {100, 100, 800, 400};

    //加载图像资源

    SDL_Rect buttonUp =  {0, 0, 100, 100};
    SDL_Rect buttonDown = {0, 100, 100, 100};
    SDL_Rect buttonRight ={100, 0, 100, 100};
    SDL_Rect buttonLeft = {100, 100, 100, 100};
    SDL_Rect buttonBg =   {0, 200, 200, 100};
    SDL_Rect buttonCancle = {370, 140, 140, 40};
    SDL_Rect buttonOK = {370, 248, 140, 40};


    //char calender[200]; //日历

    time(&t);
    setTime = gmtime(&t);
    month = setTime -> tm_mon + 1;
    year = setTime -> tm_year + 1900;
    day = setTime -> tm_mday;
    hour = setTime -> tm_hour + 8;
    minute = setTime -> tm_min;
    second = setTime -> tm_sec;

    while (!quit) {
        memset (calenderTable, 0, sizeof(calenderTable));
        showText(" ", &subWindow, smallFont, true, black); //借助显示文字的背景
        //getCalender(calender);
        if (month == 2 && GAPYEAR(year))
            daysPerMonth[2] = 29;
        else
            daysPerMonth[2] = 28;

        days = 1;
        for (int i = 1; i < month; i++) {
            days += daysPerMonth[i];
        } // days是某月一日是该年的第几天
        week = ( (year-1) + (year-1)/4 - (year/100) +(year/400) + days) % 7;
        //此公式来自 http://www.360doc.com/content/10/0517/14/11192_28043397.shtml
        //输出左侧的月历
        showText("Sun  Mon  Tue  Wed  Thr  Fri  Sat", &weekBar, smallFont, false, black);
        row = 0;
        for (int d = 1; d <= daysPerMonth[month]; d++) {
            dayLoca.x = 57 * week + 100;
            dayLoca.y = 50 * row + 150;
            calenderTable[row][week] = d;
            sprintf(str, " %2d ", d);
            showText(str, &dayLoca, smallFont, false, black);
            week++;
            if (week == 7) {
                week = 0;
                row++;
            }
        }
        SDL_RenderCopy(renderer, button, &buttonLeft, &monthButtonLeft);
        SDL_RenderCopy(renderer, button, &buttonRight, &monthButtonRight);
        sprintf(str, "%d.%02d", year, month);
        showText(str, &monthBar, largeFont, false, black);

        //显示调节时、分、秒
        SDL_RenderCopy(renderer, button, &buttonUp, &hourButtonUp);
        SDL_RenderCopy(renderer, button, &buttonUp, &minuteButtonUp);
        SDL_RenderCopy(renderer, button, &buttonUp, &secondButtonUp);
        SDL_RenderCopy(renderer, button, &buttonDown, &hourButtonDown);
        SDL_RenderCopy(renderer, button, &buttonDown, &minuteButtonDown);
        SDL_RenderCopy(renderer, button, &buttonDown, &secondButtonDown);
        sprintf(str, "%02d:", hour);
        showText(str, &hourBar, largeFont, false, black);
        sprintf(str, "%02d:", minute);
        showText(str, &minuteBar, largeFont, false, black);
        sprintf(str, "%02d ", second);
        showText(str, &secondBar, largeFont, false, black);

        //显示选择的时间日期
        sprintf(str, "%02d:%02d:%02d %02d/%02d/%04d ", hour, minute, second, month, day, year);
        showText(str, &showBar, smallFont, false, black);

        //显示两个按钮
        SDL_RenderCopy(renderer, button, &buttonOK, &OKButtonLoca);
        SDL_RenderCopy(renderer, button, &buttonCancle, &CancleButtonLoca);

        while ( (SDL_PollEvent( &event))) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                x = event.button.x;
                y = event.button.y;
                if (mousein(x, y, monthButtonLeft)) {
                    month -= 1;
                    if (month == 0) {
                        year -= 1;
                        month = 12;
                    }
                } else if (mousein(x, y, monthButtonRight)) {
                    month += 1;
                    if (month == 13) {
                        year += 1;
                        month = 1;
                    }
                } else if (mousein(x, y, hourButtonUp))  { hour += hour == 23? 0:1;}
                  else if (mousein(x, y, hourButtonDown)){ hour -= hour == 0?  0:1;}
                  else if (mousein(x, y, minuteButtonUp)){ minute += minute == 59?  0:1;}
                  else if (mousein(x, y, minuteButtonDown)){ minute -= minute == 0?  0:1;}
                  else if (mousein(x, y, secondButtonUp)){ second += second == 59?  0:1;}
                  else if (mousein(x, y, secondButtonDown)){ second -= second == 0?  0:1;}
                  else if (mousein(x, y, CancleButtonLoca)) {quit = true;}
                  else if (mousein(x, y, calLoca)) {
                        if (calenderTable[(y - 150) / 50][(x - 100) / 57])
                            day = calenderTable[(y - 150) / 50][(x - 100) / 57];
                } else if (mousein(x, y, OKButtonLoca)) {
                    setTime0 -> tm_year = year - 1900;
                    setTime0 -> tm_mon = month - 1;
                    setTime0 -> tm_mday = day;
                    setTime0 -> tm_hour = hour - 8;
                    setTime0 -> tm_min = minute;
                    setTime0 -> tm_sec = second;
                    ok = quit = true;

                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    //printf("%s\n", asctime(setTime));
    //printf("%p\n", setTime);
    TTF_CloseFont(smallFont);
    TTF_CloseFont(largeFont);
    return ok;
}


