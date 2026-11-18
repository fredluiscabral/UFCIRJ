/* Experimento Windows: duas threads e bloqueios de arquivo com LockFileEx().
   Cada thread abre seus proprios handles para A.txt e B.txt. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define ESPERA_MS 10000

typedef struct {
    const char *nome;
    const char *primeiro;
    const char *segundo;
} Experimento;

static HANDLE abrir(const char *nome) {
    return CreateFileA(nome,
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

static BOOL bloquear(HANDLE arquivo, OVERLAPPED *ov) {
    memset(ov, 0, sizeof(*ov));
    /* Bloqueia 1 byte, a partir da posicao 0. */
    return LockFileEx(arquivo, LOCKFILE_EXCLUSIVE_LOCK, 0, 1, 0, ov);
}

static DWORD WINAPI executar(LPVOID arg) {
    const Experimento *e = (const Experimento *)arg;
    OVERLAPPED ov1, ov2;
    HANDLE fd1 = abrir(e->primeiro);
    if (fd1 == INVALID_HANDLE_VALUE) {
        printf("%s: erro ao abrir %s: %lu\n",
               e->nome, e->primeiro, (unsigned long)GetLastError());
        return 1;
    }

    if (!bloquear(fd1, &ov1)) {
        printf("%s: erro no primeiro bloqueio: %lu\n",
               e->nome, (unsigned long)GetLastError());
        CloseHandle(fd1);
        return 1;
    }
    printf("%s: bloqueou %s\n", e->nome, e->primeiro);
    Sleep(ESPERA_MS);

    HANDLE fd2 = abrir(e->segundo);
    if (fd2 == INVALID_HANDLE_VALUE) {
        printf("%s: erro ao abrir %s: %lu\n",
               e->nome, e->segundo, (unsigned long)GetLastError());
        UnlockFileEx(fd1, 0, 1, 0, &ov1);
        CloseHandle(fd1);
        return 1;
    }

    printf("%s: solicitando bloqueio de %s\n", e->nome, e->segundo);
    if (!bloquear(fd2, &ov2))
        printf("%s: erro no segundo bloqueio: %lu\n",
               e->nome, (unsigned long)GetLastError());
    else {
        printf("%s: obteve bloqueio de %s\n", e->nome, e->segundo);
        UnlockFileEx(fd2, 0, 1, 0, &ov2);
    }

    UnlockFileEx(fd1, 0, 1, 0, &ov1);
    CloseHandle(fd2);
    CloseHandle(fd1);
    return 0;
}

int main(void) {
    Experimento a = {"Thread 1", "A.txt", "B.txt"};
    Experimento b = {"Thread 2", "B.txt", "A.txt"};
    HANDLE threads[2];
    setvbuf(stdout, NULL, _IONBF, 0);

    threads[0] = CreateThread(NULL, 0, executar, &a, 0, NULL);
    if (threads[0] == NULL) {
        printf("Erro ao criar Thread 1: %lu\n", (unsigned long)GetLastError());
        return 1;
    }

    threads[1] = CreateThread(NULL, 0, executar, &b, 0, NULL);
    if (threads[1] == NULL) {
        printf("Erro ao criar Thread 2: %lu\n", (unsigned long)GetLastError());
        WaitForSingleObject(threads[0], INFINITE);
        CloseHandle(threads[0]);
        return 1;
    }

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
    return 0;
}
