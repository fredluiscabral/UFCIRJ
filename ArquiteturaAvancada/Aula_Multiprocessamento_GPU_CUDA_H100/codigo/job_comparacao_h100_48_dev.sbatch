#!/bin/bash
#SBATCH --job-name=omp-cuda-h100-48
#SBATCH --partition=lncc-h100_dev
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=48
#SBATCH --gpus=1
#SBATCH --time=00:15:00
#SBATCH --output=comparacao_h100_48_%j.out

# Fila de desenvolvimento: no maximo metade dos recursos do no.
set -euo pipefail
bash "$SLURM_SUBMIT_DIR/rodar_comparacao.sh"
