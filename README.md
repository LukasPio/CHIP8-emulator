# CHIP-8 Emulator

## Idiomas / Languages

- [Português (Brasil)](#português-brasil)
- [English](#english)

## Português (Brasil)

Emulador CHIP-8 em C, com vídeo, áudio e eventos gerenciados pela SDL2. O
programa carrega ROMs pela linha de comando e implementa o conjunto de
instruções, a tela, o teclado e os timers do CHIP-8 clássico.

> [!IMPORTANT]
> Este emulador foi feito exclusivamente para ROMs do CHIP-8 clássico. Não
> há suporte a extensões ou variantes como SUPER-CHIP (SCHIP), XO-CHIP e
> similares.

### Estado do projeto

O projeto está **completo** dentro do escopo proposto: executar ROMs de CHIP-8
clássico. Não está planejada a implementação de variantes mais modernas.

A implementação possui:

- 4 KiB de memória, 16 registradores de 8 bits, registrador de índice, pilha,
  program counter e registradores de timer;
- fonte hexadecimal padrão carregada a partir do endereço `0x50`;
- carregamento de ROMs no endereço convencional `0x200`, com limite de
  3.584 bytes;
- tela monocromática de `64 × 32` pixels, desenhada em uma janela SDL2 em
  tela cheia;
- desenho de sprites com XOR, wrapping nas bordas e detecção de colisão;
- teclado hexadecimal mapeado para o teclado do computador;
- timers de delay e som atualizados a `60 Hz`, com sinal sonoro;
- CPU emulada a `700 Hz`;
- saltos, chamadas e retornos de sub-rotinas, comparações, operações
  lógicas, aritméticas e de memória.

Os opcodes implementados atualmente são:

```text
00E0  00EE  1nnn  2nnn  3xnn  4xnn  5xy0  6xnn  7xnn
8xy0  8xy1  8xy2  8xy3  8xy4  8xy5  8xy6  8xy7  8xyE
9xy0  Annn  Bnnn  Cxnn  Dxyn  Ex9E  ExA1  Fx07  Fx0A
Fx15  Fx18  Fx1E  Fx29  Fx33  Fx55  Fx65
```

### Sobre os commits e este README

Os commits deste repositório foram feitos com auxílio de IA. Por isso, as
mensagens provavelmente não são muito confiáveis como descrição exata das
mudanças.

> Eu gosto de programar, não de escrever commits ou READMEs.

### Requisitos

- GCC ou outro compilador C compatível com as opções do `Makefile`;
- GNU Make;
- arquivos de desenvolvimento da SDL2;
- suporte do compilador a AddressSanitizer e UndefinedBehaviorSanitizer.

No Debian e derivados:

```sh
sudo apt install build-essential libsdl2-dev
```

### Compilação

Na raiz do projeto, execute:

```sh
make compile
```

O executável é gerado em `build/chip8`. A compilação atual usa símbolos de
debug, desativa otimizações e habilita AddressSanitizer e
UndefinedBehaviorSanitizer.

> O diretório `build/` já faz parte do repositório. Se ele for removido, crie-o
> novamente antes de compilar com `mkdir -p build`.

### Execução

Informe o caminho completo ou relativo de uma ROM `.ch8`:

```sh
./build/chip8 roms/3_corax_plus.ch8
```

Para executar as outras ROMs incluídas:

```sh
./build/chip8 roms/1_chip8_logo.ch8
./build/chip8 roms/2_ibm_logo.ch8
```

Sem um argumento, o programa exibe `Usage: chip8 <rom_path>` e encerra. Para
sair durante a execução, pressione `-` ou feche a janela.

O teclado hexadecimal segue este mapeamento:

```text
CHIP-8       Teclado
1 2 3 C      1 2 3 4
4 5 6 D      Q W E R
7 8 9 E      A S D F
A 0 B F      Z X C V
```

### Estrutura do projeto

```text
.
├── Makefile
├── build/
├── roms/
└── src/
    ├── chip8.c
    ├── chip8.h
    └── main.c
```

## English

CHIP-8 emulator written in C, with video, audio, and event handling provided
by SDL2. The program loads ROMs from the command line and implements the
instruction set, display, keypad, and timers of the classic CHIP-8.

> [!IMPORTANT]
> This emulator was made exclusively for classic CHIP-8 ROMs. Extensions and
> variants such as SUPER-CHIP (SCHIP), XO-CHIP, and similar systems are not
> supported.

### Project status

The project is **complete** within its intended scope: running classic CHIP-8
ROMs. Support for more modern variants is not planned.

The implementation includes:

- 4 KiB of memory, sixteen 8-bit registers, an index register, a stack, a
  program counter, and timer registers;
- the standard hexadecimal font loaded at address `0x50`;
- ROM loading at the conventional address `0x200`, with a limit of 3,584
  bytes;
- a monochrome `64 × 32` display rendered in a fullscreen SDL2 window;
- XOR sprite drawing, edge wrapping, and collision detection;
- a hexadecimal keypad mapped to the computer keyboard;
- delay and sound timers updated at `60 Hz`, including an audible beep;
- CPU emulation at `700 Hz`;
- jumps, subroutine calls and returns, comparisons, logical and arithmetic
  operations, and memory operations.

The currently implemented opcodes are:

```text
00E0  00EE  1nnn  2nnn  3xnn  4xnn  5xy0  6xnn  7xnn
8xy0  8xy1  8xy2  8xy3  8xy4  8xy5  8xy6  8xy7  8xyE
9xy0  Annn  Bnnn  Cxnn  Dxyn  Ex9E  ExA1  Fx07  Fx0A
Fx15  Fx18  Fx1E  Fx29  Fx33  Fx55  Fx65
```

### About the commits and this README

The commits in this repository were made with the help of AI. Therefore,
their messages are probably not very reliable as exact descriptions of the
changes.

> I like programming, not writing commits or READMEs.

### Requirements

- GCC or another C compiler compatible with the options in the `Makefile`;
- GNU Make;
- SDL2 development files;
- compiler support for AddressSanitizer and UndefinedBehaviorSanitizer.

On Debian and derivatives:

```sh
sudo apt install build-essential libsdl2-dev
```

### Building

From the project root, run:

```sh
make compile
```

The executable is generated at `build/chip8`. The current build includes
debug symbols, disables optimizations, and enables AddressSanitizer and
UndefinedBehaviorSanitizer.

> The `build/` directory is already part of the repository. If it is removed,
> recreate it before compiling with `mkdir -p build`.

### Running

Provide the full or relative path to a `.ch8` ROM:

```sh
./build/chip8 roms/3_corax_plus.ch8
```

To run the other included ROMs:

```sh
./build/chip8 roms/1_chip8_logo.ch8
./build/chip8 roms/2_ibm_logo.ch8
```

Without an argument, the program prints `Usage: chip8 <rom_path>` and exits.
To quit while it is running, press `-` or close the window.

The hexadecimal keypad uses the following mapping:

```text
CHIP-8       Keyboard
1 2 3 C      1 2 3 4
4 5 6 D      Q W E R
7 8 9 E      A S D F
A 0 B F      Z X C V
```

### Project structure

```text
.
├── Makefile
├── build/
├── roms/
└── src/
    ├── chip8.c
    ├── chip8.h
    └── main.c
```
