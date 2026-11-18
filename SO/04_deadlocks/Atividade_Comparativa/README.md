# Atividade investigativa II — Deadlocks em Linux, Windows e macOS

**Esta é uma segunda atividade, independente da atividade anterior** em
`SO/04_deadlocks/Atividade/`. Nenhum arquivo da primeira atividade precisa ser
substituído ou movido. Comece lendo `atividade_comparativa.pdf`.

## Organização

- `Linux/`: duas experiências (threads com `flock` e processos com `fcntl`).
- `MacOS/`: duas experiências (threads com `flock` e processos com `fcntl`).
- `Windows/`: experiência nativa Win32 com `LockFileEx` e duas threads.
- `atividade_comparativa.tex` e `atividade_comparativa.pdf`: roteiro independente.

Cada subpasta possui os seus próprios arquivos `A.txt` e `B.txt`; execute os
programas com terminal aberto **dentro da pasta correspondente**. Não rode
vários experimentos da mesma pasta ao mesmo tempo. Use disco local.

## Linux (GCC e GNU make)

```sh
cd Linux
make
make teste_flock
make teste_fcntl
```

O comando `timeout` dos alvos do Makefile limita o período de observação;
não é uma detecção de deadlock realizada pelo kernel. Se restarem processos
em execução após a interrupção, encerre-os antes de repetir os testes.

## macOS (Xcode Command Line Tools)

```sh
cd MacOS
make
./01_flock_threads
./02_fcntl_processos
```

Execute um experimento de cada vez. Use `Ctrl+C` se necessário. Os processos
criados por `fork()` pertencem ao grupo de processos do terminal.

## Windows nativo (não WSL)

No CMD com MinGW-w64 disponível:

```bat
cd Windows
compilar_mingw.bat
01_lockfileex_threads.exe
```

Alternativamente, no Developer Command Prompt for VS:

```bat
cd Windows
compilar_msvc.bat
01_lockfileex_threads.exe
```

Em um shell MinGW com GNU Make, também é possível usar `make` e `make demo`.
Para interromper, `Ctrl+C`.

## Entrega e registro

O resultado da investigação deve constar de **um segundo relatório**, separado
do relatório da atividade Linux original. Responda às questões do PDF.
Se não houver acesso real a um sistema, diferencie pesquisa documental de
observações efetivamente executadas. Não invente saídas.

## Avisos

- `open(..., O_RDWR)` e `CreateFileA(...)` dão acesso, mas não fazem por si só
  o bloqueio solicitado depois no código.
- Os programas não escrevem nos arquivos de teste, apenas abrem e bloqueiam.
- O comportamento depende da API e pode variar conforme versão, contexto e
  sistema de arquivos. Registre esse ambiente.
