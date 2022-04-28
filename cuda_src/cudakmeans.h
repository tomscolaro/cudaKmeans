#pragma once
#include <cuda.h>
#include <cuda_runtime.h>


void kernelKmeans(int* outputs, double* inputs, double* centroids, int k, int n_vals, int dims, int iters, double threshold);




