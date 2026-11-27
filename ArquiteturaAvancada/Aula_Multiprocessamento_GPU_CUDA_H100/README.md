# Arquitetura Avançada de Computadores - Multiprocessamento e GPUs

**Instituição:** UFCI/RJ - Universidade Federal de Ciência e Inovação do Rio de Janeiro  
**Demonstração:** multiplicação de matrizes quadradas em C e CUDA, com execução prevista para uma NVIDIA H100 do Santos Dumont II (LNCC).

## Organização da pasta

```text
Aula_Multiprocessamento_GPU_CUDA_H100/
├── slides.pdf                 # apresentação pronta para projetar (23 slides)
├── slides/
│   ├── main.tex               # fonte Beamer para Overleaf
│   └── figs/                  # figuras e logo oficial da UFCI/RJ
├── codigo/
│   ├── matriz_cpu.c           # referência sequencial
│   ├── matriz_h100.cu         # CUDA + referência CPU + medição + verificação
│   ├── Makefile
│   ├── job_h100.sbatch         # job Slurm para 1 H100
│   ├── README.md              # instruções detalhadas
│   └── .gitignore
└── README.md
```

## Slides

- PDF pronto: `slides.pdf`.
- Para editar no Overleaf, envie os arquivos de `slides/` para um novo projeto e compile `main.tex` com pdfLaTeX.
- A apresentação contém 23 slides, incluindo capa e referências.

## Para testar localmente apenas a CPU

```bash
cd codigo
make cpu
./matriz_cpu 4
./matriz_cpu 512
```

## Para usar uma NVIDIA H100 no Santos Dumont II

Copie a pasta para o seu diretório de trabalho no cluster, entre em `codigo/` **no nó de login** e submeta um job. Não execute kernels diretamente no nó de login.

```bash
cd codigo
sacctmgr list user "$USER" -s format=partition%20,account
sbatch --account=SEU_PROJETO job_h100.sbatch
squeue -u "$USER"
```

Substitua `SEU_PROJETO` pela conta Slurm com permissão para usar a partição solicitada. O script carrega `arch_gpu/current` e `cuda/12.9`, compila com `-arch=sm_90` e executa N = 4, 256, 512 e 1024. Os nomes de partições/módulos e as permissões podem mudar: confira `module avail`, `sinfo` e o manual do LNCC antes de submeter.

**Importante:** o código CUDA não usa Tensor Cores, cuBLAS nem multiplicação de matrizes em blocos (*tiling*). O objetivo é compreender grid/bloco/thread e o custo das transferências, não atingir o máximo de FLOP/s de uma H100.

## Medições feitas pelo exemplo CUDA

1. Tempo de cálculo CPU sequencial.
2. Tempo de kernel CUDA, medido por eventos no dispositivo.
3. Tempo de transferências + kernel, medido no host, sem contar `cudaMalloc`.
4. Checagem numérica CPU × GPU.

Para comparar o desempenho *total* de CPU e GPU neste exercício, use o tempo 3, com a ressalva de que a inicialização/alocação de memória não entra no cronômetro. Não compare exclusivamente tempo de kernel com tempo da CPU e chame isso de aceleração da aplicação inteira.

## Fontes e leituras

- [Manual do Santos Dumont II (LNCC)](https://github.com/lncc-sered/manual-sdumont2nd/wiki)
- [CUDA C++ Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/)
- [NVIDIA Hopper Tuning Guide](https://docs.nvidia.com/cuda/hopper-tuning-guide/)
