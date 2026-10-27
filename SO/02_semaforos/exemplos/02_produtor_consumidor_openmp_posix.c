#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <omp.h>
#define CAP 5
#define TOTAL 20
static int buffer[CAP], entrada = 0, saida = 0;
static sem_t vazios, cheios, mutex;
static void aguardar(sem_t *s) {
    int r;
    do { r = sem_wait(s); } while (r == -1 && errno == EINTR);
    if (r != 0) { perror("sem_wait"); exit(EXIT_FAILURE); }
}
static void liberar(sem_t *s) {
    if (sem_post(s) != 0) { perror("sem_post"); exit(EXIT_FAILURE); }
}
static void produtor(void) {
    for (int item = 1; item <= TOTAL; ++item) {
        aguardar(&vazios);
        aguardar(&mutex);
        int p = entrada;
        buffer[p] = item;
        entrada = (entrada + 1) % CAP;
        printf("PRODUZ %2d -> [%d]\n", item, p);
        liberar(&mutex);
        liberar(&cheios);
    }
}
static void consumidor(void) {
    for (int i = 0; i < TOTAL; ++i) {
        aguardar(&cheios);
        aguardar(&mutex);
        int p = saida, item = buffer[p];
        saida = (saida + 1) % CAP;
        printf("CONSOME %2d <- [%d]\n", item, p);
        liberar(&mutex);
        liberar(&vazios);
    }
}
int main(void) {
    if (sem_init(&vazios, 0, CAP) || sem_init(&cheios, 0, 0) || sem_init(&mutex, 0, 1)) {
        perror("sem_init"); return 1;
    }
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        produtor();
        #pragma omp section
        consumidor();
    }
    sem_destroy(&mutex); sem_destroy(&cheios); sem_destroy(&vazios);
    return 0;
}
