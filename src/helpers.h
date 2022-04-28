#pragma once

#include <stdlib.h>


struct kmeans_args_t {
    double* input_vals;
    int num_cluster;
    int dims;
    int max_num_iter;
    int seed;
    double threshold;
    bool c;
    int n_vals;
    int* output_vals;
    double* centroids;

};

kmeans_args_t* alloc_args();


void fill_args(kmeans_args_t *args,
    double *inputs,
    int num_cluster,
    int dims,
    int max_num_iter,
    int seed,
    double threshold,
    bool c,
    int n_vals, 
    int *output_vals,
    double *centroids
 );


int kmeans_rand();
void kmeans_srand(unsigned int seed);









