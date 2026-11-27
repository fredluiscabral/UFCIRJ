#!/bin/bash
#SBATCH --job-name=cuda-geometria
#SBATCH --partition=lncc-h100_dev
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --gpus=1
#SBATCH --time=00:10:00
#SBATCH --output=variacao_blocos_%j.out

set -euo pipefail
cd "$SLURM_SUBMIT_DIR"

module load arch_gpu/current
module load cuda/12.9

echo "=== Experimento CUDA: geometria dos blocos ==="
echo "Job: $SLURM_JOB_ID | No: $(hostname) | Particao: $SLURM_JOB_PARTITION"
nvidia-smi -L
nvcc -O3 -arch=sm_90 -std=c++14 -o matriz_blocos_h100 matriz_blocos_h100.cu

# N=1024; 30 repeticoes de medicao por geometria, mais aquecimento.
srun ./matriz_blocos_h100 1024 30 "variacao_blocos_${SLURM_JOB_ID}.csv"
