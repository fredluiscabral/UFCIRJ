# Persistência em Sistemas Operacionais

Duas aulas de três horas para estudantes do 2º período de Ciência da Computação.

**Esta é uma apresentação nova**: não inclui os slides anteriores de concorrência ou deadlocks.

## Estrutura

```text
SO/05_persistencia/
├── slides/
│   ├── apresentacao.tex
│   ├── apresentacao.pdf
│   ├── aula_1.pdf
│   └── aula_2.pdf
├── exemplos/
│   ├── 01_arquivo.c
│   ├── 02_lseek.c
│   ├── 03_metadados.c
│   ├── 04_fsync.c
│   └── Makefile
├── ROTEIRO.md
└── README.md
```

## Requisitos

- Ubuntu ou outro sistema compatível com POSIX.
- GCC e Make (Ubuntu: `sudo apt install build-essential`, se necessário).
- Leitor de PDF para apresentação.
- Os quatro programas são **independentes e curtos**, com execução finita. Isso permite verificar que arquivos continuam disponíveis mesmo depois que o processo termina.
- Não é necessário Java, OpenMP, MPI, acesso root nem bibliotecas extras.

## Compilar

A partir da raiz do repositório `UFCIRJ`:

```bash
cd SO/05_persistencia/exemplos
make
```

## Quatro demonstrações, nos slides correspondentes

### Slide 22 — Gravar e ler um arquivo

```bash
./01_arquivo gravar
./01_arquivo ler
./01_arquivo gravar
./01_arquivo ler
cat registro.txt
```

A segunda gravação adiciona uma linha, pois o programa utiliza `O_APPEND`. A execução `ler` é feita por um processo diferente do que realizou a gravação.

### Slide 26 — Posição no arquivo com `lseek()`

```bash
./02_lseek
cat posicoes.txt
wc -c posicoes.txt
```

O arquivo terá `ABCDxFGHIJ` (10 bytes). O `x` substitui a quinta letra.

### Slide 40 — Metadados e links

```bash
./01_arquivo gravar
./03_metadados registro.txt
ln -f registro.txt link_fisico.txt
ln -sfn registro.txt link_simbolico.txt
./03_metadados link_fisico.txt
./03_metadados link_simbolico.txt
ls -li registro.txt link_*.txt
```

Um hard link referencia o mesmo inode do original. O link simbólico tem inode próprio, mas `stat()` segue seu destino enquanto `lstat()` examina a própria entrada simbólica.

### Slide 49 — `fsync()`

```bash
./04_fsync
cat sincronizado.txt
./04_fsync
wc -l sincronizado.txt
```

`fsync()` solicita a sincronização dos dados e dos metadados relevantes de um arquivo aberto. **Esta demonstração não simula uma queda de energia nem prova resistência a qualquer tipo de falha de hardware.** A persistência de uma nova entrada de diretório pode requerer sincronização adicional do diretório.

## Comandos auxiliares usados nos slides

```bash
mkdir -p pasta_teste
cp registro.txt pasta_teste/copia.txt
pwd
ls -l pasta_teste
findmnt -T .
lsblk -f
df -h .
du -sh .
stat registro.txt
```

## Limpeza

```bash
make clean
```

`make clean` remove apenas os executáveis, não os arquivos `.txt` criados nas atividades. Se quiser apagar *explicitamente* os arquivos de demonstração e os links:

```bash
rm -f registro.txt posicoes.txt sincronizado.txt link_fisico.txt link_simbolico.txt
rm -rf pasta_teste
```

**Cuidado:** execute esses dois comandos somente no diretório `SO/05_persistencia/exemplos` e quando não precisar mais dos arquivos de teste.

## Compilar o LaTeX

A partir de `slides/`:

```bash
pdflatex -interaction=nonstopmode apresentacao.tex
pdflatex -interaction=nonstopmode apresentacao.tex
```

O arquivo utiliza Beamer, TikZ e Listings, disponíveis em instalações usuais de TeX Live. No Overleaf, selecione `apresentacao.tex` como arquivo principal e o compilador pdfLaTeX.

## Bibliografia

Arpaci-Dusseau, Remzi H.; Arpaci-Dusseau, Andrea C. *Operating Systems: Three Easy Pieces*, versão 1.10, 2023. Capítulos **36–45**, com ênfase em **36 (I/O Devices), 39 (Files and Directories), 40 (File System Implementation) e 42 (FSCK and Journaling)**.

Sumário e capítulos oficiais: https://pages.cs.wisc.edu/~remzi/OSTEP/
