# Laboratório: vetorização automática com GCC

**Disciplina:** Arquitetura Avançada de Computadores

Este laboratório compara dois algoritmos em C: um com operações independentes,
que podem ser vetorizadas, e outro com dependências entre iterações.

## Conteúdo

- `codigo/funcoes.c`: funções utilizadas nos experimentos.
- `codigo/principal.c`: inicialização, verificação dos resultados e medição de tempo.
- `Makefile`: compilação escalar, SIMD, geração de Assembly e relatórios do GCC.

## Requisitos

Ubuntu/Linux x86-64 com GCC e GNU Make:

```bash
sudo apt update
sudo apt install build-essential
```

## Comandos

Na pasta `Vetorizacao`:

```bash
make ajuda      # lista os comandos
make all        # compila versões escalar e vetorizada
make executar   # executa as duas versões
make medir      # repete medições e salva relatorios/tempos.txt
make asm        # gera assembly/funcoes_escalar.s e funcoes_vetor.s
make relatorio  # relatório de vetorização em relatorios/vetorizacao.txt
```

O perfil principal compara dois builds em `-O3` com a vetorização desligada ou
ligada. A base utiliza SSE (128 bits), o que permite comparar instruções como
`addss`/`mulss` (escalares) com `addps`/`mulps` (empacotadas).

**Atenção:** registradores `xmm` aparecem em instruções escalares e vetoriais:
observe também as instruções. O experimento AVX2 é opcional (`make avx2`), e
sua execução depende do suporte do processador (`make executar-avx2`).

Para alterar parâmetros:

```bash
make clean
make all TAMANHO=2000000 REPETICOES=150
make medir
```

Não envie binários nem resultados gerados ao GitHub; eles são reproduzíveis com
os comandos acima. O roteiro completo da atividade está disponível no Teams.
