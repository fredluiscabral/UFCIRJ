# Complemento da atividade — macOS e Windows

Os programas desta pasta complementam os experimentos da atividade de Linux.

- **MacOS/01_flock_threads.c:** duas threads, `flock()`, dois arquivos.
- **MacOS/02_fcntl_processos.c:** dois processos, `fcntl(F_SETLKW)`, dois arquivos.
- **Windows/01_lockfileex_threads.c:** duas threads, API Win32 `LockFileEx()`, dois arquivos.

Cada subpasta inclui `A.txt`, `B.txt`, Makefile e orientacoes de compilacao.
No Windows, tambem existem scripts de compilacao para MinGW-w64 e MSVC.

Os exemplos solicitam os bloqueios em ordem inversa e fazem uma espera de
10 segundos entre o primeiro e o segundo bloqueio. A abertura com permissoes
de leitura e escrita nao equivale a bloquear um arquivo.

Observe e registre o comportamento de cada sistema sem presumir que APIs
parecidas oferecam a mesma deteccao ou recuperacao de impasses.

Estes programas nao reescrevem os arquivos, mas mantenha uma copia do material
e execute em pastas locais de teste. Interrompa com Ctrl+C se necessario.

Nao ha executavel pronto no pacote: os programas devem ser compilados na
plataforma alvo. Os arquivos-fonte POSIX foram compilados e testados no Linux
como verificacao inicial; o teste nativo de Windows e macOS deve ser feito pelos
alunos nos respectivos sistemas.
