// Multiplicacao de matrizes quadradas: versao CPU sequencial.
// Compilar: gcc -O3 -o matriz_cpu matriz_cpu.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double segundos(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 4;
    if (n <= 0 || n > 2048) {
        fprintf(stderr, "Uso: %s [N entre 1 e 2048]\n", argv[0]);
        return 1;
    }
    size_t elementos = (size_t)n * (size_t)n;
    float *A = malloc(elementos * sizeof(float));
    float *B = malloc(elementos * sizeof(float));
    float *C = malloc(elementos * sizeof(float));
    if (!A || !B || !C) {
        fprintf(stderr, "Memoria insuficiente.\n");
        free(A); free(B); free(C);
        return 1;
    }

    for (size_t i = 0; i < elementos; i++) {
        A[i] = (n == 4) ? (float)(i + 1) : (float)((i % 7) + 1);
        B[i] = (n == 4) ? 1.0f : (float)((i % 5) + 1);
    }

    double inicio = segundos();
    for (int linha = 0; linha < n; linha++) {
        for (int coluna = 0; coluna < n; coluna++) {
            float soma = 0.0f;
            for (int k = 0; k < n; k++) {
                soma += A[linha*n + k] * B[k*n + coluna];
            }
            C[linha*n + coluna] = soma;
        }
    }
    double tempo = segundos() - inicio;

    if (n == 4) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) printf("%6.0f", C[i*n+j]);
            putchar('\n');
        }
    }
    printf("CPU sequencial | N=%d | Tempo=%.6f s\n", n, tempo);
    free(A); free(B); free(C);
    return 0;
}
