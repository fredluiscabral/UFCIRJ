# Arquitetura Avançada: códigos simplificados para experimentos

**Objetivo:** entregar programas curtos para que a turma execute, meça tempos e formule hipóteses *antes* de receber a explicação arquitetural.

Há **8 arquivos C**, cada um com **uma única versão do algoritmo**. O experimento de *tiling* usa multiplicação de matrizes, porque a soma simples de matrizes normalmente não apresenta ganho consistente com blocagem. A versão antiga que misturava quatro variações de multiplicação foi abandonada.

## Como compilar no Ubuntu

```bash
make                 # sete programas C e OpenMP
make mpi             # apenas o MPI, se mpicc estiver instalado
# opcional: sudo apt install build-essential openmpi-bin libopenmpi-dev
```

## Primeiros comandos para verificar o funcionamento

```bash
# Experimento 1: MESMA soma, ordem dos lacos diferente
./01_soma_linhas 2048
./02_soma_colunas 2048

# Experimento 2: MESMA multiplicacao, sem e com blocos
./03_multiplicacao_simples 768
./04_multiplicacao_tiling 768 32

# Experimento 3: MESMO calculo de pi, sequencial e OpenMP
./05_pi_sequencial 100000000
OMP_NUM_THREADS=4 ./06_pi_openmp 100000000

# Experimento 4: MESMA soma de vetores, diferentes numeros de threads
OMP_NUM_THREADS=1 ./07_vetor_openmp_amdahl 16000000 20 10
OMP_NUM_THREADS=4 ./07_vetor_openmp_amdahl 16000000 20 10

# Experimento 5: MESMA soma de vetores com processos MPI
mpirun -np 1 ./08_vetor_mpi 8000000
mpirun -np 4 ./08_vetor_mpi 8000000
```

## Cuidados para a interpretação

- Tempos variam conforme CPU, RAM, compilador, aquecimento, frequencia, processos em segundo plano e numero de nucleos disponiveis. Repita cada comando ao menos tres vezes e compare a mediana.
- Nos experimentos 1 e 2, ambos os programas recebem os mesmos dados e usam as mesmas flags de compilacao. `-fno-loop-interchange` impede que o compilador desfaça a mudanca proposital de ordem no experimento de localidade. `-fno-tree-vectorize` reduz interferencia da vetorizacao.
- Os experimentos de multiplicacao usam a mesma ordem **i,j,k** no calculo interno. O que muda sao os blocos. O tamanho do bloco ideal depende da maquina e do problema; *tiling* nao garante ganho em todo caso.
- `07_vetor_openmp_amdahl.c` deixa **10% dos elementos** no trecho sequencial por padrao, e aceita mudar esse percentual como terceiro argumento. Isso **nao quer dizer 10% do tempo sequencial** exigido pela formula da Lei de Amdahl. A parte de memoria e o overhead das threads podem limitar o ganho.
- MPI mede **distribuicao + calculo + recolhimento**. E normal a operacao simples perder desempenho em varios processos. Neste exemplo didatico, `N` precisa ser divisivel pelo numero de processos.
- A alocacao e a inicializacao dos dados ficam fora das medidas (MPI inclui a comunicacao). Os resultados numericos aparecem para permitir conferir se as versoes fazem o mesmo calculo. Pequenas diferencas na ultima casa de PI podem decorrer da ordem de somas em ponto flutuante.

## Organizacao

| Arquivo | Objetivo de observacao |
| --- | --- |
| `01_soma_linhas.c` | Matriz percorrida por linhas, i externo / j interno |
| `02_soma_colunas.c` | Mesma operacao, j externo / i interno |
| `03_multiplicacao_simples.c` | Produto de matrizes sem blocos |
| `04_multiplicacao_tiling.c` | Produto de matrizes com blocos, mantendo i,j,k internos |
| `05_pi_sequencial.c` | Aproximacao de pi pela regra do ponto medio |
| `06_pi_openmp.c` | Mesma aproximacao de pi usando reduction OpenMP |
| `07_vetor_openmp_amdahl.c` | Trecho serial + trecho paralelo: limites e gargalos reais |
| `08_vetor_mpi.c` | Scatter, soma local, Gather e custo da comunicacao |

**Etapa seguinte:** elaborar o roteiro investigativo para alunos (tabelas de medicao, hipoteses, perguntas e discussao em aula). Este README e apenas a referencia tecnica minima para executar os programas.
