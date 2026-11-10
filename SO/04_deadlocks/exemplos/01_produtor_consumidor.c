/* Produtor-consumidor com buffer circular de N posicoes.
   Uso: ./01_produtor_consumidor correto | errado
   Ctrl+C encerra; a versao errada pode entrar em deadlock. */
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define N 5

int buffer[N], entrada = 0, saida = 0;
sem_t mutex, vazios, cheios;
bool errado = false;

void *produtor(void *arg) {
    (void)arg;
    int valor = 200;
    while (true) {
        if (errado) {
            sem_wait(&mutex);  /* ERRO: segura mutex enquanto espera espaco */
            printf("Produtor: mutex adquirido; esperando espaco...\n");
            sem_wait(&vazios);
        } else {
            sem_wait(&vazios);
            sem_wait(&mutex);
        }

        buffer[entrada] = valor;
        printf("Produziu %d na posicao %d\n", valor, entrada);
        entrada = (entrada + 1) % N;
        valor++;

        sem_post(&mutex);
        sem_post(&cheios);
        sleep(1);
    }
    return NULL;
}

void *consumidor(void *arg) {
    (void)arg;
    while (true) {
        sem_wait(&cheios);
        sem_wait(&mutex);

        int valor = buffer[saida];
        printf("Consumiu %d da posicao %d\n", valor, saida);
        saida = (saida + 1) % N;

        sem_post(&mutex);
        sem_post(&vazios);
        sleep(1);
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

    /* Comecamos com N itens: buffer inicialmente cheio. */
    for (int i = 0; i < N; i++) buffer[i] = 100 + i;
    sem_init(&mutex, 0, 1);
    sem_init(&vazios, 0, 0);
    sem_init(&cheios, 0, N);

    printf("Buffer de %d posicoes, inicialmente cheio. Modo: %s\n",
           N, argv[1]);
    pthread_t tp, tc;
    pthread_create(&tp, NULL, produtor, NULL);
    sleep(1); /* Ajuda o produtor a tentar primeiro. */
    pthread_create(&tc, NULL, consumidor, NULL);
    pthread_join(tp, NULL);
    pthread_join(tc, NULL);
    return 0;
}
