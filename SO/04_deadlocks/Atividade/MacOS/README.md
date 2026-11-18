# macOS — experimento com arquivos e bloqueios

Este material foi preparado para comparacao com o experimento Linux.

## Requisitos
- macOS com Xcode Command Line Tools (`xcode-select --install`, caso necessario).
- Terminal aberto dentro desta pasta.
- Execute preferencialmente em disco local, nao em compartilhamentos de rede.

## Compilar

```sh
make
```

## Experimento 1: duas threads, flock()

```sh
./01_flock_threads
```

Cada thread abre os arquivos separadamente em ordem inversa, solicita bloqueios
exclusivos e espera dez segundos apos adquirir o primeiro.

## Experimento 2: dois processos, fcntl()

```sh
./02_fcntl_processos
```

O programa principal usa `fork()` para criar dois processos independentes.
Cada processo abre os arquivos com `O_RDWR` e solicita um bloqueio de escrita
`F_WRLCK` em todo o arquivo com `F_SETLKW`.

## Investigar

- A segunda solicitacao de bloqueio espera, falha ou e concedida?
- Em caso de erro, qual mensagem aparece? Pesquise sua causa.
- Onde estao presentes as condicoes de Coffman?
- Compare os resultados com os experimentos Linux.
- Por que usamos processos separados em `fcntl()` tradicional?

Se o programa nao terminar, interrompa com Ctrl+C. Nao confunda a interrupcao
manual com uma decisao do kernel. Nao execute simultaneamente os dois testes
no mesmo diretorio. Os exemplos nao modificam os arquivos A.txt e B.txt.

As interfaces de bloqueio podem ter particularidades de versao e sistema
de arquivos; documente a versao do macOS e o local onde executou o teste.

Referencias: `man 2 flock`, `man 2 fcntl`, `man 2 open`.
