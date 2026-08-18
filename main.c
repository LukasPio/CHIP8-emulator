#include <SDL.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "chip8.h"

void setupGraphics();
void at_sdlerror();
void handle_input();
void render();

SDL_Window *window;
Chip8 chip8 = {0};
int quit = 0;

int main(void)
{
    setupGraphics();

    while (!quit)
    {
        handle_input();
        render();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void setupGraphics()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        at_sdlerror();
    window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CHIP8_ROWS, CHIP8_COLUMNS, SDL_WINDOW_FULLSCREEN);
    if (window == NULL)
        at_sdlerror();
}

void at_sdlerror()
{
    SDL_Log("Occurred an error: %s", SDL_GetError());
    SDL_Quit();
    exit(1);
}

void handle_input()
{
    SDL_Event e;

    while (SDL_PoolEvent(&e))
    {
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.scancode)
            {
            case SDLK_q:
                quit = 1;
                break;
            }
        }
    }
}

void render()
{

}