/* Experimento 2: cada execucao e um processo independente. */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static int bloquear(int fd) {
    struct flock trava = {0};
    trava.l_type = F_WRLCK;
    trava.l_whence = SEEK_SET;
    trava.l_start = 0;
    trava.l_len = 0; /* Do inicio do arquivo ate o fim. */
    return fcntl(fd, F_SETLKW, &trava);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s PRIMEIRO.txt SEGUNDO.txt\n", argv[0]);
        return 1;
    }

    setvbuf(stdout, NULL, _IONBF, 0);
    int primeiro = open(argv[1], O_RDWR);
    if (primeiro == -1) {
        perror("open primeiro");
        return 1;
    }
    if (bloquear(primeiro) == -1) {
        perror("bloquear primeiro");
        close(primeiro);
        return 1;
    }
    printf("Processo %ld: bloqueou %s\n", (long)getpid(), argv[1]);

    sleep(10);

    int segundo = open(argv[2], O_RDWR);
    if (segundo == -1) {
        perror("open segundo");
        close(primeiro);
        return 1;
    }
    printf("Processo %ld: solicitando bloqueio de %s\n",
           (long)getpid(), argv[2]);

    if (bloquear(segundo) == -1)
        perror("bloquear segundo");
    else
        printf("Processo %ld: obteve bloqueio de %s\n",
               (long)getpid(), argv[2]);

    /* Os bloqueios tradicionais deste exemplo terminam ao fechar os descritores. */
    close(segundo);
    close(primeiro);
    return 0;
}
