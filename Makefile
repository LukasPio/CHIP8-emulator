compile:
	@mkdir -p build
	gcc $(shell pkg-config --cflags sdl2) ./src/main.c -o ./build/game.o $(shell pkg-config --libs sdl2)