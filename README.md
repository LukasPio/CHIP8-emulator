# CHIP-8 Emulator

Emulador CHIP-8 em C, com vídeo, áudio e eventos gerenciados pela SDL2. O
programa carrega ROMs pela linha de comando e implementa o conjunto de
instruções, a tela, o teclado e os timers do CHIP-8 clássico.

> [!IMPORTANT]
> Este emulador foi feito exclusivamente para ROMs do CHIP-8 clássico. Não
> há suporte a extensões ou variantes como SUPER-CHIP (SCHIP), XO-CHIP e
> similares.

## Estado do projeto

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

## Sobre os commits e este README

Os commits deste repositório foram feitos com auxílio de IA. Por isso, as
mensagens provavelmente não são muito confiáveis como descrição exata das
mudanças.

> Eu gosto de programar, não de escrever commits ou READMEs.

## Requisitos

- GCC ou outro compilador C compatível com as opções do `Makefile`;
- GNU Make;
- arquivos de desenvolvimento da SDL2;
- suporte do compilador a AddressSanitizer e UndefinedBehaviorSanitizer.

No Debian e derivados:

```sh
sudo apt install build-essential libsdl2-dev
```

## Compilação

Na raiz do projeto, execute:

```sh
make compile
```

O executável é gerado em `build/chip8`. A compilação atual usa símbolos de
debug, desativa otimizações e habilita AddressSanitizer e
UndefinedBehaviorSanitizer.

> O diretório `build/` já faz parte do repositório. Se ele for removido, crie-o
> novamente antes de compilar com `mkdir -p build`.

## Execução

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

## Estrutura do projeto

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
