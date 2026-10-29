#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <omp.h>
#include <string.h>
#define CAP 3
static int buffer[CAP] = {10,20,30};
static int entrada=0, saida=0;
static sem_t vazios, cheios, mutex, pronto;
static int errado;
static void produtor(void) {
    if (errado) {
        sem_wait(&mutex);  /* ERRO: mutex antes de verificar vaga */
        sem_post(&pronto); /* avisa que o erro ja foi cometido */
    }
    sem_wait(&vazios);
    if (!errado) sem_wait(&mutex);
    buffer[entrada]=40;
    entrada=(entrada+1)%CAP;
    puts("Produtor colocou 40");
    sem_post(&mutex);
    sem_post(&cheios);
}
static void consumidor(void) {
    if (errado) sem_wait(&pronto);
    sem_wait(&cheios);
    sem_wait(&mutex);
    int item=buffer[saida];
    saida=(saida+1)%CAP;
    printf("Consumidor retirou %d\n",item);
    sem_post(&mutex);
    sem_post(&vazios);
}
int main(int argc,char **argv) {
    errado=(argc==2 && strcmp(argv[1],"errado")==0);
    if(argc>2 || (argc==2 && !errado)) {fprintf(stderr,"Uso: %s [errado]\n",argv[0]);return 2;}
    /* Buffer inicialmente cheio: cheios=3, vazios=0 */
    sem_init(&vazios,0,0);sem_init(&cheios,0,CAP);sem_init(&mutex,0,1);sem_init(&pronto,0,0);
    printf("Buffer inicial cheio. Modo: %s\n",errado?"INCORRETO":"CORRETO");fflush(stdout);
    #pragma omp parallel num_threads(2)
    {
        if (omp_get_thread_num()==0) {
            /* Garante que o produtor tente primeiro. */
            produtor();
        } else {
            consumidor();
        }
    }
    sem_destroy(&vazios);sem_destroy(&cheios);sem_destroy(&mutex);sem_destroy(&pronto);
    return 0;
}
