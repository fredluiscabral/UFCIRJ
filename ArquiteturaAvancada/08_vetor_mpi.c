// Experimento 5: soma de vetores distribuida com MPI_Scatter e MPI_Gather.
// Para simplificar, N precisa ser divisivel pelo numero de processos.
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int id, processos;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &processos);

    int n = (argc > 1) ? atoi(argv[1]) : 8000000;
    if (n <= 0 || n % processos != 0) {
        if (id == 0) fprintf(stderr, "Uso: mpirun -np P %s [N] (N divisivel por P)\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int bloco = n / processos;
    float *a = NULL, *b = NULL, *c = NULL;
    if (id == 0) {
        a = malloc((size_t)n * sizeof(float));
        b = malloc((size_t)n * sizeof(float));
        c = malloc((size_t)n * sizeof(float));
        if (!a || !b || !c) MPI_Abort(MPI_COMM_WORLD, 2);
        for (int i = 0; i < n; i++) { a[i] = 1.0f; b[i] = 2.0f; }
    }
    float *a_local = malloc((size_t)bloco * sizeof(float));
    float *b_local = malloc((size_t)bloco * sizeof(float));
    float *c_local = malloc((size_t)bloco * sizeof(float));
    if (!a_local || !b_local || !c_local) MPI_Abort(MPI_COMM_WORLD, 3);

    MPI_Barrier(MPI_COMM_WORLD);
    double inicio = MPI_Wtime();
    MPI_Scatter(a, bloco, MPI_FLOAT, a_local, bloco, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, bloco, MPI_FLOAT, b_local, bloco, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < bloco; i++) c_local[i] = a_local[i] + b_local[i];

    MPI_Gather(c_local, bloco, MPI_FLOAT, c, bloco, MPI_FLOAT, 0, MPI_COMM_WORLD);
    double meu_tempo = MPI_Wtime() - inicio;
    double tempo_total;
    MPI_Reduce(&meu_tempo, &tempo_total, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (id == 0)
        printf("VETOR MPI | processos=%d | N=%d | Tempo=%.6f s | C[ultimo]=%.1f\n",
               processos, n, tempo_total, c[n-1]);
    free(a_local); free(b_local); free(c_local);
    free(a); free(b); free(c);
    MPI_Finalize();
    return 0;
}
