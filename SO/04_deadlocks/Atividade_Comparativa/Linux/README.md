# Linux — programas da atividade comparativa

Compilar e executar:

```sh
make
make teste_flock
make teste_fcntl
```

Códigos:
- `01_flock_threads.c`: duas threads e `flock()`.
- `02_fcntl_processos.c`: duas execuções independentes com `fcntl(F_SETLKW)`.

Consulte `../atividade_comparativa.pdf` antes de executar. O encerramento
pelo `timeout` não deve ser interpretado como resposta do kernel. Teste em
um diretório local e encerre eventuais processos remanescentes antes de repetir.
