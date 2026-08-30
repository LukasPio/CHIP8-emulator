# CHIP-8 Emulator

Emulador CHIP-8 em C, com vídeo e eventos gerenciados pela SDL2. O projeto
ainda está em desenvolvimento: ele já carrega ROMs pela linha de comando,
executa parte do conjunto de instruções e renderiza a tela clássica de
`64 × 32` pixels, mas ainda não oferece compatibilidade completa com jogos.

## Estado atual

A implementação possui:

- 4 KiB de memória, 16 registradores de 8 bits, registrador de índice, pilha,
  program counter e registradores de timer;
- fonte hexadecimal padrão carregada a partir do endereço `0x50`;
- carregamento de ROMs no endereço convencional `0x200`, com limite de
  3.584 bytes;
- tela monocromática de `64 × 32` pixels, desenhada em uma janela SDL2 em
  tela cheia;
- desenho de sprites com XOR, wrapping nas bordas e detecção de colisão;
- saltos, chamadas e retornos de sub-rotinas, comparações, operações lógicas
  e aritméticas, além de algumas operações de memória.

Os opcodes implementados atualmente são:

```text
00E0  00EE  1nnn  2nnn  3xnn  4xnn  5xy0  6xnn  7xnn
8xy0  8xy1  8xy2  8xy3  8xy4  8xy5  8xy6  8xy7  8xyE
9xy0  Annn  Dxyn  Fx1E  Fx33  Fx55  Fx65
```

### Limitações conhecidas

- O teclado hexadecimal do CHIP-8 ainda não está conectado ao teclado do
  computador; apenas `Q` e o botão de fechar a janela encerram o emulador.
- Os timers de delay e som fazem parte do estado da máquina, mas ainda não
  são atualizados, e não há saída de áudio.
- Vários opcodes ainda não estão implementados. Em particular, as instruções
  aritméticas e de deslocamento da família `8xy_` ainda não atualizam `VF`
  para carry, borrow ou bit deslocado.
- A emulação executa um opcode por quadro renderizado, sem uma frequência de
  CPU configurável e sem temporização independente a 60 Hz.
- Não há tratamento explícito para opcodes desconhecidos nem proteções contra
  underflow/overflow da pilha durante a execução.

Por essas limitações, as ROMs incluídas são úteis para acompanhar o progresso,
mas jogos e testes de compatibilidade podem não funcionar corretamente.

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
./build/chip8 roms/3-corax+.ch8
```

Para executar as outras ROMs incluídas:

```sh
./build/chip8 roms/1-chip8-logo.ch8
./build/chip8 roms/2-ibm-logo.ch8
```

Sem um argumento, o programa exibe `Usage: chip8 <rom-path>` e encerra. Para
sair durante a execução, pressione `Q` ou feche a janela.

## Estrutura do projeto

```text
.
├── Makefile       # Receita de compilação com debug e sanitizers
├── build/         # Executável gerado
├── roms/          # ROMs CHIP-8 incluídas para teste
└── src/
    ├── chip8.c    # Inicialização, carregamento da ROM e execução de opcodes
    ├── chip8.h    # Estado da máquina e interface pública
    └── main.c     # Inicialização da SDL2, loop principal, eventos e renderização
```
