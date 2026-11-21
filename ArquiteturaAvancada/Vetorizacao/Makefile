# Laboratorio de vetorizacao para Linux x86-64 (GCC)
CC       := gcc
BASICO   := -std=c11 -O3 -Wall -Wextra -Wpedantic -fno-unroll-loops -ffp-contract=off -fno-lto
# Perfil base: SSE habilitado, AVX desabilitado para observar registradores XMM.
ARQ_BASE := -msse2 -mno-avx
# Manter o mesmo -O3; mudar apenas a vetorizacao.
ESCALAR  := $(BASICO) $(ARQ_BASE) -fno-tree-vectorize -fno-tree-slp-vectorize
VETOR    := $(BASICO) $(ARQ_BASE) -ftree-vectorize -ftree-slp-vectorize
# Perfil EXTRA: AVX2 requer CPU com suporte a AVX2 (verifique antes de executar).
AVX2     := $(BASICO) -mavx2 -ftree-vectorize -ftree-slp-vectorize

# Exemplo: make clean all TAMANHO=2000000 REPETICOES=100
TAMANHO   ?= 1000000
REPETICOES ?= 80
DEFINICOES := -DTAMANHO=$(TAMANHO) -DREPETICOES=$(REPETICOES)

.PHONY: all executar medir asm relatorio avx2 executar-avx2 clean ajuda

all: bin/escalar bin/vetor

bin assembly relatorios:
	mkdir -p $@

bin/escalar: codigo/funcoes.c codigo/principal.c | bin
	$(CC) $(ESCALAR) $(DEFINICOES) $^ -o $@

bin/vetor: codigo/funcoes.c codigo/principal.c | bin
	$(CC) $(VETOR) $(DEFINICOES) $^ -o $@

bin/vetor_avx2: codigo/funcoes.c codigo/principal.c | bin
	$(CC) $(AVX2) $(DEFINICOES) $^ -o $@

executar: all
	@echo '=== SEM VETORIZACAO ==='
	@./bin/escalar
	@echo '=== COM VETORIZACAO ==='
	@./bin/vetor

medir: all | relatorios
	@echo 'Resultados de tres execucoes para cada configuracao:' | tee relatorios/tempos.txt
	@for v in escalar vetor; do \
	  echo "=== $$v ===" | tee -a relatorios/tempos.txt; \
	  for k in 1 2 3; do ./bin/$$v | tee -a relatorios/tempos.txt; done; \
	done

asm: | assembly
	$(CC) $(ESCALAR) -S -masm=intel codigo/funcoes.c -o assembly/funcoes_escalar.s
	$(CC) $(VETOR)   -S -masm=intel codigo/funcoes.c -o assembly/funcoes_vetor.s
	@echo 'Assembly gerado em assembly/funcoes_{escalar,vetor}.s'

relatorio: | relatorios
	$(CC) $(VETOR) -c codigo/funcoes.c -o /dev/null -fopt-info-vec-all=relatorios/vetorizacao.txt
	@echo 'Relatorio gerado em relatorios/vetorizacao.txt'

avx2: bin/vetor_avx2 | assembly
	$(CC) $(AVX2) -S -masm=intel codigo/funcoes.c -o assembly/funcoes_avx2.s
	@echo 'AVX2 preparado. Execute somente se seu processador suportar AVX2.'

executar-avx2: avx2
	@grep -qm1 avx2 /proc/cpuinfo || { echo 'ERRO: CPU sem AVX2. Nao executar este binario.'; exit 1; }
	@./bin/vetor_avx2

ajuda:
	@echo 'make all           : compila escalar e vetor'
	@echo 'make executar      : executa uma vez cada versao'
	@echo 'make medir         : tres medidas por versao, salva relatorios/tempos.txt'
	@echo 'make asm           : Assembly das duas versoes'
	@echo 'make relatorio     : relatorio do GCC sobre vetorizacao'
	@echo 'make avx2          : compila versao extra AVX2 (requer CPU AVX2 para executar)'
	@echo 'make executar-avx2 : testa e executa AVX2 se a CPU permitir'
	@echo 'make clean         : limpa arquivos gerados'

clean:
	rm -rf bin assembly relatorios
