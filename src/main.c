#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <threads.h>
#include "chip8.h"

#define CPU_HZ 700
#define TIMER_HZ 60
#define NS_PER_SEC 1000000000LL
#define CPU_TIME_SLICE_NS (NS_PER_SEC / CPU_HZ)
#define TIMER_TIME_SLICE_NS (NS_PER_SEC / TIMER_HZ)

#define SAMPLE_RATE 44100
#define BEEP_FREQ 440
#define AMPLITUDE 3000

#define BACKGROUND_COLOR 99, 66, 1, 255
#define SPRITE_COLOR 255, 204, 1, 255

void setup_graphics(void);
void setup_audio(void);
void audio_callback(void *user_data, Uint8 *stream, int length);
void update_audio(void);
void at_sdl_error(void);
void quit(void);
void handle_input(void);
void render(void);

int64_t timespec_to_ns(struct timespec time);
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_AudioDeviceID audio_device = 0;
chip8_t chip8 = {0};

const SDL_Keycode emulation_keys[] = {
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_r,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_f,
    SDLK_z,
    SDLK_x,
    SDLK_c,
    SDLK_v,
};
int is_to_quit = 0;
int audio_playing = 0;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: chip8 <rom_path>\n");
        return EXIT_FAILURE;
    }
    setup_graphics();
    setup_audio();
    init_chip8(&chip8);
    load_game(&chip8, argv[1]);
    struct timespec last_cpu_time;
    struct timespec last_timer_time;
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &last_cpu_time);
    last_timer_time = last_cpu_time;
    while (!is_to_quit)
    {
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        int64_t current_ns = timespec_to_ns(current_time);
        int64_t last_cpu_ns = timespec_to_ns(last_cpu_time);
        if (current_ns - last_cpu_ns >= CPU_TIME_SLICE_NS)
        {
            emulate_cycle(&chip8);
            last_cpu_time = current_time;
        }
        int64_t last_timer_ns = timespec_to_ns(last_timer_time);
        if (current_ns - last_timer_ns >= TIMER_TIME_SLICE_NS)
        {
            decrease_timers(&chip8);
            last_timer_time = current_time;
        }
        update_audio();
        handle_input();
        render();
        struct timespec sleep_time = {
            .tv_sec = 0,
            .tv_nsec = 1000000};
        thrd_sleep(&sleep_time, NULL);
    }
    quit();
    return EXIT_SUCCESS;
}
int64_t timespec_to_ns(struct timespec time)
{
    return (int64_t)time.tv_sec * NS_PER_SEC + time.tv_nsec;
}
void setup_graphics(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0)
        at_sdl_error();
    window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CHIP8_COLUMNS, CHIP8_ROWS, SDL_WINDOW_SHOWN);
    if (window == NULL)
        at_sdl_error();
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        at_sdl_error();
    SDL_RenderSetLogicalSize(renderer, CHIP8_COLUMNS, CHIP8_ROWS);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}
void setup_audio(void)
{
    SDL_AudioSpec want = {0};
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 1024;
    want.callback = audio_callback;
    want.userdata = NULL;
    audio_device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    if (audio_device == 0)
    {
        SDL_Log("Failed to open audio device: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_PauseAudioDevice(audio_device, 1);
    audio_playing = 0;
}
void audio_callback(void *user_data, Uint8 *stream, int length)
{
    (void)user_data;
    int16_t *samples = (int16_t *)stream;
    int sample_count = length / sizeof(int16_t);
    static int phase = 0;
    int samples_per_period = SAMPLE_RATE / BEEP_FREQ;
    for (int i = 0; i < sample_count; i++)
    {
        if (phase < samples_per_period / 2)
            samples[i] = AMPLITUDE;
        else
            samples[i] = -AMPLITUDE;
        phase++;
        if (phase >= samples_per_period)
            phase = 0;
    }
}
void update_audio(void)
{
    if (chip8.sound_timer > 0)
    {
        if (!audio_playing)
        {
            SDL_PauseAudioDevice(audio_device, 0);
            audio_playing = 1;
        }
    }
    else
    {
        if (audio_playing)
        {
            SDL_PauseAudioDevice(audio_device, 1);
            audio_playing = 0;
        }
    }
}
void at_sdl_error(void)
{
    SDL_Log("Occurred an SDL error: %s", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
void quit(void)
{
    if (audio_device != 0)
    {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
    printf("Program was successfully closed\n");
}
void handle_input(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            is_to_quit = 1;
            break;
        case SDL_KEYUP:
            for (int i = 0; i < CHIP8_KEYS_AMOUNT; i++)
            {
                if (emulation_keys[i] == e.key.keysym.sym)
                {
                    chip8.keys[i] = 0;
                    break;
                }
            }
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_MINUS:
                is_to_quit = 1;
                break;
            default:
                for (int i = 0; i < CHIP8_KEYS_AMOUNT; i++)
                {
                    if (emulation_keys[i] == e.key.keysym.sym)
                    {
                        chip8.keys[i] = 1;
                        break;
                    }
                }
                break;
            }
            break;
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
            if (chip8.display[row][column])
            {
                SDL_RenderDrawPoint(renderer, column, row);
            }
        }
    }
    SDL_RenderPresent(renderer);
}
