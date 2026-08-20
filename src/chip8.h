#include <stdint.h>

#ifndef CHIP8
#define CHIP8

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_REGISTERS_AMOUNT 16
#define CHIP8_KEYS_AMOUNT 16
#define CHIP8_COLUMNS 64
#define CHIP8_ROWS 32
#define CHIP8_STACK_SIZE 16

typedef struct Chip8 
{
    uint16_t stack[CHIP8_STACK_SIZE];
    uint16_t *sp;

    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint8_t V[CHIP8_REGISTERS_AMOUNT];

    uint16_t I;
    uint16_t pc;

    uint8_t sound_timer;
    uint8_t delay_timer;

    uint8_t display[CHIP8_ROWS][CHIP8_COLUMNS];

    uint8_t keys[CHIP8_KEYS_AMOUNT];

    uint8_t drawFlag;
} Chip8;

void init_chip8(Chip8 *chip8);
void load_game(Chip8 *chip8, char *game);
void emulate_cycle(Chip8 *chip8);

#endif