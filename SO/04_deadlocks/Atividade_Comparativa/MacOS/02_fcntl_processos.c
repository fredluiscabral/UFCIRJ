/* Experimento macOS 2: dois processos e bloqueios de registros com fcntl().
   Os filhos abrem os arquivos separadamente, APOS fork(). */
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ESPERA 10

static int bloquear(int fd) {
    struct flock trava = {0};
    trava.l_type = F_WRLCK;
    trava.l_whence = SEEK_SET;
    trava.l_start = 0;
    trava.l_len = 0; /* Da posicao inicial ate o fim do arquivo. */
    return fcntl(fd, F_SETLKW, &trava);
}

static int executar(const char *primeiro, const char *segundo) {
    int fd1 = open(primeiro, O_RDWR);
    if (fd1 == -1) {
        perror("open primeiro");
        return 1;
    }
    if (bloquear(fd1) == -1) {
        perror("fcntl primeiro");
        close(fd1);
        return 1;
    }
    printf("Processo %ld: bloqueou %s\n", (long)getpid(), primeiro);
    sleep(ESPERA);

    int fd2 = open(segundo, O_RDWR);
    if (fd2 == -1) {
        perror("open segundo");
        close(fd1);
        return 1;
    }
    printf("Processo %ld: solicitando bloqueio de %s\n",
           (long)getpid(), segundo);

    if (bloquear(fd2) == -1)
        perror("fcntl segundo");
    else
        printf("Processo %ld: obteve bloqueio de %s\n",
               (long)getpid(), segundo);

    close(fd2);
    close(fd1); /* Encerramento dos bloqueios deste processo. */
    return 0;
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    pid_t p1 = fork();
    if (p1 == -1) {
        perror("fork 1");
        return 1;
    }
    if (p1 == 0)
        _exit(executar("A.txt", "B.txt"));

    pid_t p2 = fork();
    if (p2 == -1) {
        perror("fork 2");
        /* Se o segundo fork falhar, o primeiro processo pode terminar sozinho. */
        waitpid(p1, NULL, 0);
        return 1;
    }
    if (p2 == 0)
        _exit(executar("B.txt", "A.txt"));

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}
