# Arquitetura Avancada: um executavel para cada variante.
CC       = gcc
MPICC    = mpicc
CFLAGS   = -O2 -std=c11 -Wall -Wextra -Wpedantic
# Preserva a ordem de percurso e reduz interferencia da vetorizacao no experimento 1.
LOCALIDADE = -fno-loop-interchange -fno-tree-vectorize

PROGRAMAS = 01_soma_linhas 02_soma_colunas 03_multiplicacao_simples \
            04_multiplicacao_tiling 05_pi_sequencial 06_pi_openmp \
            07_vetor_openmp_amdahl

.PHONY: all mpi tudo clean ajuda help
all: $(PROGRAMAS)

01_soma_linhas: 01_soma_linhas.c
	$(CC) $(CFLAGS) $(LOCALIDADE) $< -o $@

02_soma_colunas: 02_soma_colunas.c
	$(CC) $(CFLAGS) $(LOCALIDADE) $< -o $@

03_multiplicacao_simples: 03_multiplicacao_simples.c
	$(CC) $(CFLAGS) $(LOCALIDADE) $< -o $@

04_multiplicacao_tiling: 04_multiplicacao_tiling.c
	$(CC) $(CFLAGS) $(LOCALIDADE) $< -o $@

05_pi_sequencial: 05_pi_sequencial.c
	$(CC) $(CFLAGS) $< -o $@

06_pi_openmp: 06_pi_openmp.c
	$(CC) $(CFLAGS) -fopenmp $< -o $@

07_vetor_openmp_amdahl: 07_vetor_openmp_amdahl.c
	$(CC) $(CFLAGS) -fopenmp $< -o $@

08_vetor_mpi: 08_vetor_mpi.c
	$(MPICC) $(CFLAGS) $< -o $@

mpi: 08_vetor_mpi
tudo: all mpi
clean:
	rm -f $(PROGRAMAS) 08_vetor_mpi
ajuda help:
	@echo 'make      compila 7 programas (sem exigir MPI)'
	@echo 'make mpi  compila MPI (precisa de mpicc)'
	@echo 'make tudo compila os 8 programas'
	@echo 'make clean remove executaveis'
