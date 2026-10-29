#include "comum.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void) {
    int fd=shm_open(NOME_SHM,O_RDWR,0);
    if(fd<0){perror("shm_open (execute ./iniciar primeiro)");return 1;}
    struct Estado *e=mmap(NULL,sizeof(*e),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    if(e==MAP_FAILED){perror("mmap");return 1;}
    for(int i=0;i<100000;i++) {
        sem_wait(&e->mutex);
        e->contador++;
        sem_post(&e->mutex);
    }
    puts("Processo terminou 100000 incrementos");
    munmap(e,sizeof(*e));
    return 0;
}
