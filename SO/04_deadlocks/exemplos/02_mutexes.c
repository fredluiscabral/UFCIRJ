/* Duas threads e dois mutexes: ordem diferente causa deadlock.
   Uso: ./02_mutexes correto | errado ; Ctrl+C encerra. */
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t b = PTHREAD_MUTEX_INITIALIZER;
bool errado = false;

void *thread_1(void *arg) {
    (void)arg;
    while (true) {
        pthread_mutex_lock(&a);
        printf("T1: adquiriu A\n");
        sleep(1);
        printf("T1: tentando B\n");
        pthread_mutex_lock(&b);
        printf("T1: adquiriu B; terminou a operacao\n");
        pthread_mutex_unlock(&b);
        pthread_mutex_unlock(&a);
        sleep(1);
    }
    return NULL;
}

void *thread_2(void *arg) {
    (void)arg;
    while (true) {
        if (errado) {
            pthread_mutex_lock(&b);
            printf("T2: adquiriu B\n");
            sleep(1);
            printf("T2: tentando A\n");
            pthread_mutex_lock(&a);
            pthread_mutex_unlock(&a);
            pthread_mutex_unlock(&b);
        } else {
            pthread_mutex_lock(&a);
            printf("T2: adquiriu A\n");
            sleep(1);
            printf("T2: tentando B\n");
            pthread_mutex_lock(&b);
            pthread_mutex_unlock(&b);
            pthread_mutex_unlock(&a);
        }
        printf("T2: terminou a operacao\n");
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
    printf("Ordem de aquisicao: %s\n", errado ? "oposta" : "A, depois B");

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_1, NULL);
    pthread_create(&t2, NULL, thread_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
