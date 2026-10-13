#define _DEFAULT_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define TAM_BUFFER 5
#define TOTAL_ITENS 100

/* Exemplo INTENCIONALMENTE INCORRETO: sem coordenacao ou exclusao mutua. */
int buffer[TAM_BUFFER];
int entrada = 0;
int saida = 0;
int recebidos[TOTAL_ITENS];

void *produtor(void *arg) {
    (void)arg;
    for (int item = 1; item <= TOTAL_ITENS; ++item) {
        /* ERRO: escreve sem verificar se existe espaco disponivel. */
        buffer[entrada] = item;
        entrada = (entrada + 1) % TAM_BUFFER;
        usleep(1000); /* produtor mais rapido */
    }
    return NULL;
}

void *consumidor(void *arg) {
    (void)arg;
    for (int i = 0; i < TOTAL_ITENS; ++i) {
        /* ERRO: le sem verificar se ja existe um item nesta posicao. */
        recebidos[i] = buffer[saida];
        saida = (saida + 1) % TAM_BUFFER;
        usleep(7000); /* consumidor mais lento */
    }
    return NULL;
}

int main(void) {
    pthread_t tp, tc;
    if (pthread_create(&tp, NULL, produtor, NULL) != 0 ||
        pthread_create(&tc, NULL, consumidor, NULL) != 0) {
        fprintf(stderr, "Falha ao criar threads\n");
        return 1;
    }
    pthread_join(tp, NULL);
    pthread_join(tc, NULL);

    int divergencias = 0;
    printf("=== COMPARACAO (esperado x recebido) ===\n");
    for (int i = 0; i < TOTAL_ITENS; ++i) {
        int esperado = i + 1;
        int lido = recebidos[i];
        if (lido != esperado) {
            ++divergencias;
            printf("ERRO no consumo %3d: esperado %3d, recebido %3d\n",
                   i + 1, esperado, lido);
        }
    }
    printf("\n=== RESUMO ===\n");
    printf("Itens gerados pelo produtor: %d\n", TOTAL_ITENS);
    printf("Leituras do consumidor:      %d\n", TOTAL_ITENS);
    printf("Leituras fora do esperado:   %d\n", divergencias);
    printf("Resultado: %s\n", divergencias ? "FALHA" : "SEM FALHA OBSERVADA");
    return 0;
}
