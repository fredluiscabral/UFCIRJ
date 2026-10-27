#include <stdio.h>
#include <omp.h>
#define N 1000000
int main(void) {
    long contador = 0;
    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < N; ++i) {
        contador++; /* Data race intencional: comportamento indefinido. */
    }
    printf("Esperado: %d | Obtido: %ld\n", N, contador);
    return 0;
}
