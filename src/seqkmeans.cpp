#include "helpers.h"
#include <cmath>
#include <iostream>
#include "seqkmeansutils.h"



void seq_k_means(int* outputs, double* inputs, double* centroids, int num_cluster, int n_vals, int input_dims, double threshold, int max_num_iter, int *iterConv)
{

    int dims = input_dims;
    int k_val = num_cluster;

    int iterations = 0; 
    double *oldCentroids = new double[num_cluster * dims]; 


    bool done = false;
    while (!done){
        
        for(int k=0 ; k < (k_val * dims); k++){
            oldCentroids[k] = centroids[k];
        }


        iterations++;

        nearestCentroid(inputs, outputs, centroids, k_val, dims, n_vals);

        generateNewCentroids(inputs, outputs, centroids, k_val, dims, n_vals);

        done = iterations >= max_num_iter || converged(oldCentroids, centroids, k_val, dims, threshold);
    }


    delete [] oldCentroids;


    *iterConv = iterations;
}

