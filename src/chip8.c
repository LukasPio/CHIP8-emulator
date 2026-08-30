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
    chip8->sp = 0;

    srand(time(NULL));
}

void load_game(Chip8 *chip8, const char *rompath)
{
    FILE *file = fopen(rompath, "rb");

    if (file == NULL)
    {
        printf("ROM was not found on %s\n", rompath);
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    unsigned long game_size = ftell(file);
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
        else if (opcode == 0x00EE)
        {
            chip8->sp--;
            chip8->pc = chip8->stack[chip8->sp];
        }
        break;
    case 0x1000:
        chip8->pc = nnn;
        break;
    case 0x2000:
        chip8->stack[chip8->sp] = chip8->pc;
        chip8->sp++;
        chip8->pc = nnn;
        break;
    case 0x3000:
        if (chip8->V[x] == nn)
            chip8->pc += 2;
        break;
    case 0x4000:
        if (chip8->V[x] != nn)
            chip8->pc += 2;
        break;
    case 0x5000:
        if (chip8->V[x] == chip8->V[y])
            chip8->pc += 2;
        break;
    case 0x6000:
        chip8->V[x] = nn;
        break;
    case 0x7000:
        chip8->V[x] += nn;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            chip8->V[x] = chip8->V[y];
            break;
        case 0x0001:
            chip8->V[x] |= chip8->V[y];
            break;
        case 0x0002:
            chip8->V[x] &= chip8->V[y];
            break;
        case 0x0003:
            chip8->V[x] ^= chip8->V[y];
            break;
        case 0x0004:
            chip8->V[x] += chip8->V[y];
            break;
        case 0x0005:
            chip8->V[x] -= chip8->V[y];
            break;
        case 0x0006:
            chip8->V[x] >>= 1;
            break;
        case 0x0007:
            chip8->V[x] = chip8->V[y] - chip8->V[x];
            break;
        case 0x000E:
            chip8->V[x] <<= 1;
            break;
        }
        break;
    case 0x9000:
        if (chip8->V[x] != chip8->V[y])
            chip8->pc += 2;
        break;
    case 0xA000:
        chip8->I = nnn;
        break;
    case 0xD000:
    {
        uint8_t start_x = chip8->V[x];
        uint8_t start_y = chip8->V[y];
        chip8->V[0xF] = 0;

        for (uint8_t row = 0; row < n; row++)
        {
            uint8_t spriterow = chip8->memory[chip8->I + row];
            for (uint8_t column = 0; column < 8; column++)
            {
                uint8_t spritepixel = spriterow & (0x80 >> column);

                if (spritepixel == 0)
                    continue;

                uint8_t screen_x = (start_x + column) % CHIP8_COLUMNS;
                uint8_t screen_y = (start_y + row) % CHIP8_ROWS;

                if (chip8->display[screen_y][screen_x] == 1)
                    chip8->V[0xF] = 1;

                chip8->display[screen_y][screen_x] ^= 1;
            }
        }
    }
    break;
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0065:
            for (int i = 0; i <= x; i++)
            {
                chip8->V[i] = chip8->memory[chip8->I + i];
            }
            break;
        case 0x0055:
            for (int i = 0; i <= x; i++)
            {
                chip8->memory[chip8->I + i] = chip8->V[i];
            }
            break;
        case 0x0033:
            chip8->memory[chip8->I] = chip8->V[x]/100;
            chip8->memory[chip8->I + 1] = (chip8->V[x]/10) % 10;
            chip8->memory[chip8->I + 2] = chip8->V[x]%10;
            break;
        case 0x001E:
            chip8->I += chip8->V[x];
            break;
        }
    }
}