# Sistemas Operacionais (SO) | UFCIRJ / CEFET-RJ

Material didático de **Sistemas Operacionais**, com foco inicial em **sincronização de processos e threads**.

Repositório previsto: `UFCIRJ/SO`.

## Estrutura

```text
SO/
├── README.md
├── .gitignore
├── slides/
│   ├── main.tex            # fonte Beamer (Overleaf)
│   ├── main.pdf            # PDF compilado
│   └── Makefile
├── exemplos/
│   ├── produtor_consumidor_basico/
│   ├── produtor_consumidor_diagnostico/
│   ├── produtor_consumidor_visual/
│   └── openmp/
└── docs/
    └── NOTAS.md
```

## Slides

Para editar online, crie um projeto no [Overleaf](https://www.overleaf.com/) e carregue `slides/main.tex`. Compilador: **pdfLaTeX**.

Localmente (TeX Live com Beamer, Babel português, TikZ e listings):

```bash
cd slides
make
```

## Exemplos em C

Requerem **GCC**, `make` e Linux/POSIX para os exemplos com pthreads. Entre na pasta do exemplo e execute:

```bash
make
./produtor_consumidor
```

Exemplos OpenMP (GCC com suporte a OpenMP):

```bash
cd exemplos/openmp
make
./01_threads
./02_corrida
./03_critical
./04_atomic
./05_reduction
```

O exemplo `02_corrida.c` e os exemplos de produtor-consumidor são **intencionalmente incorretos**, para discussão em sala de aula. Eles podem apresentar *data races* e comportamento indefinido; não devem ser usados como implementação de produção. Nem toda execução exibirá um erro.

## Publicar no GitHub

Depois de criar o repositório vazio `UFCIRJ/SO` (caso tenha permissões na organização UFCIRJ), na pasta `SO`:

```bash
git init
git add .
git commit -m "Adiciona aula de sincronização e exemplos em C"
git branch -M main
git remote add origin https://github.com/UFCIRJ/SO.git
git push -u origin main
```

Se a organização/repositório ainda não existir, crie-o antes do `git push`. Este pacote **não foi publicado automaticamente**.

## Estado do material

O PDF foi gerado a partir do `main.tex` incluído no pacote. Os exemplos de OpenMP são versões executáveis dos trechos discutidos na apresentação. Consulte `docs/NOTAS.md` para limitações didáticas e de formatação.
