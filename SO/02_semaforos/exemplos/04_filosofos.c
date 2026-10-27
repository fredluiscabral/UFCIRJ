#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <omp.h>
#define N 5
sem_t garfos[N], sala;
int main(void) {
    for(int i=0;i<N;i++) sem_init(&garfos[i],0,1);
    sem_init(&sala,0,N-1);
    #pragma omp parallel num_threads(N)
    {
        int i=omp_get_thread_num();
        for(int rodada=1;rodada<=3;rodada++) {
            printf("Filosofo %d pensando\n",i);
            usleep(15000*(i+1));
            sem_wait(&sala);
            sem_wait(&garfos[i]);
            sem_wait(&garfos[(i+1)%N]);
            printf("Filosofo %d COMENDO (rodada %d)\n",i,rodada);
            usleep(20000);
            sem_post(&garfos[(i+1)%N]);
            sem_post(&garfos[i]);
            sem_post(&sala);
        }
    }
    for(int i=0;i<N;i++) sem_destroy(&garfos[i]);
    sem_destroy(&sala);
    puts("Todos terminaram; limite de quatro evita deadlock circular.");
    return 0;
}
