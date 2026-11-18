/* Experimento 1: duas threads usam flock() em A.txt e B.txt. */
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>

static void *executar(void *arg) {
    const char **arquivos = arg;

    int primeiro = open(arquivos[0], O_RDWR);
    if (primeiro == -1) {
        perror("open primeiro");
        return NULL;
    }
    if (flock(primeiro, LOCK_EX) == -1) {
        perror("flock primeiro");
        close(primeiro);
        return NULL;
    }
    printf("Thread %lu: bloqueou %s\n",
           (unsigned long)pthread_self(), arquivos[0]);

    sleep(10);

    int segundo = open(arquivos[1], O_RDWR);
    if (segundo == -1) {
        perror("open segundo");
        close(primeiro);
        return NULL;
    }
    printf("Thread %lu: solicitando bloqueio de %s\n",
           (unsigned long)pthread_self(), arquivos[1]);

    if (flock(segundo, LOCK_EX) == -1)
        perror("flock segundo");
    else
        printf("Thread %lu: obteve bloqueio de %s\n",
               (unsigned long)pthread_self(), arquivos[1]);

    close(segundo);
    close(primeiro);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    const char *ordem1[] = {"A.txt", "B.txt"};
    const char *ordem2[] = {"B.txt", "A.txt"};
    setvbuf(stdout, NULL, _IONBF, 0);

    if (pthread_create(&t1, NULL, executar, ordem1) != 0) {
        fputs("Falha ao criar thread 1\n", stderr);
        return 1;
    }
    if (pthread_create(&t2, NULL, executar, ordem2) != 0) {
        fputs("Falha ao criar thread 2\n", stderr);
        pthread_join(t1, NULL);
        return 1;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
