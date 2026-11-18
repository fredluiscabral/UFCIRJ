# Atividade investigativa - bloqueio de arquivos no Linux

**Leiam primeiro `atividade.pdf`.** Não há resultados ou respostas antecipadas neste README.

## Arquivos

- `01_flock_threads.c`: experimento com duas threads e bloqueios via `flock()`.
- `02_fcntl_processos.c`: experimento com dois processos independentes e bloqueios via `fcntl()`.
- `A.txt` e `B.txt`: arquivos utilizados nos dois testes.
- `Makefile`: compilação e execução controlada.
- `atividade.tex` / `atividade.pdf`: roteiro da atividade e perguntas.

## Requisitos

Linux, GCC, make e o comando `timeout` (GNU coreutils).

## Execução

```bash
make
make teste_flock
make teste_fcntl
```

A primeira execução inicia duas threads. A segunda inicia dois processos com ordens opostas de solicitação de bloqueio. Em ambos os casos há uma pausa de dez segundos entre os pedidos.

`timeout` é um limite de observação imposto externamente pelo terminal; o encerramento por limite de tempo **não é uma resposta do kernel a deadlock**. É possível interromper manualmente com `Ctrl+C`.

O conteúdo dos arquivos de texto não deve ser alterado. Os programas usam `O_RDWR` e solicitam bloqueios explícitos, mas não escrevem dados nos arquivos.

## Leitura das APIs

Para estudar as funções usadas nos códigos, consultem `man 2 open`, `man 2 flock`, `man 2 fcntl` e `man 2 fcntl_locking` (quando disponíveis no Linux).
