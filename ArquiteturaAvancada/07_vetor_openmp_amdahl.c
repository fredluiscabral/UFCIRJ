// Experimento 4: soma de vetores com uma parte serial e outra OpenMP.
// A fracao de ELEMENTOS nao e necessariamente a fracao de TEMPO de Amdahl.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

int main(int argc, char **argv) {
    long long n = (argc > 1) ? atoll(argv[1]) : 16000000LL;
    int repeticoes = (argc > 2) ? atoi(argv[2]) : 20;
    int percentual_serial = (argc > 3) ? atoi(argv[3]) : 10;
    if (n <= 0 || repeticoes <= 0 || percentual_serial < 0 || percentual_serial > 100 ||
        (unsigned long long)n > SIZE_MAX/sizeof(float)) {
        fprintf(stderr, "Uso: %s [N] [repeticoes] [percentual_serial=10]\n", argv[0]); return 1;
    }
    float *a = malloc((size_t)n * sizeof(float));
    float *b = malloc((size_t)n * sizeof(float));
    float *c = malloc((size_t)n * sizeof(float));
    if (!a || !b || !c) { fprintf(stderr, "Memoria insuficiente.\n"); free(a); free(b); free(c); return 1; }
    for (long long i = 0; i < n; i++) { a[i] = 1.0f; b[i] = 2.0f; }
    long long limite = (long long)((double)n * percentual_serial / 100.0);
    double tempo_serial = 0.0;
    double inicio = omp_get_wtime();
    for (int r = 0; r < repeticoes; r++) {
        // Regiao serial proposital: identica em todas as execucoes.
        double t0 = omp_get_wtime();
        for (long long i = 0; i < limite; i++)
            c[i] = a[i] + b[i];
        tempo_serial += omp_get_wtime() - t0;

        #pragma omp parallel for schedule(static)
        for (long long i = limite; i < n; i++)
            c[i] = a[i] + b[i];
    }
    double tempo = omp_get_wtime() - inicio;

    printf("VETOR OPENMP | threads=%d | N=%lld | repeticoes=%d | serial_elementos=%d%%\n",
           omp_get_max_threads(), n, repeticoes, percentual_serial);
    printf("Tempo total=%.6f s | Tempo trecho serial=%.6f s | C[ultimo]=%.1f\n",
           tempo, tempo_serial, c[n-1]);
    free(a); free(b); free(c);
    return 0;
}
