#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <omp.h>
#define CLIENTES 10
#define CADEIRAS 3
sem_t mutex, chegadas, chamado[CLIENTES], concluido[CLIENTES];
int fila[CADEIRAS], inicio=0, fim=0, esperando=0, restantes=CLIENTES;
void cliente(int id) {
    usleep(12000*(id%5));
    sem_wait(&mutex);
    if(esperando==CADEIRAS) {
        printf("Cliente %d foi embora (sem cadeira)\n",id);
        restantes--;
        int ultimo=(restantes==0);
        sem_post(&mutex);
        if(ultimo) sem_post(&chegadas);
        return;
    }
    fila[fim]=id;
    fim=(fim+1)%CADEIRAS;
    esperando++;
    printf("Cliente %d sentou na espera (%d/%d)\n",id,esperando,CADEIRAS);
    sem_post(&mutex);
    sem_post(&chegadas);
    sem_wait(&chamado[id]);
    sem_wait(&concluido[id]);
    printf("Cliente %d terminou atendimento\n",id);
    sem_wait(&mutex);
    restantes--;
    int ultimo=(restantes==0);
    sem_post(&mutex);
    if(ultimo) sem_post(&chegadas);
}
void barbeiro(void) {
    for(;;) {
        sem_wait(&chegadas);
        sem_wait(&mutex);
        if(esperando==0 && restantes==0) {
            sem_post(&mutex);
            break;
        }
        if(esperando==0) { sem_post(&mutex); continue; }
        int id=fila[inicio];
        inicio=(inicio+1)%CADEIRAS;
        esperando--;
        sem_post(&mutex);
        printf("Barbeiro chamou cliente %d\n",id);
        sem_post(&chamado[id]);
        usleep(45000);
        sem_post(&concluido[id]);
    }
    puts("Barbeiro encerrou o expediente.");
}
int main(void) {
    sem_init(&mutex,0,1); sem_init(&chegadas,0,0);
    for(int i=0;i<CLIENTES;i++) {
        sem_init(&chamado[i],0,0);
        sem_init(&concluido[i],0,0);
    }
    #pragma omp parallel num_threads(CLIENTES+1)
    {
        int id=omp_get_thread_num();
        if(id==0) barbeiro(); else cliente(id-1);
    }
    for(int i=0;i<CLIENTES;i++) {
        sem_destroy(&chamado[i]); sem_destroy(&concluido[i]);
    }
    sem_destroy(&mutex); sem_destroy(&chegadas);
    return 0;
}
