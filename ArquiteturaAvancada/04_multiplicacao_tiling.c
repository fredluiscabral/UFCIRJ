// Experimento 2B: MESMA ordem interna (i, j, k), agora com blocos.
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
static int minimo(int a, int b) { return a < b ? a : b; }

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 768;
    int bloco = (argc > 2) ? atoi(argv[2]) : 32;
    if (n <= 0 || bloco <= 0) { fprintf(stderr, "Uso: %s [N] [bloco]\n", argv[0]); return 1; }
    size_t total = (size_t)n * n;
    if (total / n != (size_t)n || total > SIZE_MAX / sizeof(double)) return 1;

    double *a = malloc(total * sizeof(double));
    double *b = malloc(total * sizeof(double));
    double *c = calloc(total, sizeof(double));
    if (!a || !b || !c) { fprintf(stderr, "Memoria insuficiente.\n"); free(a); free(b); free(c); return 1; }
    for (size_t p = 0; p < total; p++) { a[p] = (p % 13) * 0.1; b[p] = (p % 7) * 0.1; }

    double inicio = relogio();
    for (int ii = 0; ii < n; ii += bloco)
        for (int jj = 0; jj < n; jj += bloco)
            for (int kk = 0; kk < n; kk += bloco)
                for (int i = ii; i < minimo(ii+bloco, n); i++)
                    for (int j = jj; j < minimo(jj+bloco, n); j++) {
                        double soma = c[(size_t)i*n + j];
                        for (int k = kk; k < minimo(kk+bloco, n); k++)
                            soma += a[(size_t)i*n + k] * b[(size_t)k*n + j];
                        c[(size_t)i*n + j] = soma;
                    }
    double tempo = relogio() - inicio;

    printf("MULT. TILING | N=%d | Bloco=%d | Tempo=%.6f s | C[0]=%.4f | C[ultimo]=%.4f\n",
           n, bloco, tempo, c[0], c[total-1]);
    free(a); free(b); free(c);
    return 0;
}
