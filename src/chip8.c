#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include "chip8.h"

#define FONT_START 0x50

static const uint8_t font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void init_chip8(Chip8 *chip8)
{
    memset(chip8, 0, sizeof(*chip8));

    memcpy(&chip8->memory[FONT_START], font, sizeof(font));

    chip8->pc = 0x200;

    srand(time(NULL));
}

void load_game(Chip8 *chip8, const char *game_name)
{
    char path[128];

    snprintf(path, sizeof(path), "../games/%s.ch8", game_name);

    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long game_size = ftell(file);
    rewind(file);

    if (game_size < 0)
    {
        perror("ftell");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (game_size > sizeof(chip8->memory) - 0x200)
    {
        fprintf(stderr, "ROM is too large\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t bytes_read = fread(
        &chip8->memory[0x200],
        1,
        game_size,
        file);

    if (bytes_read != (size_t)game_size)
    {
        fprintf(stderr, "Failed to read complete ROM\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

// 00E0

void emulate_cycle(Chip8 *chip8)
{
    uint16_t opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;

    uint16_t nnn = opcode & 0x0FFF;
    uint8_t nn = opcode & 0x00FF;
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0xF000)
    {
    case 0x0000:
        if (opcode == 0x00E0)
        {
            memset(chip8->display, 0, sizeof(chip8->display));
        }
        break;
    case 0x6000:
        chip8->V[x] = nn;
        break;
    case 0xA000:
        chip8->I = nnn;
        break;
    case 0xD000:
    }
}