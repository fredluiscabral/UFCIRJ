#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <omp.h>
#define N 100000
int main(void) {
    sem_t mutex;
    long contador = 0;
    if (sem_init(&mutex, 0, 1) != 0) { perror("sem_init"); return 1; }
    int erro = 0;
    #pragma omp parallel for num_threads(4) reduction(|:erro)
    for (int i = 0; i < N; ++i) {
        int r;
        do { r = sem_wait(&mutex); } while (r == -1 && errno == EINTR);
        if (r != 0) { erro = 1; continue; }
        contador++;
        if (sem_post(&mutex) != 0) erro = 1;
    }
    if (sem_destroy(&mutex) != 0) { perror("sem_destroy"); return 1; }
    printf("Esperado: %d | Obtido: %ld\n", N, contador);
    return erro ? 1 : 0;
}
