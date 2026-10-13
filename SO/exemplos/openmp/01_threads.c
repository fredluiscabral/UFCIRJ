#include <stdio.h>
#include <omp.h>
int main(void) {
    #pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();
        #pragma omp critical
        printf("Thread %d de %d\n", id, omp_get_num_threads());
    }
    return 0;
}
