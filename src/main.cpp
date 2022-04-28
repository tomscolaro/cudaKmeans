#include <iostream>
#include "argparse.h"
#include "seqkmeans.h"
#include "io.h"
#include <chrono>
#include <cstring>
#include "helpers.h"

using namespace std;


int main(int argc, char **argv)
{   
    //TEST COMMAND
    //  ./bin/cuda_kmeans -k 16 -d 16 -i ./tests/random-n2048-d16-c16.txt -m 150 -t 10e-5 -s 8675309 -a 0 --c
    struct options_t opts;
    get_opts(argc, argv, &opts);
    // // Setup args & read input data
    kmeans_args_t *ps_args = alloc_args();
    int n_vals;
    double  *input_vals;
    int *output_vals;
    // double centroids[256];
    double *centroids = new double[ (opts.num_cluster * opts.dims)];



    read_file(&opts, &n_vals, &input_vals, &output_vals);
    // srand(opts.seed);

    kmeans_srand(opts.seed ); // cmd_seed is a cmdline arg
    // // Get Random Point
    for (int i = 0; i < opts.num_cluster; i++){
        int idx = kmeans_rand() % n_vals;
        // int idx = 1453;
        for (int j = 0; j < opts.dims ; j++) {
				centroids[i*opts.dims + j] = input_vals[idx*opts.dims + j];
		} 
    }

    //Run Sequential    
    //start timmer
    int hold = 0;
    int *a = &hold;
    auto start = std::chrono::high_resolution_clock::now();
    
    seq_k_means(output_vals, input_vals, centroids, opts.num_cluster, n_vals, opts.dims, opts.threshold, opts.max_num_iter, a);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    
    printf("%d,%lf\n", *a, (float)diff.count()/(*a)) ;
    std::cout.precision(5);
    if(opts.c){
        
        for(int i =0; i<opts.num_cluster; i++){
            std::cout << std::fixed;
            std::cout << i << " ";
            for (int d = 0; d<opts.dims; d++){
                if (d < opts.dims -1){
                    std::cout << centroids[i*opts.dims + d] <<" ";
                }
                else{
                    std::cout << centroids[i*opts.dims + d];
                }
            }

            std::cout << std::endl;
        }

    } else{
     printf("clusters:");
    for (int p=0; p < n_vals; p++){
        printf(" %d", output_vals[p]);
    };
    }



    delete[] input_vals;
    delete[] output_vals;
    delete[] centroids;
}
