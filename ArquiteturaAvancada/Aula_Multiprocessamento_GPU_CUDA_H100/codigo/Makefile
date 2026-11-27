CC ?= gcc
NVCC ?= nvcc
CFLAGS = -O3 -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=199309L
NVFLAGS = -O3 -arch=sm_90 -std=c++14

.PHONY: all cpu gpu demo-cpu demo-gpu benchmark clean help
all: cpu gpu
cpu: matriz_cpu
gpu: matriz_h100

matriz_cpu: matriz_cpu.c
	$(CC) $(CFLAGS) -o $@ $<

matriz_h100: matriz_h100.cu
	$(NVCC) $(NVFLAGS) -o $@ $<

demo-cpu: cpu
	./matriz_cpu 4

demo-gpu: gpu
	./matriz_h100 4

benchmark: gpu
	./matriz_h100 256
	./matriz_h100 512
	./matriz_h100 1024

help:
	@echo 'make cpu          - compilar codigo C comum'
	@echo 'make gpu          - compilar codigo CUDA para H100 (sm_90)'
	@echo 'make demo-cpu     - executar exemplo 4x4 na CPU'
	@echo 'make demo-gpu     - executar 4x4 na GPU (dentro de job com H100)'
	@echo 'make benchmark    - executar N=256, 512, 1024 na GPU'
	@echo 'make clean        - apagar binarios'

clean:
	rm -f matriz_cpu matriz_h100
