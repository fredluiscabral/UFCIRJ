/* Experimento macOS 1: duas threads e bloqueios de arquivos com flock().
   Compare os resultados com o experimento Linux. */
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>

#define ESPERA 10

typedef struct {
    const char *nome;
    const char *primeiro;
    const char *segundo;
} Experimento;

static void *executar(void *arg) {
    const Experimento *e = arg;

    int fd1 = open(e->primeiro, O_RDWR);
    if (fd1 == -1) {
        perror("open primeiro");
        return NULL;
    }

    if (flock(fd1, LOCK_EX) == -1) {
        perror("flock primeiro");
        close(fd1);
        return NULL;
    }
    printf("%s: bloqueou %s\n", e->nome, e->primeiro);
    sleep(ESPERA);

    int fd2 = open(e->segundo, O_RDWR);
    if (fd2 == -1) {
        perror("open segundo");
        flock(fd1, LOCK_UN);
        close(fd1);
        return NULL;
    }

    printf("%s: solicitando bloqueio de %s\n", e->nome, e->segundo);
    if (flock(fd2, LOCK_EX) == -1)
        perror("flock segundo");
    else {
        printf("%s: obteve bloqueio de %s\n", e->nome, e->segundo);
        flock(fd2, LOCK_UN);
    }

    flock(fd1, LOCK_UN);
    close(fd2);
    close(fd1);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    Experimento a = {"Thread 1", "A.txt", "B.txt"};
    Experimento b = {"Thread 2", "B.txt", "A.txt"};
    setvbuf(stdout, NULL, _IONBF, 0);

    int r = pthread_create(&t1, NULL, executar, &a);
    if (r != 0) {
        fprintf(stderr, "pthread_create Thread 1: erro %d\n", r);
        return 1;
    }
    r = pthread_create(&t2, NULL, executar, &b);
    if (r != 0) {
        fprintf(stderr, "pthread_create Thread 2: erro %d\n", r);
        pthread_join(t1, NULL);
        return 1;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
