#ifndef CHIP8
#define CHIP8

#define CHIP8_MEMORY 4096
#define CHIP8_REGISTERS 16
#define CHIP8_COLUMNS 64
#define CHIP8_ROWS 32
#define CHIP8_STACK 24

typedef struct Chip8 
{
    unsigned short stack[CHIP8_STACK];
    unsigned char memory[CHIP8_MEMORY];
    unsigned short opcode;
    unsigned char V[CHIP8_REGISTERS]; // Registers V0-VF
    unsigned char key[16];
    unsigned short I; // Index register
    unsigned short pc; // Program counter
    unsigned char gfx[CHIP8_COLUMNS * CHIP8_ROWS]; // Graphics buffer
    unsigned char delay_timer;
    unsigned char sound_timer;
} Chip8;

void init_chip8(Chip8 *cpu);
void load_game(Chip8 *cpu);

#endif