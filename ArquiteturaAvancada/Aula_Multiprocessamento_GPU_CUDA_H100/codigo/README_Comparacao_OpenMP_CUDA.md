# Comparacao OpenMP x CUDA - Santos Dumont II / UFCI-RJ

## Arquitetura do no H100

O no H100 do SDumont II possui **2 CPUs Intel Xeon CPU Max 9468 (48 nucleos fisicos por soquete), totalizando 96 nucleos**, e 4 GPUs NVIDIA H100 80 GB. Os exemplos usam **1 GPU**.

Fontes: https://sdumont.lncc.br/machine.php e https://github.com/lncc-sered/manual-sdumont2nd/wiki/06-%E2%80%90--Gerenciador-de-filas

**Restricao fundamental:** `lncc-h100_dev` admite somente metade dos recursos do no por job. Por isso, com essa fila use ate **48 nucleos** e 1 H100. Para usar **96 nucleos**, utilize `lncc-h100` (alocacao exclusiva do no). A fila exclusiva bloqueia 4 GPUs; use-a somente se houver justificativa para reservar o no inteiro. A fila `lncc-h100` **nao aceita** `--gpus`, conforme manual.

## Programa

- `matriz_openmp.c`: exatamente a mesma inicializacao e os mesmos tres lacos de C=A*B usados em `matriz_h100.cu`. Paraleliza somente o laco das linhas com `#pragma omp parallel for schedule(static)`; cada thread escreve linhas distintas.
- Mede **apenas a multiplicacao**, sem alocacao, inicializacao, validacao ou aquecimento.
- Em cada configuracao usa aquecimento e 5 repeticoes. Reporta tempo minimo, medio, mediana, maximo, desvio-padrao, alem de conferencia de todas as celulas com referencia sequencial.
- N=4 serve para ilustracao, nao para calculo confiavel de speedup.
- `matriz_h100.cu` mantido intacto (mesma implementacao original CUDA). Seus tempos de GPU seguem como 1 medicao por tamanho: **nao** possuem a mesma robustez estatistica do experimento OpenMP.
- Mesmos valores de `float` em ambos os codigos e mesmas dimensoes: 4, 256, 512, 1024.

## Compilar localmente (Ubuntu)

```bash
make openmp
./matriz_openmp 4 4 5
./matriz_openmp 256 4 5
```

## Executar no SDumont II

Apos `git pull` na pasta do projeto:

```bash
cd ~/UFCIRJ/ArquiteturaAvancada/Aula_Multiprocessamento_GPU_CUDA_H100/codigo

# Recomendado para primeiro teste: 48 nucleos + 1 H100 (fila de desenvolvimento)
sbatch job_comparacao_h100_48_dev.sbatch

# SOMENTE se necessario avaliar os 96 nucleos fisicos:
# alocacao EXCLUSIVA do no H100 e suas 4 GPUs (embora o codigo use somente 1)
sbatch job_comparacao_h100_96.sbatch

squeue -u "$USER"
```

Se sua conta padrao nao estiver configurada, use `sbatch --account=cenapadrjsd ...`.

## Saidas de cada job

- `comparacao_h100_48_<jobid>.out` ou `comparacao_h100_96_<jobid>.out`: log geral.
- `resultados_openmp_<jobid>.csv`: tempos OpenMP por N e numero de threads (tempos em **ms**).
- `resultados_cuda_<jobid>.csv`: tempos CPU sequencial, kernel CUDA e GPU com transferencias por N (tempos em **ms**).
- `comparacao_openmp_cuda_<jobid>.csv`: speedups calculados e tempos OpenMP/CUDA lado a lado (se Python 3 disponivel).
- `openmp_<jobid>.txt` e `cuda_comparacao_<jobid>.txt`: logs individuais.

Para inspecionar:

```bash
ls -lt comparacao_h100_*.out
cat resultados_openmp_<jobid>.csv
cat resultados_cuda_<jobid>.csv
cat comparacao_openmp_cuda_<jobid>.csv
```

## Interpretacao

- `speedup_OpenMP(T) = tempo_OpenMP_1_thread / tempo_OpenMP_T_threads` (preferir medianas).
- `speedup_GPU_kernel_vs_OpenMP(T) = tempo_OpenMP_T_threads / tempo_GPU_kernel`.
- `speedup_GPU_total_vs_OpenMP(T) = tempo_OpenMP_T_threads / tempo_GPU_com_transferencias`.
- **Cuidado:** OpenMP computa apenas a multiplicacao, enquanto `GPU com transferencias` inclui copias mas exclui `cudaMalloc` e custos anteriores. Comparacao orientada para fins didaticos. Um estudo rigoroso pede muitos repeticoes de CUDA, aquecimento e diferentes otimizacoes.
- A implementacao e ingenua: nao usa BLAS, tiling, Tensor Cores nem uma estrategia refinada de memoria. O objetivo e comparar modelos de paralelismo.
- `OMP_PLACES=cores` e `OMP_PROC_BIND=spread` fixam as threads nos nucleos alocados. Com 96 threads sao usados dois soquetes e a politica de memoria/NUMA pode influenciar o desempenho.

## Recursos Slurm

Documentacao oficial (atualizada em setembro de 2026): https://github.com/lncc-sered/manual-sdumont2nd/wiki/06-%E2%80%90--Gerenciador-de-filas

O log do job permite verificar modelo de CPU, GPU e quantidade de CPUs reservadas. O agendador pode escolher um no H100 diferente de `sdumont2nd2019` em outra submissao, mas o **OpenMP e o CUDA do mesmo job** executam no mesmo no fisico.
