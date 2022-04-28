#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <chrono>
#include <cstring>
#include "cudakmeans.h"
#include <fstream>

using namespace std;
struct options_t {
    char *inputs;
    int num_cluster;
    int dims;
    int max_num_iter;
    int seed;
    double threshold;
    bool c;
    int implementation;
};

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

void get_opts(int argc,
              char **argv,
              struct options_t *opts){
    if (argc == 1)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t--num_cluster or -k <num_cluster>" << std::endl;
        std::cout << "\t--dims or -d <dim>" << std::endl;
        std::cout << "\t--inputfilename or -i <inputfilename>" << std::endl;
        // std::cout << "\t--outputfilename or -o <inputfilename>" << std::endl;
        std::cout << "\t--max_num_iter or -m <max_num_iter>" << std::endl;
        std::cout << "\t--threshold or -t <threshold>" << std::endl;
        std::cout << "\t--c or -c <output>" << std::endl;
        std::cout << "\t--seed or -s <seed>" << std::endl;
        std::cout << "\t--imp or -a <implementation>" << std::endl;
        exit(0);
    }

    opts->c = false;

    struct option l_opts[] = {
        {"num_cluster", required_argument, NULL, 'k'},
        {"dim", required_argument, NULL, 'd'},
        {"inputs", required_argument, NULL, 'i'},
        // {"outputs", required_argument, NULL, 'n'},
        {"max_num_iter", required_argument, NULL, 'm'},
        {"threshold", no_argument, NULL, 't'},
        {"seed", no_argument, NULL, 's'},
        {"centroids", no_argument, 0, 'c'},
        {"implementation", no_argument, NULL, 'a'}
    };

    int ind, c;
    while ((c = getopt_long(argc, argv, "k:d:i:m:t:s:ca:", l_opts, &ind)) != -1)
    {
        switch (c)
        {
        case 0:
            break;
        case 'k':
            opts->num_cluster = atoi((char *)optarg);
            break;
        case 'd':
            opts->dims = atoi((char *)optarg);
            break;
        case 'i':
            opts->inputs = (char *)optarg;
            break;
        case 'm':
            opts->max_num_iter = atoi((char *)optarg);
            break;
        case 't':
            opts->threshold = atof((char *)optarg);
            break;
        case 'c':
            opts->c = true;
            break;
        case 's':
            opts->seed = atoi((char *)optarg);
            break;
        case 'a':
            opts->implementation = atoi((char *)optarg);
            break;
        case ':':
            std::cerr << argv[0] << ": option -" << (char)optopt << "requires an argument." << std::endl;
            exit(1);
        }
    }
}



kmeans_args_t* alloc_args(void ) {
  return (kmeans_args_t*) malloc(sizeof(kmeans_args_t));
}

    static unsigned long int next_val = 1;
    static unsigned long kmeans_rmax = 32767;
    
    int kmeans_rand() {
        next_val = next_val * 1103515245 + 12345;
        return (unsigned int)(next_val/65536) % (kmeans_rmax+1);
    };
    void kmeans_srand(unsigned int seed) {
        next_val = seed;
    };



void read_file(struct options_t* args,
               int*              n_vals,
               double**             input_vals,
               int**             output_vals) {

  	// Open file
	std::ifstream read_file(args->inputs);
	read_file.precision(16);
	read_file >> *n_vals;

	*input_vals = (double*) malloc( *n_vals * args->dims *sizeof(double));

	*output_vals = (int*) malloc(*n_vals * sizeof(int));

	for (int i = 0; i < *n_vals; i++) {
		for (int j = 0; j < args->dims ; j++) {
				read_file.ignore ( std::numeric_limits<std::streamsize>::max(), ' ' );
				read_file >> (*input_vals)[i*args->dims + j];
		}
	}
	    //set mem out for return
        // std::memcpy(data, *input_vals , *n_vals * args->dims * sizeof(double));

	// std::cout << sizeof(data) << " |  "  << sizeof(*input_vals)   << std::endl;

}

void write_file(struct options_t*         args,
               	struct kmeans_args_t* opts) {
  // Open file
	std::ofstream read_out_file;
	read_out_file.precision(5);
	read_out_file.open("./read_out_filetest.txt");

	// Write solution to read_out_fileput file
	// read_out_file << opts->n_vals << std::endl;

	
	for(int i =0; i < (opts->num_cluster); i++){
            read_out_file << std::fixed;
            read_out_file << i << " ";
            for (int d = 0; d < (opts->dims); d++){
                if (d < (opts->dims -1)){
                    read_out_file << (opts->centroids)[i*opts->dims + d] <<" ";
                }
                else{
                    read_out_file << (opts->centroids)[i*opts->dims + d];
                }
            }

           	read_out_file << std::endl;
        }


	read_out_file.flush();
	read_out_file.close();
	
	// std::cout << opts -> output_vals << std::endl;
	// // Free memory
	free(opts->input_vals);
	free(opts-> output_vals);
    free(opts->centroids);
}

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
    double *centroids = new double[opts.num_cluster * opts.dims];



    read_file(&opts, &n_vals, &input_vals, &output_vals);
    kmeans_srand(opts.seed ); // cmd_seed is a cmdline arg
    // // Get Random Point for idx
    for (int i = 0; i < opts.num_cluster; i++){
        int idx = kmeans_rand() % n_vals;
        for (int j = 0; j < opts.dims ; j++) {
                //std::cout << "centroid idx" << idx << std::endl;
				centroids[i*opts.dims + j] = input_vals[idx*opts.dims + j];
		} 
    }

    
    kernelKmeans(output_vals, input_vals, centroids, opts.num_cluster, n_vals, opts.dims, opts.max_num_iter, opts.threshold);


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

    } else {
       printf("clusters:");
       for (int p=0; p < n_vals; p++){
         std::cout << output_vals[p] << " ";
        };
        std::cout << std::endl;
        
    }

    
    delete[] centroids;
    
    
    


}


