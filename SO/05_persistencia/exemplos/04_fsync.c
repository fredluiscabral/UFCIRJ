#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    const char texto[] = "Linha escrita com fsync\n";
    int fd = open("sincronizado.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1) { perror("open"); return 1; }

    if (write(fd, texto, sizeof(texto) - 1) !=
        (ssize_t)(sizeof(texto) - 1)) {
        perror("write"); close(fd); return 1;
    }
    puts("1. write(): escrita aceita pelo sistema operacional.");

    if (fsync(fd) == -1) { perror("fsync"); close(fd); return 1; }
    puts("2. fsync(): sincronizacao solicitada e concluida.");

    if (close(fd) == -1) { perror("close"); return 1; }
    puts("3. close(): descritor fechado.");
    return 0;
}
