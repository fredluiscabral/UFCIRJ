// Experimento 2A: multiplicacao tradicional de matrizes (i, j, k).
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

static double relogio(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 768;
    if (n <= 0) { fprintf(stderr, "Uso: %s [N]\n", argv[0]); return 1; }
    size_t total = (size_t)n * n;
    if (total / n != (size_t)n || total > SIZE_MAX / sizeof(double)) return 1;

    double *a = malloc(total * sizeof(double));
    double *b = malloc(total * sizeof(double));
    double *c = malloc(total * sizeof(double));
    if (!a || !b || !c) { fprintf(stderr, "Memoria insuficiente.\n"); free(a); free(b); free(c); return 1; }
    for (size_t p = 0; p < total; p++) { a[p] = (p % 13) * 0.1; b[p] = (p % 7) * 0.1; }

    double inicio = relogio();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double soma = 0.0;
            for (int k = 0; k < n; k++)
                soma += a[(size_t)i*n + k] * b[(size_t)k*n + j];
            c[(size_t)i*n + j] = soma;
        }
    double tempo = relogio() - inicio;

    printf("MULT. SIMPLES | N=%d | Tempo=%.6f s | C[0]=%.4f | C[ultimo]=%.4f\n",
           n, tempo, c[0], c[total-1]);
    free(a); free(b); free(c);
    return 0;
}
