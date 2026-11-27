// Experimento didatico: influencia da geometria dos blocos CUDA na NVIDIA H100.
// Mesmo algoritmo e mesmos dados usados em matriz_h100.cu (UFCI/RJ).
// Cada thread calcula um unico elemento C[linha, coluna].
// Compilar: nvcc -O3 -arch=sm_90 -std=c++14 -o matriz_blocos_h100 matriz_blocos_h100.cu
// Executar: ./matriz_blocos_h100 1024 30 [arquivo.csv]

#include <cuda_runtime.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#define CUDA_OK(expr) do {                                      \
    cudaError_t err = (expr);                                  \
    if (err != cudaSuccess) {                                  \
        std::fprintf(stderr, "Erro CUDA em %s:%d: %s\n",     \
            __FILE__, __LINE__, cudaGetErrorString(err));       \
        std::exit(EXIT_FAILURE);                                \
    }                                                           \
} while (0)

// Identico ao kernel original da aula (sem tiling, sem Tensor Cores).
__global__ void multiplicar_gpu(const float *A, const float *B,
                                float *C, int n) {
    int linha  = blockIdx.y * blockDim.y + threadIdx.y;
    int coluna = blockIdx.x * blockDim.x + threadIdx.x;
    if (linha < n && coluna < n) {
        float soma = 0.0f;
        for (int k = 0; k < n; ++k) {
            soma += A[linha * n + k] * B[k * n + coluna];
        }
        C[linha * n + coluna] = soma;
    }
}

static void multiplicar_cpu(const std::vector<float>& A,
                           const std::vector<float>& B,
                           std::vector<float>& C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float soma = 0.0f;
            for (int k = 0; k < n; ++k) {
                soma += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = soma;
        }
    }
}

struct Configuracao { int bx, by; };

