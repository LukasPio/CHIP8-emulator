#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "chip8.h"
#define FONT_START 0x50
static const uint8_t font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80};
void init_chip8(chip8_t *chip8)
{
    memset(chip8, 0, sizeof(*chip8));
    memcpy(&chip8->memory[FONT_START], font, sizeof(font));
    chip8->pc = 0x200;
    srand(time(NULL));
}
void load_game(chip8_t *chip8, const char *rom_path)
{
    FILE *file = fopen(rom_path, "rb");
    if (file == NULL)
    {
        printf("ROM was not found on %s\n", rom_path);
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
    if ((unsigned long)game_size > sizeof(chip8->memory) - 0x200)
    {
        fprintf(stderr, "ROM is too large\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    size_t bytes_read = fread(&chip8->memory[0x200], 1, game_size, file);
    if (bytes_read != (size_t)game_size)
    {
        fprintf(stderr, "Failed to read complete ROM\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}
void emulate_cycle(chip8_t *chip8)
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
        if (chip8->registers[x] == nn)
            chip8->pc += 2;
        break;
    case 0x4000:
        if (chip8->registers[x] != nn)
            chip8->pc += 2;
        break;
    case 0x5000:
        if (chip8->registers[x] == chip8->registers[y])
            chip8->pc += 2;
        break;
    case 0x6000:
        chip8->registers[x] = nn;
        break;
    case 0x7000:
        chip8->registers[x] += nn;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            chip8->registers[x] = chip8->registers[y];
            break;
        case 0x0001:
            chip8->registers[x] |= chip8->registers[y];
            break;
        case 0x0002:
            chip8->registers[x] &= chip8->registers[y];
            break;
        case 0x0003:
            chip8->registers[x] ^= chip8->registers[y];
            break;
        case 0x0004:
        {
            uint16_t sum = (uint16_t)chip8->registers[x] + (uint16_t)chip8->registers[y];
            chip8->registers[x] = sum;
            chip8->registers[0xF] = (sum > UINT8_MAX);
            break;
        }
        case 0x0005:
        {
            uint8_t result = (uint8_t)(chip8->registers[x] - chip8->registers[y]);
            uint8_t no_borrow = chip8->registers[x] >= chip8->registers[y];
            chip8->registers[x] = result;
            chip8->registers[0xF] = no_borrow;
        }
        break;
        case 0x0006:
        {
            uint8_t lsb = chip8->registers[x] & 0x01;
            chip8->registers[x] >>= 1;
            chip8->registers[0xF] = lsb;
            break;
        }
        case 0x0007:
        {
            uint8_t result = (uint8_t)(chip8->registers[y] - chip8->registers[x]);
            uint8_t no_borrow = chip8->registers[y] >= chip8->registers[x];
            chip8->registers[x] = result;
            chip8->registers[0xF] = no_borrow;
            break;
        }
        case 0x000E:
        {
            uint8_t msb = ((chip8->registers[x] >> 7) & 0x01);
            chip8->registers[x] <<= 1;
            chip8->registers[0xF] = msb;
            break;
        }
        }
        break;
    case 0x9000:
        if (chip8->registers[x] != chip8->registers[y])
            chip8->pc += 2;
        break;
    case 0xA000:
        chip8->index_register = nnn;
        break;
    case 0xB000:
        chip8->pc = chip8->registers[0] + nnn;
        break;
    case 0xC000:
    {
        uint8_t random = rand() % 256;
        chip8->registers[x] = random & nn;
        break;
    }
    case 0xD000:
    {
        uint8_t start_x = chip8->registers[x];
        uint8_t start_y = chip8->registers[y];
        chip8->registers[0xF] = 0;
        for (uint8_t row = 0; row < n; row++)
        {
            uint8_t sprite_row = chip8->memory[chip8->index_register + row];
            for (uint8_t column = 0; column < 8; column++)
            {
                uint8_t sprite_pixel = sprite_row & (0x80 >> column);
                if (sprite_pixel == 0)
                    continue;
                uint8_t screen_x = (start_x + column) % CHIP8_COLUMNS;
                uint8_t screen_y = (start_y + row) % CHIP8_ROWS;
                if (chip8->display[screen_y][screen_x] == 1)
                    chip8->registers[0xF] = 1;
                chip8->display[screen_y][screen_x] ^= 1;
            }
        }
    }
    break;
    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E:
        {
            uint8_t key_index = chip8->registers[x] & 0x0F;
            if (chip8->keys[key_index])
                chip8->pc += 2;
            break;
        }
        case 0x00A1:
        {
            uint8_t key_index = chip8->registers[x] & 0x0F;
            if (!chip8->keys[key_index])
                chip8->pc += 2;
            break;
        }
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0065:
            for (int i = 0; i <= x; i++)
            {
                chip8->registers[i] = chip8->memory[chip8->index_register + i];
            }
            break;
        case 0x0055:
            for (int i = 0; i <= x; i++)
            {
                chip8->memory[chip8->index_register + i] = chip8->registers[i];
            }
            break;
        case 0x0033:
            chip8->memory[chip8->index_register] = chip8->registers[x] / 100;
            chip8->memory[chip8->index_register + 1] = (chip8->registers[x] / 10) % 10;
            chip8->memory[chip8->index_register + 2] = chip8->registers[x] % 10;
            break;
        case 0x001E:
            chip8->index_register += chip8->registers[x];
            break;
        case 0x0029:
            chip8->index_register = FONT_START + (chip8->registers[x] & 0x0F) * 5;
            break;
        case 0x0018:
            chip8->sound_timer = chip8->registers[x];
            break;
        case 0x0015:
            chip8->delay_timer = chip8->registers[x];
            break;
        case 0x000A:
        {
            int key_pressed = 0;
            for (int i = 0; i < CHIP8_KEYS_AMOUNT; i++)
            {
                if (chip8->keys[i])
                {
                    chip8->registers[x] = i;
                    key_pressed = 1;
                    break;
                }
            }
            if (!key_pressed)
                chip8->pc -= 2;
            break;
        }
        case 0x0007:
            chip8->registers[x] = chip8->delay_timer;
            break;
        }
    }
}
void decrease_timers(chip8_t *chip8)
{
    if (chip8->delay_timer > 0)
        chip8->delay_timer--;
    if (chip8->sound_timer > 0)
        chip8->sound_timer--;
}
