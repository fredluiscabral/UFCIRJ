# Aula 4 - Deadlocks: exemplos em C

Códigos didáticos simples para o 2º período, em Linux. Todos usam `while (true)`
(com `<stdbool.h>`); encerram com Ctrl+C. Os que entram em deadlock **não se
recuperam sozinhos**. Execute-os com `timeout`.

## Compilar

```bash
make
```

## Executar

```bash
# Produtor-consumidor: buffer circular de N=5 posições, inicialmente cheio
./01_produtor_consumidor correto             # Ctrl+C para interromper
timeout 5s ./01_produtor_consumidor errado    # deadlock intencional

# Duas threads e dois mutexes
timeout 9s ./02_mutexes correto
timeout 5s ./02_mutexes errado

# Cinco filósofos e cinco garfos
timeout 12s ./03_filosofos correto
timeout 5s ./03_filosofos errado

# Banco de 10 recursos, três processos; simulação repetida indefinidamente
./04_banqueiro                             # Ctrl+C para interromper
```

`timeout` normalmente retorna código de saída 124 ao atingir o limite: **isso
não significa que ocorreu erro de compilação**. O modo `errado` de mutexes e
filósofos usa uma pausa para tornar o deadlock fácil de reproduzir; a ordem
exata das threads depende do escalonador, então não há garantia formal de que
ocorra em toda execução.

## Resumo

| Arquivo | Ponto da aula | Demonstração |
|---|---|---|
| `01_produtor_consumidor.c` | Introdução e condições | Ordem errada dos semáforos, com N=5 |
| `02_mutexes.c` | Prevenção | Duas threads, aquisição em ordens opostas vs iguais |
| `03_filosofos.c` | Prevenção | Espera circular vs `sala=N-1` |
| `04_banqueiro.c` | Evitação | Estado seguro vs concessão que torna o estado inseguro |

## Observações didáticas

- Os programas de concorrência executam sem fim no modo correto. Não há menus,
  arquivos de entrada, sinalizadores de parada nem logs sofisticados.
- O buffer circular usa índices `entrada` e `saida` e semáforos `vazios`,
  `cheios` e `mutex`. Começa cheio **para que o deadlock apareça logo**.
- A pausa `sleep(1)` é apenas para acompanhar o comportamento no terminal;
  ela não é um mecanismo de sincronização.
- O banqueiro não precisa de threads; o teste de segurança é finito, porém o
  `main` o repete com `while (true)` para apresentação contínua.
- A prevenção de deadlock não garante ausência de inanição.
