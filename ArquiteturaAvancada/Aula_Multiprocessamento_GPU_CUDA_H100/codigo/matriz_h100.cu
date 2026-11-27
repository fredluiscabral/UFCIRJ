// Multiplicacao de matrizes quadradas em CPU e GPU NVIDIA H100.
// Cada thread CUDA calcula um elemento da matriz C = A * B.
// Compilar: nvcc -O3 -arch=sm_90 -o matriz_h100 matriz_h100.cu
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CUDA_OK(chamada) do {                                     \
    cudaError_t erro = (chamada);                                 \
    if (erro != cudaSuccess) {                                    \
        fprintf(stderr, "Erro CUDA (%s:%d): %s\n",              \
                __FILE__, __LINE__, cudaGetErrorString(erro));    \
        exit(EXIT_FAILURE);                                      \
    }                                                            \
} while (0)

static double segundos(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
}

// Mesmo algoritmo matematico no processador (CPU).
static void multiplicar_cpu(const float *A, const float *B, float *C, int n) {
    for (int linha = 0; linha < n; linha++) {
        for (int coluna = 0; coluna < n; coluna++) {
            float soma = 0.0f;
            for (int k = 0; k < n; k++) {
                soma += A[linha*n + k] * B[k*n + coluna];
            }
            C[linha*n + coluna] = soma;
        }
    }
}

// Uma thread calcula exatamente uma posicao de C.
__global__ void multiplicar_gpu(const float *A, const float *B, float *C, int n) {
    int linha  = blockIdx.y * blockDim.y + threadIdx.y;
    int coluna = blockIdx.x * blockDim.x + threadIdx.x;

    if (linha < n && coluna < n) {
        float soma = 0.0f;
        for (int k = 0; k < n; k++) {
            soma += A[linha*n + k] * B[k*n + coluna];
        }
        C[linha*n + coluna] = soma;
    }
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 4;
    if (n <= 0 || n > 2048) {
        fprintf(stderr, "Uso: %s [N entre 1 e 2048]\n", argv[0]);
        return 1;
    }

    size_t elementos = (size_t)n * (size_t)n;
    size_t bytes = elementos * sizeof(float);
    float *A = (float *)malloc(bytes);
    float *B = (float *)malloc(bytes);
    float *C_cpu = (float *)malloc(bytes);
    float *C_gpu = (float *)malloc(bytes);
    if (!A || !B || !C_cpu || !C_gpu) {
        fprintf(stderr, "Memoria insuficiente na CPU.\n");
        free(A); free(B); free(C_cpu); free(C_gpu);
        return 1;
    }

    // N=4 permite acompanhar o calculo manualmente; N maior serve para medicao.
    for (size_t i = 0; i < elementos; i++) {
        A[i] = (n == 4) ? (float)(i + 1) : (float)((i % 7) + 1);
        B[i] = (n == 4) ? 1.0f : (float)((i % 5) + 1);
    }

    double inicio = segundos();
    multiplicar_cpu(A, B, C_cpu, n);
    double tempo_cpu = segundos() - inicio;

    int quantidade = 0;
    CUDA_OK(cudaGetDeviceCount(&quantidade));
    if (quantidade == 0) {
        fprintf(stderr, "Nenhuma GPU CUDA visivel neste job.\n");
        return 1;
    }
    cudaDeviceProp gpu;
    CUDA_OK(cudaGetDeviceProperties(&gpu, 0));
    printf("GPU: %s | compute capability %d.%d\n", gpu.name, gpu.major, gpu.minor);

    float *d_A = NULL, *d_B = NULL, *d_C = NULL;
    CUDA_OK(cudaMalloc((void **)&d_A, bytes));
    CUDA_OK(cudaMalloc((void **)&d_B, bytes));
    CUDA_OK(cudaMalloc((void **)&d_C, bytes));

    cudaEvent_t inicio_kernel, fim_kernel;
    CUDA_OK(cudaEventCreate(&inicio_kernel));
    CUDA_OK(cudaEventCreate(&fim_kernel));

    double inicio_total = segundos();
    CUDA_OK(cudaMemcpy(d_A, A, bytes, cudaMemcpyHostToDevice));
    CUDA_OK(cudaMemcpy(d_B, B, bytes, cudaMemcpyHostToDevice));

    // Para N=4: bloco 4x4. Para N maior: blocos 16x16 (256 threads).
    int lado = (n == 4) ? 4 : 16;
    dim3 bloco(lado, lado);
    dim3 grade((n + lado - 1) / lado, (n + lado - 1) / lado);

    CUDA_OK(cudaEventRecord(inicio_kernel));
    multiplicar_gpu<<<grade, bloco>>>(d_A, d_B, d_C, n);
    CUDA_OK(cudaGetLastError());
    CUDA_OK(cudaEventRecord(fim_kernel));
    CUDA_OK(cudaEventSynchronize(fim_kernel));

    float tempo_kernel_ms = 0.0f;
    CUDA_OK(cudaEventElapsedTime(&tempo_kernel_ms, inicio_kernel, fim_kernel));
    CUDA_OK(cudaMemcpy(C_gpu, d_C, bytes, cudaMemcpyDeviceToHost));
    double tempo_total_gpu = segundos() - inicio_total;

    int erros = 0;
    for (size_t i = 0; i < elementos; i++) {
        // Dados de entrada sao inteiros pequenos armazenados em float.
        // Uma pequena tolerancia tambem permite outras politicas de arredondamento.
        if (fabsf(C_cpu[i] - C_gpu[i]) > 0.001f) erros++;
    }

    printf("Matriz: %dx%d | Bloco CUDA: %dx%d | Grade: %dx%d\n",
           n, n, (int)bloco.x, (int)bloco.y, (int)grade.x, (int)grade.y);
    if (n == 4) {
        puts("C = A x B (calculada pela GPU):");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) printf("%6.0f", C_gpu[i*n+j]);
            putchar('\n');
        }
    }
    printf("Tempo CPU sequencial:       %.6f s\n", tempo_cpu);
    printf("Tempo GPU kernel:           %.6f ms\n", tempo_kernel_ms);
    printf("Tempo GPU com transferencias: %.6f s (sem alocacao)\n", tempo_total_gpu);
    printf("Conferencia CPU x GPU: %s (%d divergencias)\n",
           erros == 0 ? "OK" : "FALHOU", erros);

    CUDA_OK(cudaEventDestroy(inicio_kernel));
    CUDA_OK(cudaEventDestroy(fim_kernel));
    CUDA_OK(cudaFree(d_A));
    CUDA_OK(cudaFree(d_B));
    CUDA_OK(cudaFree(d_C));
    free(A); free(B); free(C_cpu); free(C_gpu);
    return erros == 0 ? 0 : 2;
}
