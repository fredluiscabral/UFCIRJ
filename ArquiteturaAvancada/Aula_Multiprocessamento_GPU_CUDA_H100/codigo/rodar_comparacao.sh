#!/bin/bash
# Execucao sequencial dos benchmarks CUDA e OpenMP no mesmo no Slurm.
# Chamado pelos scripts job_comparacao_h100_*.sbatch (nao executar no login).
set -euo pipefail
cd "$SLURM_SUBMIT_DIR"

module load arch_gpu/current
module load cuda/12.9

export OMP_DYNAMIC=FALSE
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
export OMP_MAX_ACTIVE_LEVELS=1

printf '\n=== AMBIENTE ===\n'
printf 'Job: %s | No: %s | Particao: %s | CPUs reservadas: %s\n' \
    "$SLURM_JOB_ID" "$(hostname)" "$SLURM_JOB_PARTITION" "$SLURM_CPUS_PER_TASK"
command -v lscpu >/dev/null && lscpu | grep -E 'Model name:|Socket\(s\):|Core\(s\) per socket:' || true
nvidia-smi -L
printf '\n=== COMPILACAO ===\n'
gcc --version | head -1
nvcc --version | tail -4
make openmp gpu

REPS=5
MAX="$SLURM_CPUS_PER_TASK"
CSV="resultados_openmp_${SLURM_JOB_ID}.csv"
CSV_CUDA="resultados_cuda_${SLURM_JOB_ID}.csv"
LOG_OMP="openmp_${SLURM_JOB_ID}.txt"
LOG_CUDA="cuda_comparacao_${SLURM_JOB_ID}.txt"

printf 'matriz,threads,repeticoes,min_ms,media_ms,mediana_ms,max_ms,desvio_ms,validacao\n' > "$CSV"
: > "$LOG_OMP"
: > "$LOG_CUDA"

printf '\n=== OPENMP: mesmo algoritmo, entrada e dimensoes ===\n'
for N in 4 256 512 1024; do
    for T in 1 2 4 8 16 24 32 48 64 96; do
        if (( T > MAX )); then
            continue
        fi
        printf '\n--- Matriz %dx%d | %d threads ---\n' "$N" "$N" "$T"
        export OMP_NUM_THREADS="$T"
        # As 96/48 CPUs ja estao reservadas; o binario limita as threads por parametro.
        srun --cpus-per-task="$MAX" --cpu-bind=cores \
            ./matriz_openmp "$N" "$T" "$REPS" \
            | tee -a "$LOG_OMP" \
            | awk -v csv="$CSV" '
                /^CSV_OPENMP,/ { row=$0; sub(/^CSV_OPENMP,/, "", row); print row >> csv; fflush(csv) }
                { print; fflush() }
              '
    done
done

printf '\n=== CUDA: mesmas matrizes na H100 (kernel + transferencias) ===\n'
for N in 4 256 512 1024; do
    printf '\n--- CUDA Matriz %dx%d ---\n' "$N" "$N"
    srun --cpus-per-task="$MAX" ./matriz_h100 "$N" | tee -a "$LOG_CUDA"
done

# Converte a saida ja produzida pelo programa CUDA em tabela numerica.
awk '
BEGIN { OFS=","; print "matriz,cpu_sequencial_ms,gpu_kernel_ms,gpu_total_ms,validacao" }
/^Matriz: / { split($2,a,"x"); N=a[1] }
/^Tempo CPU sequencial:/ { cpu=$4*1000 }
/^Tempo GPU kernel:/ { kernel=$4 }
/^Tempo GPU com transferencias:/ { total=$5*1000 }
/^Conferencia CPU x GPU:/ { print N,cpu,kernel,total,$5 }
' "$LOG_CUDA" > "$CSV_CUDA"

if command -v python3 >/dev/null 2>&1; then
    python3 comparar_resultados.py "$SLURM_JOB_ID"
else
    echo 'python3 nao encontrado. CSVs brutos ja estao salvos.'
fi

printf '\n=== RESULTADOS SALVOS ===\n'
printf 'OpenMP: %s\nCUDA:   %s\nLog OpenMP: %s\nLog CUDA:   %s\n' \
    "$CSV" "$CSV_CUDA" "$LOG_OMP" "$LOG_CUDA"
cat "$CSV_CUDA"
