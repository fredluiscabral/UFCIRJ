#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <omp.h>
#define LEITORES 4
#define ESCRITAS 5
sem_t mutex_contagem, recurso;
int leitores_ativos = 0, dado = 0;
void leitor(int id) {
    for (int k=0;k<5;k++) {
        sem_wait(&mutex_contagem);
        leitores_ativos++;
        if (leitores_ativos==1) sem_wait(&recurso);
        int presentes = leitores_ativos;
        sem_post(&mutex_contagem);
        printf("Leitor %d leu %d (leitores na entrada: %d)\n", id, dado, presentes);
        usleep(20000);
        sem_wait(&mutex_contagem);
        leitores_ativos--;
        if (leitores_ativos==0) sem_post(&recurso);
        sem_post(&mutex_contagem);
        usleep(8000);
    }
}
void escritor(void) {
    for (int k=1;k<=ESCRITAS;k++) {
        sem_wait(&recurso);
        dado=k;
        printf("ESCRITOR gravou %d\n", dado);
        usleep(30000);
        sem_post(&recurso);
        usleep(12000);
    }
}
int main(void) {
    sem_init(&mutex_contagem,0,1); sem_init(&recurso,0,1);
    #pragma omp parallel num_threads(LEITORES+1)
    {
        int id=omp_get_thread_num();
        if (id==0) escritor(); else leitor(id);
    }
    sem_destroy(&mutex_contagem); sem_destroy(&recurso);
    puts("Fim. Politica: preferencia aos leitores; escritor pode sofrer inanicao.");
    return 0;
}
