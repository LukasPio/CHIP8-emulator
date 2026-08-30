#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "chip8.h"

#define BACKGROUND_COLOR 99, 66, 01, 255
#define SPRITE_COLOR 255, 204, 01, 255

void setupGraphics(void);
void setupInput(void);
void at_sdlerror(void);
void quit(void);
void handle_input(void);
void render(void);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Chip8 chip8 = {0};
int isToQuit = 0;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: chip8 <rom-path>\n");
        exit(1);
    }

    setupGraphics();
    setupInput();

    init_chip8(&chip8);
    load_game(&chip8, argv[1]);

    while (!isToQuit)
    {
        emulate_cycle(&chip8);

        handle_input();

        render();
    }

    quit();

    return 0;
}

void setupGraphics(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        at_sdlerror();

    window = SDL_CreateWindow("CHIP8",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              CHIP8_COLUMNS,
                              CHIP8_ROWS,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
        at_sdlerror();

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        at_sdlerror();

    SDL_RenderSetLogicalSize(renderer, CHIP8_COLUMNS, CHIP8_ROWS);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void setupInput(void)
{
}

void at_sdlerror(void)
{
    SDL_Log("Occurred an error: %s", SDL_GetError());
    SDL_Quit();
    exit(1);
}

void quit(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Program was successfully closed\n");
}

void handle_input(void)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            isToQuit = 1;
        }
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_q:
                isToQuit = 1;
                break;
            }
        }
    }
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, SPRITE_COLOR);
    for (int row = 0; row < CHIP8_ROWS; row++)
    {
        for (int column = 0; column < CHIP8_COLUMNS; column++)
        {
            if (chip8.display[row][column] == 1)
            {
                SDL_RenderDrawPoint(renderer, column, row);
            }
        }
    }

    SDL_RenderPresent(renderer);
}