#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "chip8.h"

void init_chip8();
void at_sdlerror();

SDL_Window *window;
Chip8 chip8;

int main(void) 
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) at_sdlerror();

    window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CHIP8_ROWS, CHIP8_COLUMNS, SDL_WINDOW_FULLSCREEN);
    if (window == NULL) at_sdlerror();

    return 0;
}

void init_chip8()
{

}

void at_sdlerror()
{
    SDL_Log("Occurred an error: %s", SDL_GetError());
    SDL_Quit();
    exit(1);
}