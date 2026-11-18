# Windows — experimento de bloqueio de arquivo com LockFileEx()

Programa nativo Win32 em C. **Nao execute pelo WSL**, que usa o kernel Linux.

## Requisitos
- Windows 10 ou 11.
- MinGW-w64 (`gcc`) OU Visual Studio C/C++ Build Tools (`cl`).
- Execute os testes num diretorio local com A.txt e B.txt presentes.

## Opcao 1: MinGW-w64

No terminal de compilacao com `gcc` disponivel:

```bat
compilar_mingw.bat
01_lockfileex_threads.exe
```

Se GNU Make estiver disponivel: `make` e `make demo`.

## Opcao 2: Visual Studio (MSVC)

No **Developer Command Prompt for VS**:

```bat
compilar_msvc.bat
01_lockfileex_threads.exe
```

## Investigar

- Qual o papel de `GENERIC_READ | GENERIC_WRITE`?
- Por que `FILE_SHARE_READ | FILE_SHARE_WRITE` aparece em `CreateFileA()`?
- O que significa `LOCKFILE_EXCLUSIVE_LOCK`?
- Qual intervalo de bytes e bloqueado? Alterar o intervalo mudaria o teste?
- Quando as threads solicitam os segundos bloqueios, o que se observa?
- Se uma chamada falhar, anote o valor numerico de `GetLastError()` e investigue.

Se o processo nao terminar, use **Ctrl+C** no terminal. Os arquivos nao serao
modificados. Nao mantenha outras copias do experimento executando sobre a mesma
pasta durante os testes.

Referencias da Microsoft: `CreateFileA`, `LockFileEx`, `UnlockFileEx`,
`CreateThread`, `GetLastError`.
