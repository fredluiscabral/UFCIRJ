# Sistemas Operacionais

**CEFET/RJ | Graduação em Ciência da Computação**

Este diretório reúne slides e programas utilizados nas aulas de **Sistemas Operacionais**. O material será atualizado ao longo do período letivo.

## Material de aula

- **[Slides: sincronização de processos e threads (PDF)](slides/main.pdf)**
- [Código-fonte dos slides (LaTeX/Beamer)](slides/main.tex)

## Exemplos de programação

Os exemplos são escritos em **C** e ilustram conceitos discutidos em sala:

| Pasta | Conteúdo |
| --- | --- |
| [`exemplos/produtor_consumidor_basico/`](exemplos/produtor_consumidor_basico/) | Buffer compartilhado e concorrência entre produtor e consumidor. |
| [`exemplos/produtor_consumidor_diagnostico/`](exemplos/produtor_consumidor_diagnostico/) | Versão experimental para observar resultados inesperados. |
| [`exemplos/produtor_consumidor_visual/`](exemplos/produtor_consumidor_visual/) | Visualização do conteúdo do buffer durante a execução. |
| [`exemplos/openmp/`](exemplos/openmp/) | Exemplos com `parallel`, condição de corrida, `critical`, `atomic` e `reduction`. |

### Como compilar e executar

Em um ambiente Linux com **GCC** e **make**, abra o terminal na pasta do exemplo. Para os programas de produtor-consumidor:

```bash
cd exemplos/produtor_consumidor_visual
make
./produtor_consumidor
```

Para os exemplos de OpenMP:

```bash
cd exemplos/openmp
make
./01_threads
./02_corrida
./03_critical
./04_atomic
./05_reduction
```

Os caminhos acima são relativos à pasta `SO` deste repositório. Para limpar arquivos gerados na compilação, use `make clean` quando disponível.

## Atenção aos exemplos sem sincronização

Alguns programas contêm **erros intencionais de concorrência**, para fins didáticos. Eles podem perder dados, apresentar resultados inesperados ou aparentar funcionar corretamente, dependendo da execução. Programas com acessos concorrentes conflitantes sem sincronização podem ter **comportamento indefinido em C**.

**Não utilize esses exemplos como implementações prontas para aplicações reais.** O objetivo é analisar os problemas e discutir as soluções apresentadas nas aulas.

## Referências

- Remzi H. Arpaci-Dusseau e Andrea C. Arpaci-Dusseau. *Operating Systems: Three Easy Pieces (OSTEP)*: https://pages.cs.wisc.edu/~remzi/OSTEP/
- Especificações do OpenMP: https://www.openmp.org/specifications/
