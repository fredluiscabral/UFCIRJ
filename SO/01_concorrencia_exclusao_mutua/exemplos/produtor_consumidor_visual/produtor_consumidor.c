#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define TAM_BUFFER 5
#define TOTAL_ITENS 20

/* Exemplo deliberadamente incorreto: sem sincronizacao do buffer. */
int buffer[TAM_BUFFER] = {0};
int entrada = 0;
int saida = 0;

/* Cores ANSI (terminal do VS Code/Linux). */
#define VERDE "\033[1;32m"
#define AZUL  "\033[1;36m"
#define AMARELO "\033[1;33m"
#define RESET "\033[0m"

static void mostrar_buffer(int posicao) {
    printf("   buffer: ");
    for (int i = 0; i < TAM_BUFFER; i++) {
        if (i == posicao) printf(AMARELO "[%2d]" RESET, buffer[i]);
        else printf("[%2d]", buffer[i]);
    }
    putchar('\n');
}

void *produtor(void *arg) {
    (void)arg;
    for (int item = 1; item <= TOTAL_ITENS; item++) {
        int posicao = entrada;
        buffer[posicao] = item;
        entrada = (entrada + 1) % TAM_BUFFER;
        /* A saida e a visualizacao do terminal sao agrupadas somente
           para nao embaralhar caracteres das linhas. Nao protege o buffer. */
        flockfile(stdout);
        printf(VERDE "PRODUZ " RESET "%2d -> posicao %d\n", item, posicao);
        mostrar_buffer(posicao);
        funlockfile(stdout);
        usleep(15000);
    }
    return NULL;
}

void *consumidor(void *arg) {
    (void)arg;
    /* Consumidor inicia depois: ilustra que o produtor pode sobrescrever
       itens antes de eles serem consumidos. O atraso nao corrige o problema. */
    usleep(350000);
    for (int i = 0; i < TOTAL_ITENS; i++) {
        int posicao = saida;
        int item = buffer[posicao];
        saida = (saida + 1) % TAM_BUFFER;
        flockfile(stdout);
        printf(AZUL "CONSOME " RESET "%2d <- posicao %d\n", item, posicao);
        mostrar_buffer(posicao);
        funlockfile(stdout);
        usleep(50000);
    }
    return NULL;
}

int main(void) {
    pthread_t p, c;
    puts("PRODUTOR-CONSUMIDOR: BUFFER SEM SINCRONIZACAO");
    puts("Amarelo = posicao acessada; verde = producao; azul = consumo");
    puts("Cada numero deveria ser consumido uma unica vez, na ordem 1..20.\n");
    pthread_create(&p, NULL, produtor, NULL);
    pthread_create(&c, NULL, consumidor, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}
