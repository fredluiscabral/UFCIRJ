#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    const char *nome = argc > 1 ? argv[1] : "registro.txt";
    struct stat info, destino;

    if (lstat(nome, &info) == -1) { perror("lstat"); return 1; }
    if (stat(nome, &destino) == -1) { perror("stat"); return 1; }

    printf("Caminho: %s\n", nome);
    printf("Tipo do caminho: %s\n",
           S_ISLNK(info.st_mode) ? "link simbolico" :
           S_ISDIR(info.st_mode) ? "diretorio" : "arquivo/outro");
    printf("Inode (lstat): %llu\n", (unsigned long long)info.st_ino);
    printf("Inode do destino (stat): %llu\n",
           (unsigned long long)destino.st_ino);
    printf("Links fisicos do destino: %llu\n",
           (unsigned long long)destino.st_nlink);
    printf("Tamanho do destino: %lld bytes\n",
           (long long)destino.st_size);
    printf("Permissoes do destino: %03o\n",
           (unsigned)(destino.st_mode & 0777));
    return 0;
}