int main(int argc, char **argv) {
    const int n = (argc >= 2) ? std::atoi(argv[1]) : 1024;
    const int repeticoes = (argc >= 3) ? std::atoi(argv[2]) : 30;
    const int aquecimentos = 5;
    const char *caminho_csv = (argc >= 4) ? argv[3] : nullptr;
    if (n <= 0 || n > 2048 || repeticoes < 3 || repeticoes > 10000) {
        std::fprintf(stderr, "Uso: %s [N=1024, ate 2048] [repeticoes=30, min. 3] [saida.csv]\n", argv[0]);
        return 1;
    }

    const size_t elementos = static_cast<size_t>(n) * n;
    const size_t bytes = elementos * sizeof(float);
    std::vector<float> A(elementos), B(elementos), C_cpu(elementos), C_gpu(elementos);
    for (size_t i = 0; i < elementos; ++i) {
        A[i] = (n == 4) ? static_cast<float>(i + 1) : static_cast<float>((i % 7) + 1);
        B[i] = (n == 4) ? 1.0f : static_cast<float>((i % 5) + 1);
    }

    // Referencia sequencial calculada uma unica vez, fora das medicoes CUDA.
    multiplicar_cpu(A, B, C_cpu, n);

    int ngpus = 0;
    CUDA_OK(cudaGetDeviceCount(&ngpus));
    if (ngpus < 1) {
        std::fprintf(stderr, "Nenhuma GPU CUDA foi alocada. Execute via Slurm.\n");
        return 2;
    }
    cudaDeviceProp device{};
    CUDA_OK(cudaGetDeviceProperties(&device, 0));
    std::printf("GPU: %s | compute capability %d.%d\n", device.name, device.major, device.minor);
    std::printf("Matriz: %dx%d | 1 grid por lancamento | %d aquecimentos | %d medicoes por configuracao\n\n",
                n, n, aquecimentos, repeticoes);

    float *d_A = nullptr, *d_B = nullptr, *d_C = nullptr;
    CUDA_OK(cudaMalloc(reinterpret_cast<void **>(&d_A), bytes));
    CUDA_OK(cudaMalloc(reinterpret_cast<void **>(&d_B), bytes));
    CUDA_OK(cudaMalloc(reinterpret_cast<void **>(&d_C), bytes));
    CUDA_OK(cudaMemcpy(d_A, A.data(), bytes, cudaMemcpyHostToDevice));
    CUDA_OK(cudaMemcpy(d_B, B.data(), bytes, cudaMemcpyHostToDevice));

    cudaEvent_t inicio, fim;
    CUDA_OK(cudaEventCreate(&inicio));
    CUDA_OK(cudaEventCreate(&fim));

    FILE *csv = nullptr;
    if (caminho_csv) {
        csv = std::fopen(caminho_csv, "w");
        if (!csv) { std::perror("Nao foi possivel abrir o CSV"); return 3; }
        std::fprintf(csv, "n,bloco_x,bloco_y,threads_por_bloco,grade_x,grade_y,total_blocos,threads_lancadas,repeticoes,mediana_ms,media_ms,min_ms,max_ms,dp_ms,validacao\n");
    }

    const Configuracao configuracoes[] = {{8, 8}, {16, 16}, {32, 8}, {32, 16}, {32, 32}};
    std::printf("%-10s %12s %10s %15s %13s %13s %13s %10s\n",
                "Bloco", "Grade", "Blocos", "Threads", "Mediana(ms)", "Media(ms)", "Min(ms)", "Validacao");
    bool todas_ok = true;
    std::vector<double> medianas;

    for (const auto &cfg : configuracoes) {
        const dim3 bloco(cfg.bx, cfg.by);
        const dim3 grade((n + cfg.bx - 1) / cfg.bx,
                         (n + cfg.by - 1) / cfg.by);
        const unsigned long long total_blocos = static_cast<unsigned long long>(grade.x) * grade.y;
        const unsigned long long total_threads = total_blocos * bloco.x * bloco.y;
        if (cfg.bx * cfg.by > device.maxThreadsPerBlock) {
            std::fprintf(stderr, "Bloco %dx%d excede maxThreadsPerBlock (%d).\n",
                         cfg.bx, cfg.by, device.maxThreadsPerBlock);
            return 4;
        }

        for (int i = 0; i < aquecimentos; ++i) {
            multiplicar_gpu<<<grade, bloco>>>(d_A, d_B, d_C, n);
            CUDA_OK(cudaGetLastError());
        }
        CUDA_OK(cudaDeviceSynchronize());

        std::vector<double> tempos;
        tempos.reserve(repeticoes);
        for (int i = 0; i < repeticoes; ++i) {
            CUDA_OK(cudaEventRecord(inicio));
            multiplicar_gpu<<<grade, bloco>>>(d_A, d_B, d_C, n);
            CUDA_OK(cudaGetLastError());
            CUDA_OK(cudaEventRecord(fim));
            CUDA_OK(cudaEventSynchronize(fim));
            float ms = 0.0f;
            CUDA_OK(cudaEventElapsedTime(&ms, inicio, fim));
            tempos.push_back(ms);
        }
        CUDA_OK(cudaMemcpy(C_gpu.data(), d_C, bytes, cudaMemcpyDeviceToHost));

        size_t divergencias = 0;
        for (size_t i = 0; i < elementos; ++i) {
            if (std::fabs(C_cpu[i] - C_gpu[i]) > 0.001f) ++divergencias;
        }
        const bool ok = (divergencias == 0);
        todas_ok = todas_ok && ok;

        std::sort(tempos.begin(), tempos.end());
        const double mediana = (repeticoes % 2 == 0)
            ? (tempos[repeticoes / 2 - 1] + tempos[repeticoes / 2]) / 2.0
            : tempos[repeticoes / 2];
        medianas.push_back(mediana);
        double soma = 0.0;
        for (double t : tempos) soma += t;
        const double media = soma / repeticoes;
        double soma_quad = 0.0;
        for (double t : tempos) soma_quad += (t - media) * (t - media);
        const double dp = std::sqrt(soma_quad / repeticoes);

        char bnome[16], gnome[16];
        std::snprintf(bnome, sizeof(bnome), "%dx%d", cfg.bx, cfg.by);
        std::snprintf(gnome, sizeof(gnome), "%ux%u", grade.x, grade.y);
        std::printf("%-10s %12s %10llu %15llu %13.6f %13.6f %13.6f %10s\n",
                    bnome, gnome, total_blocos, total_threads,
                    mediana, media, tempos.front(), ok ? "OK" : "FALHOU");
        if (csv) {
            std::fprintf(csv, "%d,%d,%d,%u,%u,%u,%llu,%llu,%d,%.9f,%.9f,%.9f,%.9f,%.9f,%s\n",
                n, cfg.bx, cfg.by, bloco.x * bloco.y, grade.x, grade.y,
                total_blocos, total_threads, repeticoes, mediana, media,
                tempos.front(), tempos.back(), dp, ok ? "OK" : "FALHOU");
            std::fflush(csv);
        }
    }

    const double referencia = medianas[1]; // Configuracao 16x16 da aula original.
    size_t melhor = 0;
    std::printf("\nComparacao com 16x16 (aceleracao > 1 significa menor tempo):\n");
    for (size_t i = 0; i < medianas.size(); ++i) {
        if (medianas[i] < medianas[melhor]) melhor = i;
        std::printf("  %2dx%-2d : %.3fx\n",
                    configuracoes[i].bx, configuracoes[i].by, referencia / medianas[i]);
    }
    std::printf("Mais rapida nesta rodada: %dx%d (mediana %.6f ms)\n",
                configuracoes[melhor].bx, configuracoes[melhor].by, medianas[melhor]);
    if (csv) {
        std::fclose(csv);
        std::printf("CSV salvo: %s\n", caminho_csv);
    }
    std::printf("Nota: os tempos acima medem apenas o kernel; nao incluem transferencias ou cudaMalloc.\n");
    CUDA_OK(cudaEventDestroy(inicio));
    CUDA_OK(cudaEventDestroy(fim));
    CUDA_OK(cudaFree(d_A));
    CUDA_OK(cudaFree(d_B));
    CUDA_OK(cudaFree(d_C));
    return todas_ok ? 0 : 5;
}
