#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

const int SCREENWIDTH = 800;
const int SCREENHIGHT = 600;
const int BPP = 32;
const int MOUSEOVER = 0;
const int MOUSEOUT = 1;
const int MOUSEDOWN = 2;
const int MOUSEUP = 3;
SDL_Rect clips[4];

SDL_Surface *init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Surface *screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHIGHT, BPP, SDL_SWSURFACE);
    TTF_Init();
    return screen;
}

void draw(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, clip, destination, &offset);
}
void setClips()
{
    clips[ MOUSEOVER ].x = 0;
    clips[ MOUSEOVER ].y = 0;
    clips[ MOUSEOVER ].w = 320;
    clips[ MOUSEOVER ].h = 240;

    clips[ MOUSEOUT ].x = 320;
    clips[ MOUSEOUT ].y = 0;
    clips[ MOUSEOUT ].w = 320;
    clips[ MOUSEOUT ].h = 240;

    clips[ MOUSEDOWN ].x = 0;
    clips[ MOUSEDOWN ].y = 240;
    clips[ MOUSEDOWN ].w = 320;
    clips[ MOUSEDOWN ].h = 240;

    clips[ MOUSEUP ].x = 320;
    clips[ MOUSEUP ].y = 240;
    clips[ MOUSEUP ].w = 320;
    clips[ MOUSEUP ].h = 240;
}

SDL_Surface *loadImage(char *name)
{
    SDL_Surface *image = NULL, *opitimizedImage = NULL;

    image = IMG_Load(name);
    if (image != NULL) {
        opitimizedImage = SDL_DisplayFormat(image);
        SDL_FreeSurface(image);
    }
    return opitimizedImage;
}

int main(void)
{
    SDL_Surface *screen;
    TTF_Font *font = NULL;
    SDL_Color textcolor = {0, 0, 0};
    SDL_Surface *button = NULL, *text = NULL;
    SDL_Event event;
    SDL_Rect box, blank;
    int x, y;
    bool quit = false;
    char str[128] = {0};
    box.x = (SCREENWIDTH - 320) / 2;
    box.y = (SCREENHIGHT - 240) / 2;
    box.w = 320;
    box.h = 240;
    blank.x = 10;
    blank.y = 12;
    blank.w = 290;
    blank.h = 40;
    screen = init();
    font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 28);
    if (font == NULL)
        return 99;
    button = loadImage("button.png");
    setClips();
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF,0xFF,0xFF));
    draw(box.x, box.y, button, screen, &clips[MOUSEOUT]);
    while (!quit) {
        while (SDL_PollEvent( &event)) {
            if (event.type == SDL_MOUSEMOTION) {
                x = event.motion.x;
                y = event.motion.y;
                sprintf(str, "X = %d, Y = %d", x, y);
                text = TTF_RenderText_Solid(font, str, textcolor);
                draw(0, 0, button, screen, &blank);
                draw(0, 0, text, screen, NULL);
                if (x > box.x && x < box.x + box.w && y > box.y && y < box.y + box.h) {
                    draw(box.x, box.y, button, screen, &clips[MOUSEOVER]);
                }
                else {
                    draw(box.x, box.y, button, screen, &clips[MOUSEOUT]);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                x = event.button.x;
                y = event.button.y;
                if (x > box.x && x < box.x + box.w && y > box.y && y < box.y + box.h) {
                    draw(box.x, box.y, button, screen, &clips[MOUSEDOWN]);
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                x = event.button.x;
                y = event.button.y;
                if (x > box.x && x < box.x + box.w && y > box.y && y < box.y + box.h) {
                    draw(box.x, box.y, button, screen, &clips[MOUSEUP]);
                }
            }
            if (event.type == SDL_QUIT)
                quit = true;
        }

        SDL_Flip(screen);
    }

    SDL_FreeSurface(screen);
    SDL_FreeSurface(button);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
