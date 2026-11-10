# Roteiro de demonstrações - Aula 4: Deadlocks

A apresentação completa contém **150 slides**, sendo **111 anteriores e 39 novos**.

Abra um terminal Linux na pasta `SO/04_deadlocks/exemplos` e compile:

```bash
make
```

| Slide | Assunto | Comando | Observação |
|---:|---|---|---|
| **124** | Buffer circular de 5 posições | `timeout 7s ./01_produtor_consumidor correto` | Continua produzindo e consumindo |
| **124** | Mesmo buffer, deadlock | `timeout 5s ./01_produtor_consumidor errado` | Buffer inicialmente cheio; produtor segura mutex |
| **127** | Mutexes em ordens opostas | `timeout 5s ./02_mutexes errado` | Espera circular |
| **127** | Mutexes na mesma ordem | `timeout 9s ./02_mutexes correto` | Progresso contínuo |
| **133** | Filósofos sem prevenção | `timeout 5s ./03_filosofos errado` | Todos podem segurar um garfo e esperar pelo outro |
| **133** | Filósofos com `sala=N-1` | `timeout 12s ./03_filosofos correto` | Sem deadlock nessa implementação |
| **143** | Algoritmo do banqueiro | `./04_banqueiro` | Testes de segurança repetidos; pare com Ctrl+C |

### Atenção

Todos os quatro códigos contêm `while (true)` e são pensados para demonstração contínua.
`timeout` evita que uma execução bloqueante prenda o terminal. Seu código de saída 124
é normal quando o tempo se esgota. As pausas `sleep(1)` tornam o intercalamento
mais visível, mas não garantem um resultado idêntico em todo computador.

No algoritmo do banqueiro, **o teste de segurança precisa terminar** para
verificar se todos os processos podem concluir; é o `main` que repete esse
teste infinitamente para fins de demonstração.

Os arquivos `.tex` e `.pdf` em `slides/` preservam os 111 slides anteriores e
acrescentam o capítulo de deadlocks ao final, sem criar uma apresentação separada.
