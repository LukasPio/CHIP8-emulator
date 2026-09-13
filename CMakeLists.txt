cmake_minimum_required(VERSION 3.18)
project(chip8 VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compile SDL into the executable; desktop/audio drivers are loaded at runtime.
include(FetchContent)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
set(CHIP8_SDL_ARCHIVE "https://www.libsdl.org/release/SDL2-2.32.10.tar.gz"
    CACHE STRING "SDL source archive (URL or local path)")
FetchContent_Declare(sdl2
    URL "${CHIP8_SDL_ARCHIVE}"
    URL_HASH SHA256=5f5993c530f084535c65a6879e9b26ad441169b3e25d789d83287040a9ca5165)
FetchContent_MakeAvailable(sdl2)

add_executable(chip8 src/main.c src/chip8.c)
target_link_libraries(chip8 PRIVATE SDL2::SDL2-static)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(chip8 PRIVATE -Wall -Wextra)
endif()
if(WIN32)
    enable_language(RC)
    target_sources(chip8 PRIVATE packaging/windows.rc)
    target_include_directories(chip8 PRIVATE packaging)
    if(MINGW)
        # Console application; no SDL DLL or MinGW runtime DLL to install.
        target_link_options(chip8 PRIVATE -static -static-libgcc -mconsole)
    endif()
endif()

install(TARGETS chip8 RUNTIME DESTINATION . COMPONENT Runtime)
install(FILES README.md DESTINATION . COMPONENT Runtime)
install(FILES "${sdl2_SOURCE_DIR}/LICENSE.txt"
    DESTINATION . RENAME SDL2-LICENSE.txt COMPONENT Runtime)

include(CTest)
if(BUILD_TESTING AND NOT CMAKE_CROSSCOMPILING)
    add_test(NAME cli-help COMMAND chip8 --help)
    set_tests_properties(cli-help PROPERTIES PASS_REGULAR_EXPRESSION "Usage: chip8 <rom_path>")
    add_test(NAME cli-missing-argument COMMAND chip8)
    set_tests_properties(cli-missing-argument PROPERTIES WILL_FAIL TRUE)
endif()
