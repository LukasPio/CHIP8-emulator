compile:
	gcc -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer -I/usr/include/SDL2 -D_REENTRANT src/*.c -o build/chip8 -lSDL2
