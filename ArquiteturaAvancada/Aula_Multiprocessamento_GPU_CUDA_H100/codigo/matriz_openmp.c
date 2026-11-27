// Mesmos dados, operacoes e ordem do codigo CUDA: C = A * B (float).
// Compilar: gcc -O3 -Wall -Wextra -std=c11 -fopenmp -o matriz_openmp matriz_openmp.c -lm
// Uso: ./matriz_openmp N THREADS REPETICOES
// Ex.: ./matriz_openmp 1024 48 7
#define _POSIX_C_SOURCE 199309L
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void multiplicar_referencia(const float *A, const float *B, float *C, int n) {
    for (int linha = 0; linha < n; ++linha) {
        for (int coluna = 0; coluna < n; ++coluna) {
            float soma = 0.0f;
            for (int k = 0; k < n; ++k)
                soma += A[(size_t)linha * n + k] * B[(size_t)k * n + coluna];
            C[(size_t)linha * n + coluna] = soma;
        }
    }
}

// Paralelizar so o laco de linhas. Cada thread escreve em linhas distintas de C.
// Nao precisamos de critical/atomic/reduction, pois cada celula pertence a uma thread.
static void multiplicar_openmp(const float *A, const float *B, float *C, int n) {
    #pragma omp parallel for schedule(static)
    for (int linha = 0; linha < n; ++linha) {
        for (int coluna = 0; coluna < n; ++coluna) {
            float soma = 0.0f;
            for (int k = 0; k < n; ++k)
                soma += A[(size_t)linha * n + k] * B[(size_t)k * n + coluna];
            C[(size_t)linha * n + coluna] = soma;
        }
    }
}

static int ordenar_double(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s N THREADS REPETICOES (N de 1 a 2048)\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]), threads = atoi(argv[2]), repeticoes = atoi(argv[3]);
    if (n < 1 || n > 2048 || threads < 1 || threads > 256 ||
        repeticoes < 1 || repeticoes > 100) {
        fprintf(stderr, "Argumentos invalidos. N:1-2048, threads:1-256, repeticoes:1-100.\n");
        return 1;
    }
    const char *limite = getenv("SLURM_CPUS_PER_TASK");
    if (limite && threads > atoi(limite)) {
        fprintf(stderr, "ERRO: solicitado %d threads, mas Slurm reservou %s CPUs por tarefa.\n",
                threads, limite);
        return 1;
    }

    const size_t quantidade = (size_t)n * (size_t)n;
    const size_t bytes = quantidade * sizeof(float);
    float *A = (float *)malloc(bytes), *B = (float *)malloc(bytes);
    float *C_ref = (float *)malloc(bytes), *C_omp = (float *)malloc(bytes);
    double *tempos = (double *)malloc((size_t)repeticoes * sizeof(double));
    if (!A || !B || !C_ref || !C_omp || !tempos) {
        fprintf(stderr, "ERRO: memoria insuficiente.\n");
        free(A); free(B); free(C_ref); free(C_omp); free(tempos);
        return 1;
    }
    for (size_t i = 0; i < quantidade; ++i) {
        A[i] = (n == 4) ? (float)(i + 1) : (float)((i % 7) + 1);
        B[i] = (n == 4) ? 1.0f : (float)((i % 5) + 1);
    }

    // Valida integralmente contra o mesmo algoritmo sequencial do exemplo CUDA.
    // Esta verificacao NAO entra nos tempos OpenMP.
    multiplicar_referencia(A, B, C_ref, n);

    omp_set_dynamic(0);
    omp_set_num_threads(threads);
    int threads_reais = 0;
    #pragma omp parallel
    {
        #pragma omp single
        threads_reais = omp_get_num_threads();
    }
    if (threads_reais != threads) {
        fprintf(stderr, "ERRO: solicitadas %d threads; obtidas %d.\n", threads, threads_reais);
        free(A); free(B); free(C_ref); free(C_omp); free(tempos);
        return 2;
    }

    // Aquecimento: evita incluir o primeiro acesso/initializacao de threads.
    multiplicar_openmp(A, B, C_omp, n);

    double soma_ms = 0.0;
    for (int r = 0; r < repeticoes; ++r) {
        double inicio = omp_get_wtime();
        multiplicar_openmp(A, B, C_omp, n);
        double fim = omp_get_wtime();
        tempos[r] = (fim - inicio) * 1000.0;
        soma_ms += tempos[r];
    }

    int divergencias = 0;
    for (size_t i = 0; i < quantidade; ++i)
        if (fabsf(C_ref[i] - C_omp[i]) > 0.001f) ++divergencias;

    qsort(tempos, (size_t)repeticoes, sizeof(double), ordenar_double);
    double media = soma_ms / repeticoes;
    double mediana = (repeticoes % 2) ? tempos[repeticoes / 2] :
        (tempos[repeticoes / 2 - 1] + tempos[repeticoes / 2]) / 2.0;
    double variancia = 0.0;
    for (int r = 0; r < repeticoes; ++r)
        variancia += (tempos[r] - media) * (tempos[r] - media);
    double desvio = sqrt(variancia / repeticoes);

    printf("OpenMP | N=%d | threads=%d | repeticoes=%d\n", n, threads, repeticoes);
    printf("Tempos (ms): min=%.6f media=%.6f mediana=%.6f max=%.6f dp=%.6f\n",
           tempos[0], media, mediana, tempos[repeticoes - 1], desvio);
    if (n == 4) {
        printf("Matriz C (4x4):\n");
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) printf("%6.0f", C_omp[(size_t)i * n + j]);
            putchar('\n');
        }
        puts("Observacao: tempos de 4x4 sao pequenos demais para speedups confiaveis.");
    }
    printf("Conferencia sequencial x OpenMP: %s (%d divergencias)\n",
           divergencias ? "FALHOU" : "OK", divergencias);
    // Formato processavel pelo script Slurm, todos os tempos em milissegundos.
    printf("CSV_OPENMP,%d,%d,%d,%.9f,%.9f,%.9f,%.9f,%.9f,%s\n",
           n, threads, repeticoes, tempos[0], media, mediana,
           tempos[repeticoes - 1], desvio, divergencias ? "FALHOU" : "OK");

    free(A); free(B); free(C_ref); free(C_omp); free(tempos);
    return divergencias ? 3 : 0;
}
