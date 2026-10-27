# Sistemas Operacionais

**CEFET/RJ — Bacharelado em Ciência da Computação**

**Professor:** Frederico Luís Cabral

Este diretório reúne os materiais da disciplina de Sistemas
Operacionais, incluindo apresentações, programas em C e
atividades práticas.

## Unidade 1 — Concorrência e Exclusão Mútua

Conteúdos:

- Processos e threads
- Condições de corrida
- Regiões críticas
- Exclusão mútua
- Alternância estrita
- Algoritmo de Peterson
- OpenMP: critical, atomic e reduction

[**Acessar a Unidade 1**](01_concorrencia_exclusao_mutua/)

[Slides em PDF](01_concorrencia_exclusao_mutua/slides/Sistemas_Operacionais_sincronizacao_parte1.pdf)

## Unidade 2 — Semáforos e Problemas Clássicos

Conteúdos:

- Semáforos binários e contadores
- Operações wait e signal
- Semáforos POSIX
- OpenMP e semáforos
- Produtor-consumidor
- Leitores e escritores
- Filósofos jantando
- Barbeiro dorminhoco

[**Acessar a Unidade 2**](02_semaforos/)

[Slides em PDF](02_semaforos/slides/apresentacao.pdf)

### Exemplos em C

Os programas utilizam OpenMP e semáforos POSIX.

Para compilar:

    cd SO/02_semaforos/exemplos
    make

Para executar:

    ./01_contador_semaforo
    ./02_produtor_consumidor_openmp_posix
    ./03_leitores_escritores
    ./04_filosofos
    ./05_barbeiro

## Atividades

[**Acessar atividades práticas**](atividades/)

## Referências

- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)
- [OpenMP](https://www.openmp.org/)
