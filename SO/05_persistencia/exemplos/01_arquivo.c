#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s gravar|ler\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "gravar") == 0) {
        const char mensagem[] = "Registro gravado pelo processo\n";
        fd = open("registro.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) { perror("open"); return 1; }
        if (write(fd, mensagem, sizeof(mensagem) - 1) !=
            (ssize_t)(sizeof(mensagem) - 1)) {
            perror("write");
            close(fd);
            return 1;
        }
        if (close(fd) == -1) { perror("close"); return 1; }
        puts("Gravacao concluida. O processo ja pode terminar.");
    } else if (strcmp(argv[1], "ler") == 0) {
        char buffer[128];
        ssize_t n;
        fd = open("registro.txt", O_RDONLY);
        if (fd == -1) { perror("open"); return 1; }
        while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
            if (fwrite(buffer, 1, (size_t)n, stdout) != (size_t)n) {
                perror("fwrite");
                close(fd);
                return 1;
            }
        }
        if (n < 0) { perror("read"); close(fd); return 1; }
        if (close(fd) == -1) { perror("close"); return 1; }
    } else {
        fprintf(stderr, "Opcao desconhecida: %s\n", argv[1]);
        return 1;
    }
    return 0;
}
