#!/bin/bash
#SBATCH --job-name=omp-cuda-h100-96
#SBATCH --partition=lncc-h100
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=96
#SBATCH --time=00:15:00
#SBATCH --output=comparacao_h100_96_%j.out

# Fila de alocacao EXCLUSIVA: ocupa um no H100 inteiro.
# O manual proibe --gpus nesta fila. Evite reservar o no sem necessidade.
set -euo pipefail
bash "$SLURM_SUBMIT_DIR/rodar_comparacao.sh"
