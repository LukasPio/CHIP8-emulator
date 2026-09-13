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

### Releases portáteis (sem instalação)

Os pacotes de release são para **x86_64 (64 bits)**. Extraia o pacote em uma
pasta do seu usuário. Não é necessário instalar SDL2, usar administrador ou
executar um instalador. As ROMs não fazem parte dos pacotes.

[Release v1.0.0 e checksums SHA-256](https://github.com/LukasPio/CHIP8-Emulator/releases/tag/v1.0.0).

**Windows 10/11:** extraia [chip8-1.0.0-windows-x86_64.zip](https://github.com/LukasPio/CHIP8-Emulator/releases/download/v1.0.0/chip8-1.0.0-windows-x86_64.zip) e execute no
PowerShell, dentro da pasta extraída:

```powershell
.\chip8.exe "C:\Jogos\minha rom.ch8"
```

O `chip8.exe` contém a SDL2 e o runtime do compilador; não precisa de DLLs
adicionais. O manifesto usa `asInvoker`, sem solicitação de elevação.

**Linux:** extraia [chip8-1.0.0-linux-x86_64.tar.gz](https://github.com/LukasPio/CHIP8-Emulator/releases/download/v1.0.0/chip8-1.0.0-linux-x86_64.tar.gz) e execute:

```sh
tar -xzf chip8-1.0.0-linux-x86_64.tar.gz
cd chip8-1.0.0-linux-x86_64
./chip8 "/home/usuario/Jogos/minha rom.ch8"
```

O pacote preserva a permissão de execução. Se o arquivo for copiado por um
programa que a remova, use `chmod +x chip8` (sem `sudo`). Requer glibc 2.35 ou
superior e uma sessão gráfica X11 ou Wayland, com os drivers de vídeo/áudio
usuais da distribuição. A SDL2 está incorporada. A base de compilação é
Ubuntu 22.04; Linux com musl (como Alpine) não é compatível com este pacote.

Use `--help` para consultar a sintaxe. Caminhos relativos são resolvidos a
partir da pasta atual do terminal. Pressione `-` para sair.

### Gerar os pacotes de release

Para manter a mesma base de compatibilidade Linux, compile pelo Docker:

```sh
docker build -f packaging/Dockerfile -t chip8-release-builder .
docker run --rm --user "$(id -u):$(id -g)" -v "$PWD:/work" chip8-release-builder
```

Os dois arquivos, pastas extraídas e `SHA256SUMS` são gerados em `dist/`.
As compilações de release usam otimização, sem sanitizers. O download da
SDL2 tem versão fixa e SHA-256 verificado; a licença acompanha os pacotes.

Para compilar diretamente em um host Linux x86_64, instale CMake, Ninja,
GNU Make, GCC, pkg-config, curl, zip e os headers de desenvolvimento SDL2.
Para Windows, instale também `gcc-mingw-w64-x86-64`. Execute `make release`
(ambos), `make release-linux` ou `make release-windows`. Um build Linux
feito em uma distribuição mais nova pode exigir uma glibc mais nova.

O workflow `.github/workflows/release.yml` gera os dois pacotes e testa os
executáveis em Linux e Windows. Uma execução manual disponibiliza os
artefatos; uma tag `v1.0.0` (correspondente à versão no CMake) também cria
uma GitHub Release com ambos os downloads, após os testes passarem.

### Requisitos para desenvolvimento

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
UndefinedBehaviorSanitizer. O diretório `build/` é criado automaticamente e ignorado pelo Git.

### Execução

Informe o caminho completo ou relativo de uma ROM `.ch8`:

```sh
./build/chip8 roms/test/3_corax_plus.ch8
```

Para executar as outras ROMs incluídas:

```sh
./build/chip8 roms/test/1_chip8_logo.ch8
./build/chip8 roms/test/2_ibm_logo.ch8
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
├── .github/workflows/release.yml
├── CMakeLists.txt
├── Makefile
├── build/
├── cmake/
├── dist/
├── packaging/
├── roms/
├── scripts/release.sh
├── tests/
└── src/
    ├── chip8.c
    ├── chip8.h
    └── main.c
```

### Fontes e referências

As seguintes fontes foram usadas durante o desenvolvimento:

- [How to write an emulator (CHIP-8 interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
  — guia usado para entender o caminho inicial da implementação;
- [Timendus CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite)
  — ROMs usadas para testar o emulador;
- [CHIP-8 — Wikipedia](https://en.wikipedia.org/wiki/CHIP-8) — especificações
  técnicas e referência dos opcodes;
- [JamesGriffin/CHIP-8-Emulator](https://github.com/JamesGriffin/CHIP-8-Emulator/)
  — projeto completo usado como referência e inspiração.

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

### Portable releases (no installation)

Release packages target **x86_64 (64-bit)**. Extract them into a folder you
own. No SDL2 installation, administrator account or installer is required.
ROMs are not included in the packages.

[Release v1.0.0 and SHA-256 checksums](https://github.com/LukasPio/CHIP8-Emulator/releases/tag/v1.0.0).

**Windows 10/11:** extract [chip8-1.0.0-windows-x86_64.zip](https://github.com/LukasPio/CHIP8-Emulator/releases/download/v1.0.0/chip8-1.0.0-windows-x86_64.zip), open PowerShell
in the extracted folder and run:

```powershell
.\chip8.exe "C:\Games\my rom.ch8"
```

SDL2 and the compiler runtime are linked into `chip8.exe`; no additional
DLLs are needed. Its `asInvoker` manifest does not request elevation.

**Linux:** extract [chip8-1.0.0-linux-x86_64.tar.gz](https://github.com/LukasPio/CHIP8-Emulator/releases/download/v1.0.0/chip8-1.0.0-linux-x86_64.tar.gz) and run:

```sh
tar -xzf chip8-1.0.0-linux-x86_64.tar.gz
cd chip8-1.0.0-linux-x86_64
./chip8 "/home/user/Games/my rom.ch8"
```

The archive preserves executable permissions. If another program strips
those permissions when copying the binary, run `chmod +x chip8` (no `sudo`).
Requires glibc 2.35 or newer, an X11 or Wayland desktop session, and the
distribution's usual video/audio drivers. SDL2 is embedded. The build uses
Ubuntu 22.04 as its baseline; musl distributions such as Alpine are not
compatible with this package.

Use `--help` for usage. Relative ROM paths are resolved from the terminal's
current directory. Press `-` to quit.

### Building release packages

Use Docker to preserve the Linux compatibility baseline:

```sh
docker build -f packaging/Dockerfile -t chip8-release-builder .
docker run --rm --user "$(id -u):$(id -g)" -v "$PWD:/work" chip8-release-builder
```

Both archives, extracted folders and `SHA256SUMS` are written to `dist/`.
Release builds are optimized, without sanitizers. The SDL2 source download
is version-pinned and SHA-256 verified; its license ships with the packages.

For direct builds on an x86_64 Linux host, install CMake, Ninja, GNU Make,
GCC, pkg-config, curl, zip and SDL2 development headers. Windows builds also
require `gcc-mingw-w64-x86-64`. Run `make release` (both platforms),
`make release-linux` or `make release-windows`. Linux builds made on a newer
distribution may require a newer glibc.

The `.github/workflows/release.yml` workflow packages both targets and tests
the executables on Linux and Windows. Manual runs upload build artifacts;
a `v1.0.0` tag matching the CMake version also creates a GitHub Release with
both downloads after tests pass.

### Development requirements

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
UndefinedBehaviorSanitizer. The `build/` directory is created automatically and ignored by Git.

### Running

Provide the full or relative path to a `.ch8` ROM:

```sh
./build/chip8 roms/test/3_corax_plus.ch8
```

To run the other included ROMs:

```sh
./build/chip8 roms/test/1_chip8_logo.ch8
./build/chip8 roms/test/2_ibm_logo.ch8
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
├── .github/workflows/release.yml
├── CMakeLists.txt
├── Makefile
├── build/
├── cmake/
├── dist/
├── packaging/
├── roms/
├── scripts/release.sh
├── tests/
└── src/
    ├── chip8.c
    ├── chip8.h
    └── main.c
```

### Sources and references

The following sources were used during development:

- [How to write an emulator (CHIP-8 interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
  — a guide used to understand how to get started with the implementation;
- [Timendus CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite)
  — test ROMs used to validate the emulator;
- [CHIP-8 — Wikipedia](https://en.wikipedia.org/wiki/CHIP-8) — technical
  specifications and opcode reference;
- [JamesGriffin/CHIP-8-Emulator](https://github.com/JamesGriffin/CHIP-8-Emulator/)
  — a complete project used as a reference and source of inspiration.
