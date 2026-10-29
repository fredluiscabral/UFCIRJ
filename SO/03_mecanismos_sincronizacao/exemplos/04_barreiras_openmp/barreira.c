#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <omp.h>
int main(void) {
    #pragma omp parallel num_threads(4)
    {
        int id=omp_get_thread_num();
        usleep((unsigned int)(30000*(3-id)));
        printf("Thread %d concluiu fase 1\n",id);
        fflush(stdout);
        #pragma omp barrier
        printf("Thread %d iniciou fase 2\n",id);
    }
    return 0;
}
