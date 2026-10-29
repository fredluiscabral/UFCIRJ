#ifndef COMUM_H
#define COMUM_H
#include <semaphore.h>
#define NOME_SHM "/so3_contador_demo_v1"
struct Estado {sem_t mutex; int contador;};
#endif
