/*
 * Atividade investigativa: vetorizacao automatica no GCC.
 * Estes sao os dois lacos que devem ser analisados no Assembly.
 */

/* Experimento A: cada posicao do vetor e calculada independentemente. */
void operacoes(float a[], float b[], float saida[], int n) {
    for (int i = 0; i < n; i++) {
        float x = a[i];
        float y = b[i];
        saida[i] = (x*x + y*y) * (x+y) + x*y;
    }
}

/* Experimento B: cada iteracao depende do resultado anterior (RAW). */
void acumulado(float a[], float saida[], int n) {
    if (n <= 0) return;

    saida[0] = a[0];
    for (int i = 1; i < n; i++) {
        saida[i] = saida[i - 1] + a[i];
    }
}
