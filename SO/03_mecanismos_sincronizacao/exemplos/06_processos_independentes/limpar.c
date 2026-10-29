#include "comum.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void) {
    int fd=shm_open(NOME_SHM,O_RDWR,0);
    if(fd<0){perror("shm_open");return 1;}
    struct Estado *e=mmap(NULL,sizeof(*e),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    if(e==MAP_FAILED){perror("mmap");return 1;}
    /* Executar apenas quando nenhum outro processo estiver usando! */
    sem_destroy(&e->mutex);
    munmap(e,sizeof(*e));
    if(shm_unlink(NOME_SHM)<0){perror("shm_unlink");return 1;}
    puts("Memoria compartilhada removida");
    return 0;
}
