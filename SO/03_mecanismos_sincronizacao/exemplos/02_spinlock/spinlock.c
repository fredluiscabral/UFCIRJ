#include <stdio.h>
#include <stdatomic.h>
#include <omp.h>
#define REP 100000
static atomic_flag trava=ATOMIC_FLAG_INIT;
static long contador=0;
static void lock(void) {
    while(atomic_flag_test_and_set_explicit(&trava,memory_order_acquire)) {
        /* espera ocupada: usa CPU enquanto tenta */
    }
}
static void unlock(void) {
    atomic_flag_clear_explicit(&trava,memory_order_release);
}
int main(void) {
    #pragma omp parallel num_threads(4)
    {
        for(int i=0;i<REP;i++) {
            lock();
            contador++;
            unlock();
        }
    }
    printf("Contador: %ld (esperado: %d)\n",contador,4*REP);
    return 0;
}
