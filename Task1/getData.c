bool getDate(struct tm *setTime)
{
    const int days[13] = {29 , 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    struct tm *today;
    time_t t;
    SDL_Event event;
    TTF_Font *font = NULL;
    bool quit = false;
    int month;


    SDL_Rect calLoca = { 100, 100, 400, 300};
    SDL_Rect monLoca = { 100, 400, 400, 100};
    SDL_Rect dayLoca = {0, 0, 57, 50}; // 日历中的每一个日期宽57，高50；


    //显示一个800x400的窗口
    SDL_Rect subWindow = {100, 100, 800, 400};
    showText("", &subWindow, true); //借助显示文字的背景
    //加载图像资源
    SDL_Texture *button = loadImage("button.bmp", true);
    SDL_Rect buttonTop =  {0, 0, 100, 100};
    SDL_Rect buttonDown = {0, 100, 100, 100};
    SDL_Rect buttonRight ={100, 0, 100, 100};
    SDL_Rect buttonLeft = {100, 100, 100, 100};
    SDL_Rect buttonBg =   {0, 200, 200, 100};
    //打开字体
    smallFont = TTF_OpenFont("fzssjt.ttf", 12);
    largeFont = TTF_OpenFont("fzssjt.ttf", 24);

    //char calender[200]; //日历

    time(t);
    setTime = gmtime(t);
    month = setTime -> tm_mon;

    while (!quit) {
        //getCalender(calender);

        week = getFirstDay(month); //获得该月第一天是星期几
        if
        for (int d = 1; d <= days[setTime->])

        while ( (SDL_PollEvent( &event))) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {

            }
        }
    }
}
