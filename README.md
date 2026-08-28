# CHIP-8 Emulator

A CHIP-8 emulator written in C using SDL2 for graphics and input. The project
is under active development and is being expanded to support the complete
CHIP-8 instruction set, with the goal of running any CHIP-8 game ROM.

## Project status

This emulator is still in development. It currently runs the bundled Corax+
opcode test ROM and renders the CHIP-8 display in a fullscreen window. Support
for the remaining instructions and features is planned so that it can run any
CHIP-8 game ROM.

The current implementation includes:

- 4 KB of CHIP-8 memory, 16 general-purpose registers, a stack, timers, and a
  64 × 32 monochrome display
- The standard CHIP-8 font set loaded at address `0x50`
- ROM loading at the conventional `0x200` start address
- SDL2-based display rendering
- A subset of CHIP-8 instructions, including clear screen, jumps, conditional
  skips, register assignment/addition, index-register assignment, and sprite
  drawing

Keypad input, timer updates, sound, subroutines, and several instruction
families are not implemented yet. As a result, many CHIP-8 ROMs will not run
correctly.

## Requirements

- A C compiler such as GCC
- GNU Make
- SDL2 development files

On Debian or Ubuntu, the dependencies can be installed with:

```sh
sudo apt install build-essential libsdl2-dev
```

## Build and run

From the project root, run:

```sh
make compile
./build/chip8
```

The build enables AddressSanitizer and UndefinedBehaviorSanitizer and includes
debug symbols.

Press `Q` or close the window to quit.

## Selecting a ROM

The ROM is currently chosen at compile time in `src/main.c`:

```c
load_game(&chip8, "3-corax+");
```

`load_game` looks for `./roms/<name>.ch8`, so the executable must be launched
from the project root. To run another bundled ROM, replace `"3-corax+"` with
one of these names and rebuild:

- `1-chip8-logo`
- `2-ibm-logo`
- `3-corax+`

You can also place another `.ch8` file in `roms/` and use its filename without
the extension.

## Project layout

```text
.
├── Makefile       # Debug/sanitizer build command
├── roms/          # Bundled CHIP-8 ROMs
└── src/
    ├── chip8.c    # CPU initialization, ROM loading, and instruction execution
    ├── chip8.h    # CHIP-8 state and public interface
    └── main.c     # SDL2 window, render loop, and host input
```
