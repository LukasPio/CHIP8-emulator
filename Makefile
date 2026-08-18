compile:
	gcc $(shell pkg-config --cflags sdl2) main.c -o game.o $(shell pkg-config --libs sdl2)
