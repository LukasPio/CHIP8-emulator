.PHONY: compile release release-linux release-windows

compile:
	mkdir -p build
	$(CC) -std=c11 -Wall -Wextra -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer $$(pkg-config --cflags sdl2) src/*.c -o build/chip8 $$(pkg-config --libs sdl2)

release:
	bash scripts/release.sh all

release-linux:
	bash scripts/release.sh linux

release-windows:
	bash scripts/release.sh windows
