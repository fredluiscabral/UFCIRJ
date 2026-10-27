#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#define TAM_BUFFER 5
#define TOTAL_ITENS 100

/* Exemplo didático INTENCIONALMENTE sem sincronização.
 * Há data races e comportamento indefinido segundo o padrão C.
 */
int buffer[TAM_BUFFER];
int entrada = 0;
int saida = 0;
int quantidade = 0;

void *produtor(void *arg)
{
    (void)arg;
    for (int item = 1; item <= TOTAL_ITENS; item++) {
        while (quantidade == TAM_BUFFER)
            sched_yield();

        buffer[entrada] = item;
        entrada = (entrada + 1) % TAM_BUFFER;
        quantidade++;

        printf("PRODUZIU: %3d | Itens no buffer: %d\n", item, quantidade);
        usleep(1000);
    }
    return NULL;
}

void *consumidor(void *arg)
{
    (void)arg;
    for (int i = 0; i < TOTAL_ITENS; i++) {
        while (quantidade == 0)
            sched_yield();

        int item = buffer[saida];
        saida = (saida + 1) % TAM_BUFFER;
        quantidade--;

        printf("CONSUMIU: %3d | Itens no buffer: %d\n", item, quantidade);
        usleep(1500);
    }
    return NULL;
}

int main(void)
{
    pthread_t thread_produtor, thread_consumidor;

    printf("=== PRODUTOR-CONSUMIDOR (SEM SINCRONIZACAO) ===\n");
    printf("Buffer: %d posicoes\n", TAM_BUFFER);
    printf("Total de itens: %d\n\n", TOTAL_ITENS);

    if (pthread_create(&thread_produtor, NULL, produtor, NULL) != 0) {
        perror("pthread_create produtor");
        return 1;
    }
    if (pthread_create(&thread_consumidor, NULL, consumidor, NULL) != 0) {
        perror("pthread_create consumidor");
        return 1;
    }

    pthread_join(thread_produtor, NULL);
    pthread_join(thread_consumidor, NULL);

    printf("\n=== EXECUCAO FINALIZADA ===\n");
    printf("Quantidade final no buffer: %d\n", quantidade);
    return 0;
}
