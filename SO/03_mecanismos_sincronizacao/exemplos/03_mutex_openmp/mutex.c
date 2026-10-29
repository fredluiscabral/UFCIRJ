#include <stdio.h>
#include <pthread.h>
#include <omp.h>
#define REP 100000
static int contador=0;
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int main(void) {
    #pragma omp parallel num_threads(4)
    {
        for(int i=0;i<REP;i++) {
            pthread_mutex_lock(&mutex);
            contador++;
            pthread_mutex_unlock(&mutex);
        }
    }
    printf("Contador: %d (esperado: %d)\n",contador,4*REP);
    pthread_mutex_destroy(&mutex);
    return 0;
}
