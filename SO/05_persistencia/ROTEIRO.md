# Persistência em Sistemas Operacionais — roteiro dos slides

Apresentação nova. Numeração inicia no slide 1.

- **Slide 1:** Persistência em Sistemas Operacionais
- **Slide 2:** Roteiro das aulas
- **Slide 3:** Do processo ao arquivo
- **Slide 4:** Pergunta inicial: o que sobrevive?
- **Slide 5:** Persistência: definição operacional
- **Slide 6:** RAM e armazenamento
- **Slide 7:** O caminho de uma chamada de escrita
- **Slide 8:** O que o sistema operacional abstrai?
- **Slide 9:** Arquitetura simplificada de E/S
- **Slide 10:** Polling e interrupções
- **Slide 11:** DMA: por que faz diferença?
- **Slide 12:** HDD e SSD: mecanismos físicos
- **Slide 13:** Custos de acesso a um HDD
- **Slide 14:** Por que o SSD também precisa de gerenciamento?
- **Slide 15:** Teste rápido: E/S e armazenamento
- **Slide 16:** Arquivo: uma sequência de bytes
- **Slide 17:** Descritores de arquivos
- **Slide 18:** O descritor não é o nome do arquivo
- **Slide 19:** Principais chamadas POSIX
- **Slide 20:** Flags de abertura: decisões do processo
- **Slide 21:** Código 01: gravar e ler em processos diferentes
- **Slide 22:** Persistência após o término do processo  ← execução no terminal
- **Slide 23:** Interprete a primeira demonstração
- **Slide 24:** Posição de leitura/escrita: deslocamento
- **Slide 25:** Código 02: substituindo um byte
- **Slide 26:** Controle da posição com lseek  ← execução no terminal
- **Slide 27:** Diretórios: nomes e hierarquia
- **Slide 28:** Terminal: explorando diretórios e arquivos
- **Slide 29:** Permissões de arquivo e de diretório
- **Slide 30:** Fechamento da Aula 1
- **Slide 31:** Por dentro do sistema de arquivos
- **Slide 32:** Retomada da Aula 1
- **Slide 33:** Sistemas de arquivos: responsabilidades
- **Slide 34:** Uma visão simplificada de disco
- **Slide 35:** Bloco e metadados
- **Slide 36:** Nome, inode e blocos
- **Slide 37:** O que guarda um inode?
- **Slide 38:** Links físicos e simbólicos
- **Slide 39:** Código 03: stat e lstat
- **Slide 40:** Metadados, hard links e links simbólicos  ← execução no terminal
- **Slide 41:** Interprete inodes e links
- **Slide 42:** Alocação e blocos livres
- **Slide 43:** Leitura e escrita: passos essenciais
- **Slide 44:** Localidade e fragmentação
- **Slide 45:** Page cache: por que existe?
- **Slide 46:** write, cache e dispositivo
- **Slide 47:** write, close, fsync: papéis diferentes
- **Slide 48:** Código 04: usando fsync
- **Slide 49:** Sincronização explícita com fsync  ← execução no terminal
- **Slide 50:** A hipótese do fsync
- **Slide 51:** Consistência após falhas: o problema
- **Slide 52:** Experimento mental: criar um arquivo
- **Slide 53:** fsck: verificação e reparo
- **Slide 54:** Journaling: ideia principal
- **Slide 55:** O princípio de uma transação em journal
- **Slide 56:** Consistência não é sinônimo de backup
- **Slide 57:** ext4: exemplo de sistema de arquivos do Linux
- **Slide 58:** Terminal: observar o sistema de arquivos
- **Slide 59:** Integridade e proteção
- **Slide 60:** RAID: por que combinar discos?
- **Slide 61:** Comparação: três camadas de proteção
- **Slide 62:** Síntese final: persistência
- **Slide 63:** Bibliografia e leitura orientada — OSTEP 1.10
- **Slide 64:** Repositório e próximos passos


## Planejamento de aula

**Aula 1, slides 1–30 (3 horas):** conceito de persistência (20 min); arquitetura de E/S, HDD e SSD (60 min); interface POSIX de arquivos e demonstrações 01 e 02 (70 min); diretórios, permissões e revisão (30 min).

**Aula 2, slides 31–64 (3 horas):** organização interna, inodes e demonstração 03 (60 min); blocos, cache e demonstração 04 (40 min); crash consistency, fsck e journaling (45 min); ext4, RAID, integridade e discussão final (35 min).

Se o ritmo da turma exigir, os slides de RAID e SSD podem ser tratados como leitura complementar. Os quatro programas não dependem de modificações para funcionar.

## Referência de execução no terminal

1. **Slide 22 — `01_arquivo.c`:** duas execuções de `gravar`, intercaladas com `ler`, para mostrar que os dados sobrevivem ao encerramento dos processos.
2. **Slide 26 — `02_lseek.c`:** escreve 10 bytes, altera a quinta posição e verifica o resultado.
3. **Slide 40 — `03_metadados.c`:** cria hard link e link simbólico, compara os inodes com `stat` e `lstat`.
4. **Slide 49 — `04_fsync.c`:** executa `write`, `fsync` e `close` em sequência; explica a diferença entre cache e armazenamento.

**Em todos os casos:** entrar primeiro em `SO/05_persistencia/exemplos`, executar `make`, e depois os comandos exibidos nos slides de demonstração.
