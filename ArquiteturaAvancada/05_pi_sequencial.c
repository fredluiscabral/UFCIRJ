// Experimento 3A: calculo de PI por integracao numerica, sequencial.
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double relogio(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    long long passos = (argc > 1) ? atoll(argv[1]) : 100000000LL;
    if (passos <= 0) { fprintf(stderr, "Uso: %s [passos]\n", argv[0]); return 1; }
    double largura = 1.0 / passos;
    double soma = 0.0;

    double inicio = relogio();
    for (long long i = 0; i < passos; i++) {
        double x = (i + 0.5) * largura;
        soma += 4.0 / (1.0 + x*x);
    }
    double pi = largura * soma;
    double tempo = relogio() - inicio;

    printf("PI SEQUENCIAL | passos=%lld | pi=%.12f | Tempo=%.6f s\n", passos, pi, tempo);
    return 0;
}
