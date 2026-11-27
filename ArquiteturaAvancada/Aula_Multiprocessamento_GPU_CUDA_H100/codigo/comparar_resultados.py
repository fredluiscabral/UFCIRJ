#!/usr/bin/env python3
"""Junta CSV OpenMP (mediana) e CUDA (1 leitura) e calcula speedups didaticos.
Uso: python3 comparar_resultados.py JOBID
"""
import csv
import sys
from pathlib import Path


def ler_csv(caminho):
    with caminho.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def main():
    if len(sys.argv) != 2 or not sys.argv[1].isdigit():
        sys.exit("Uso: python3 comparar_resultados.py JOBID")
    job = sys.argv[1]
    omps = ler_csv(Path(f"resultados_openmp_{job}.csv"))
    cuda = {int(v["matriz"]): v for v in ler_csv(Path(f"resultados_cuda_{job}.csv"))}
    baseline = {int(v["matriz"]): float(v["mediana_ms"]) for v in omps
                if int(v["threads"]) == 1}
    output = Path(f"comparacao_openmp_cuda_{job}.csv")
    linhas = []
    for r in omps:
        n, t = int(r["matriz"]), int(r["threads"])
        if n not in baseline or n not in cuda:
            continue
        c = cuda[n]
        mediana = float(r["mediana_ms"])
        kernel = float(c["gpu_kernel_ms"])
        total = float(c["gpu_total_ms"])
        valido = r["validacao"] == "OK" and c["validacao"] == "OK"
        linhas.append({
            "N": n, "threads_omp": t,
            "omp_mediana_ms": f"{mediana:.6f}",
            "cuda_kernel_ms": f"{kernel:.6f}",
            "cuda_total_ms": f"{total:.6f}",
            "speedup_omp_vs_1thread": f"{baseline[n] / mediana:.3f}" if mediana > 0 else "",
            "speedup_cuda_kernel_vs_omp": f"{mediana / kernel:.3f}" if kernel > 0 else "",
            "speedup_cuda_total_vs_omp": f"{mediana / total:.3f}" if total > 0 else "",
            "validacao": "OK" if valido else "FALHOU",
        })
    if not linhas:
        sys.exit("Nao foi possivel juntar os CSVs. Confira os arquivos.")
    with output.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=list(linhas[0]))
        w.writeheader()
        w.writerows(linhas)
    print(f"Comparacao salva: {output}")
    print("Matriz  Threads   OpenMP mediana(ms)  GPU kernel(ms)  GPU total(ms)  CUDA total / OpenMP")
    for n in [256, 512, 1024]:
        candidatos = [r for r in linhas if r["N"] == n]
        if not candidatos:
            continue
        melhor_t = max(int(r["threads_omp"]) for r in candidatos)
        r = next(x for x in candidatos if int(x["threads_omp"]) == melhor_t)
        # Razao GPU total vs OpenMP: <1 significa GPU mais rapida; >1 OpenMP mais rapido.
        razao = float(r["cuda_total_ms"]) / float(r["omp_mediana_ms"])
        print(f"{n:6d} {melhor_t:8d} {r['omp_mediana_ms']:18} "
              f"{r['cuda_kernel_ms']:15} {r['cuda_total_ms']:14} {razao:18.3f}x")
    print("OBS: 4x4 nao possui cronometragem confiavel para calcular speedups.")
    print("OBS: OpenMP usa 5 repeticoes; CUDA original faz 1 leitura por tamanho.")


if __name__ == "__main__":
    main()
