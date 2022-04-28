#include "io.h"
#include "helpers.h"
#include <array>
#include <typeinfo>
#include <iostream>
#include <limits>
#include <cstring>


void read_file(struct options_t* args,
               int*              n_vals,
               double**             input_vals,
               int**             output_vals) {

  	// Open file
	std::ifstream file(args->inputs);
	file.precision(16);
	file >> *n_vals;


	*input_vals = (double *) malloc( *n_vals * args->dims *sizeof(double));
	*output_vals = (int *) malloc(*n_vals * sizeof(int));

	for (int i = 0; i < *n_vals; i++) {
		for (int j = 0; j < args->dims ; j++) {
				file.ignore ( std::numeric_limits<std::streamsize>::max(), ' ' );
				file >> (*input_vals)[i*args->dims + j];
		}
	}

}

void write_file(struct options_t*         args,
               	struct kmeans_args_t* opts) {
  // Open file
	std::ofstream out;
	out.precision(5);
	out.open("./outtest.txt");

	// Write solution to output file
	// out << opts->n_vals << std::endl;

	
	for(int i =0; i < (opts->num_cluster); i++){
            out << std::fixed;
            out << i << " ";
            for (int d = 0; d < (opts->dims); d++){
                if (d < (opts->dims -1)){
                    out << (opts->centroids)[i*opts->dims + d] <<" ";
                }
                else{
                    out << (opts->centroids)[i*opts->dims + d];
                }
            }

           	out << std::endl;
        }


	out.flush();
	out.close();
	
	// std::cout << opts -> output_vals << std::endl;
	// // Free memory
	free(opts->input_vals);
	free(opts-> output_vals);
}
