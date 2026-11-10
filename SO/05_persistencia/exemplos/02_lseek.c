#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    char buffer[11] = {0};
    int fd = open("posicoes.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd == -1) { perror("open"); return 1; }

    if (write(fd, "ABCDEFGHIJ", 10) != 10) {
        perror("write"); close(fd); return 1;
    }
    if (lseek(fd, 4, SEEK_SET) == -1) {
        perror("lseek"); close(fd); return 1;
    }
    if (write(fd, "x", 1) != 1) {
        perror("write"); close(fd); return 1;
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek"); close(fd); return 1;
    }
    if (read(fd, buffer, 10) != 10) {
        perror("read"); close(fd); return 1;
    }
    printf("Conteudo: %s\n", buffer);
    puts("A posicao 4 (quinta letra) foi sobrescrita.");
    if (close(fd) == -1) { perror("close"); return 1; }
    return 0;
}
