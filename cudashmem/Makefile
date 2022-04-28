CC = nvcc 
INC = ./cudasrc/
OPTS = -std=c++17 -O3

EXEC = ../kmeans

all: compile

compile:
	$(CC) -o kmeans.o -c ./cudakmeansShmem.cpp -arch=sm_75 
	$(CC) -o kmeans_kernel.o -c ./kmeans_kernelShmem.cu -arch=sm_75 
	$(CC) -o kmeans kmeans.o kmeans_kernel.o -arch=sm_75
