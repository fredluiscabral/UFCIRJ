#include "comum.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void) {
    int fd=shm_open(NOME_SHM,O_CREAT|O_EXCL|O_RDWR,0600);
    if(fd<0){perror("shm_open (ja existe? execute ./limpar)");return 1;}
    if(ftruncate(fd,sizeof(struct Estado))<0){perror("ftruncate");close(fd);return 1;}
    struct Estado *e=mmap(NULL,sizeof(*e),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    if(e==MAP_FAILED){perror("mmap");return 1;}
    e->contador=0;
    if(sem_init(&e->mutex,1,1)<0){perror("sem_init");return 1;}
    puts("Memoria e semaforo criados: contador=0");
    munmap(e,sizeof(*e));
    return 0;
}
