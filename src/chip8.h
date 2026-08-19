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
    unsigned short stack[CHIP8_STACK_SIZE];
    unsigned short sp;

    unsigned char memory[CHIP8_MEMORY_SIZE];
    unsigned char V[CHIP8_REGISTERS_AMOUNT];

    unsigned short I;
    unsigned short pc;
    unsigned short opcode;

    unsigned char sound_timer;
    unsigned char delay_timer;

    unsigned char key[CHIP8_KEYS_AMOUNT];
    unsigned char gfx[CHIP8_COLUMNS * CHIP8_ROWS];

    unsigned char drawFlag;
} Chip8;

void load(Chip8 *chip8);
void emulate_cycle();

#endif