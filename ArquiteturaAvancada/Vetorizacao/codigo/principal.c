#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <time.h>

#ifndef TAMANHO
#define TAMANHO 1000000
#endif

#ifndef REPETICOES
#define REPETICOES 80
#endif

/* Implementadas em funcoes.c */
void operacoes(float a[], float b[], float saida[], int n);
void acumulado(float a[], float saida[], int n);

/* Vetores globais: evitam alocar muitos megabytes na pilha. */
static float a[TAMANHO];
static float b[TAMANHO];
static float saida[TAMANHO];

static double segundos(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
}

static double amostra(void) {
    double soma = 0.0;
    for (int i = 0; i < TAMANHO; i += 997) {
        soma += saida[i];
    }
    return soma;
}

int main(void) {
    for (int i = 0; i < TAMANHO; i++) {
        a[i] = (float)(i % 100) / 100.0f;
        b[i] = (float)((i * 7) % 100) / 100.0f;
    }

    /* Aquecimento: evita que a primeira chamada distorca a comparacao. */
    operacoes(a, b, saida, TAMANHO);
    acumulado(a, saida, TAMANHO);

    double inicio = segundos();
    for (int r = 0; r < REPETICOES; r++) {
        operacoes(a, b, saida, TAMANHO);
    }
    double tempo_operacoes = segundos() - inicio;
    double soma_operacoes = amostra();

    inicio = segundos();
    for (int r = 0; r < REPETICOES; r++) {
        acumulado(a, saida, TAMANHO);
    }
    double tempo_acumulado = segundos() - inicio;
    double soma_acumulado = amostra();

    printf("Tamanho = %d, repeticoes = %d\n", TAMANHO, REPETICOES);
    printf("OPERACOES  tempo = %.6f s | amostra = %.6f\n", tempo_operacoes, soma_operacoes);
    printf("ACUMULADO  tempo = %.6f s | amostra = %.6f\n", tempo_acumulado, soma_acumulado);
    return 0;
}
