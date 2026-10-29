# Aula 3 — Mecanismos de sincronização

Exemplos para Linux (Ubuntu), com GCC, OpenMP, POSIX Threads e Java.

## Conteúdo

| Diretório | Mecanismo | Execução |
|---|---|---|
| `01_deadlock_produtor_consumidor` | Ordem dos semáforos | `make && make correto && make errado` |
| `02_spinlock` | Operação atômica / spinlock | `make && ./spinlock` |
| `03_mutex_openmp` | Mutex POSIX com OpenMP | `make && ./mutex` |
| `04_barreiras_openmp` | Barreira OpenMP | `make && ./barreira` |
| `05_monitores_java` | Monitor Java e `wait/notifyAll` | `make run` |
| `06_processos_independentes` | `mmap` e semáforo entre processos | `make && make demo` |

## Observações

* No exemplo 01, o buffer começa cheio, e a versão `errado` **provoca deadlock deliberadamente**. `make errado` usa `timeout` para encerrar essa execução após 2 segundos. Não execute `./buffer errado` sem conhecer essa limitação.
* O exemplo 01 mantém índices **distintos** (`entrada` e `saida`): o deadlock vem da ordem de aquisição dos semáforos, não de compartilhar o índice.
* No exemplo 06, os processos usam o mesmo objeto `sem_t` dentro de uma área criada por `shm_open`/`mmap(MAP_SHARED)` e inicializado com `pshared=1`. **Não rode `./limpar` enquanto houver incrementadores ativos.**
* O exemplo 05 demonstra um monitor usando `synchronized` e variáveis de condição do objeto Java; `notifyAll` não transfere imediatamente a posse do monitor.
* Exemplos didáticos mínimos; sistemas de produção precisam de tratamento de falhas e ciclo de vida mais robustos.

### Compilação

Em cada pasta, use `make`. Para remover binários, `make clean`.

### Referências

OSTEP: capítulos 28, 30 e 31; documentação do OpenMP, POSIX e Java.
