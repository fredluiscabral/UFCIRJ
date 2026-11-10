/* Algoritmo de seguranca do banqueiro: 3 processos, 1 tipo de recurso.
   O teste termina; o main repete o teste indefinidamente (Ctrl+C). */
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define N 3

bool estado_seguro(int alocado[N], int maximo[N], int disponiveis) {
    bool terminou[N] = {false};
    int concluidos = 0;
    printf("Disponiveis no inicio: %d\n", disponiveis);

    while (concluidos < N) {
        bool avancou = false;
        for (int i = 0; i < N; i++) {
            int falta = maximo[i] - alocado[i];
            if (!terminou[i] && falta <= disponiveis) {
                printf("P%d termina (faltava %d); devolve %d\n",
                       i, falta, alocado[i]);
                disponiveis += alocado[i];
                terminou[i] = true;
                concluidos++;
                avancou = true;
                break; /* Volta ao P0 para encontrar a proxima etapa. */
            }
        }
        if (!avancou) return false;
    }
    return true;
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    int alocado[N] = {3, 2, 2};
    int maximo[N] = {7, 4, 5};
    int apos_pedido[N] = {5, 2, 2}; /* P0 recebeu mais 2. */

    while (true) {
        printf("\n=== ESTADO ATUAL: 10 recursos no total ===\n");
        printf("Seguro? %s\n",
               estado_seguro(alocado, maximo, 3) ? "SIM" : "NAO");

        printf("\n=== E SE P0 RECEBER MAIS 2 RECURSOS? ===\n");
        printf("Seguro? %s\n",
               estado_seguro(apos_pedido, maximo, 1) ? "SIM" : "NAO");
        printf("Pedido negado: o estado seria inseguro.\n");
        sleep(4);
    }
    return 0;
}
