#include "helpers.h"

kmeans_args_t* alloc_args(void ) {
  return (kmeans_args_t*) malloc(sizeof(kmeans_args_t));
}


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
                double *centroids){
  
  args[0] = {inputs, num_cluster, dims,
                max_num_iter, seed, 
                threshold, c, n_vals, output_vals, centroids};
        }



    static unsigned long int next = 1;
    static unsigned long kmeans_rmax = 32767;
    
    int kmeans_rand() {
        next = next * 1103515245 + 12345;
        return (unsigned int)(next/65536) % (kmeans_rmax+1);
    };
    void kmeans_srand(unsigned int seed) {
        next = seed;
    };