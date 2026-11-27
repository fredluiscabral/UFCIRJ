#!/bin/bash
#SBATCH --job-name=aula-cuda-h100
#SBATCH --partition=lncc-h100_dev
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --gpus=1
#SBATCH --time=00:10:00
#SBATCH --output=cuda_h100_%j.out

set -euo pipefail
cd "$SLURM_SUBMIT_DIR"

# Modulos publicados pelo manual do Santos Dumont II (LNCC).
module load arch_gpu/current
module load cuda/12.9

printf 'No: %s | Job: %s\n' "$(hostname)" "$SLURM_JOB_ID"
nvidia-smi -L
nvcc --version

make gpu

# N=4: explicar linhas, colunas, blocos e threads.
srun ./matriz_h100 4

# N maiores: comparar tempo de CPU sequencial, kernel e transferencias.
srun ./matriz_h100 256
srun ./matriz_h100 512
srun ./matriz_h100 1024
