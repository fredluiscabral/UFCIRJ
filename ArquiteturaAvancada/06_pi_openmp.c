// Experimento 3B: MESMO calculo de PI, com OpenMP.
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    long long passos = (argc > 1) ? atoll(argv[1]) : 100000000LL;
    if (passos <= 0) { fprintf(stderr, "Uso: %s [passos]\n", argv[0]); return 1; }
    double largura = 1.0 / passos;
    double soma = 0.0;

    double inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:soma)
    for (long long i = 0; i < passos; i++) {
        double x = (i + 0.5) * largura;
        soma += 4.0 / (1.0 + x*x);
    }
    double pi = largura * soma;
    double tempo = omp_get_wtime() - inicio;

    printf("PI OPENMP | threads=%d | passos=%lld | pi=%.12f | Tempo=%.6f s\n",
           omp_get_max_threads(), passos, pi, tempo);
    return 0;
}
