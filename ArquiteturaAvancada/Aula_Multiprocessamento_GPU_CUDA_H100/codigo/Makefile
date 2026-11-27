CC = gcc
NVCC ?= nvcc
CFLAGS = -O3 -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=199309L
OMPFLAGS = $(CFLAGS) -fopenmp
NVFLAGS = -O3 -arch=sm_90 -std=c++14

.PHONY: all cpu gpu openmp demo-cpu demo-gpu demo-openmp benchmark clean help
all: cpu gpu openmp
cpu: matriz_cpu
gpu: matriz_h100
openmp: matriz_openmp

matriz_cpu: matriz_cpu.c
	$(CC) $(CFLAGS) -o $@ $<

matriz_h100: matriz_h100.cu
	$(NVCC) $(NVFLAGS) -o $@ $<

matriz_openmp: matriz_openmp.c
	$(CC) $(OMPFLAGS) -o $@ $< -lm

demo-cpu: cpu
	./matriz_cpu 4

demo-gpu: gpu
	./matriz_h100 4

demo-openmp: openmp
	./matriz_openmp 4 4 5

benchmark: gpu
	./matriz_h100 256
	./matriz_h100 512
	./matriz_h100 1024

help:
	@echo 'make openmp         - compilar OpenMP na CPU Intel'
	@echo 'make demo-openmp    - multiplicacao 4x4, 4 threads, 5 repeticoes'
	@echo 'make gpu           - compilar CUDA para H100 (sm_90)'
	@echo 'make cpu           - compilar codigo C sequencial'
	@echo 'make all           - compilar os tres programas'
	@echo 'make clean         - remover binarios'

clean:
	rm -f matriz_cpu matriz_h100 matriz_openmp
