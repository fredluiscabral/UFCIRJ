/* N filosofos: versao incorreta e prevencao com sala=N-1.
   Uso: ./03_filosofos correto | errado ; Ctrl+C encerra. */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define N 5

sem_t garfo[N], sala;
bool errado = false;

void *filosofo(void *arg) {
    int i = *(int *)arg;
    int direita = (i + 1) % N;
    while (true) {
        if (!errado) sem_wait(&sala);

        sem_wait(&garfo[i]);
        printf("Filosofo %d pegou garfo %d\n", i, i);
        sleep(1); /* Facilita observar a espera circular no modo errado. */
        printf("Filosofo %d tenta garfo %d\n", i, direita);
        sem_wait(&garfo[direita]);

        printf("Filosofo %d esta comendo\n", i);
        sleep(1);
        sem_post(&garfo[direita]);
        sem_post(&garfo[i]);
        if (!errado) sem_post(&sala);
        sleep(1); /* Pensando. */
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2 || (strcmp(argv[1], "correto") != 0 &&
                      strcmp(argv[1], "errado") != 0)) {
        fprintf(stderr, "Uso: %s correto|errado\n", argv[0]);
        return 1;
    }
    errado = strcmp(argv[1], "errado") == 0;
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("%d filosofos. Modo: %s\n", N, argv[1]);
    for (int i = 0; i < N; i++) sem_init(&garfo[i], 0, 1);
    sem_init(&sala, 0, N - 1);

    pthread_t t[N];
    int id[N];
    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, filosofo, &id[i]);
    }
    for (int i = 0; i < N; i++) pthread_join(t[i], NULL);
    return 0;
}
