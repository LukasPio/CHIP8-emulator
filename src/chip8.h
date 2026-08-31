#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_REGISTERS_AMOUNT 16
#define CHIP8_KEYS_AMOUNT 16
#define CHIP8_COLUMNS 64
#define CHIP8_ROWS 32
#define CHIP8_STACK_SIZE 16
typedef struct chip8
{
    uint16_t stack[CHIP8_STACK_SIZE];
    uint16_t sp;
    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint8_t registers[CHIP8_REGISTERS_AMOUNT];
    uint16_t index_register;
    uint16_t pc;
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint8_t display[CHIP8_ROWS][CHIP8_COLUMNS];
    uint8_t keys[CHIP8_KEYS_AMOUNT];
    uint8_t draw_flag;
} chip8_t;
void init_chip8(chip8_t *chip8);
void load_game(chip8_t *chip8, const char *rom_path);
void emulate_cycle(chip8_t *chip8);
void decrease_timers(chip8_t *chip8);
#endif
