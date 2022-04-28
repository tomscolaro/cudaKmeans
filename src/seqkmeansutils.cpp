#include "helpers.h"
#include <cmath>
#include <iostream>
#include <float.h>
#include <cmath>

double distance(double* inputs, 
                double* centroids,
                int idx1,
                int idx2,
                int dims){

	double dist = 0;

    for(int i = 0; i<dims; i++){
        dist += pow(centroids[idx2*dims + i] - inputs[idx1*dims + i], 2 ) ;
    }
    
    dist = sqrt(dist);
    return dist;
}

void nearestCentroid(double* inputs,
                    int* outputs,
                    double* centroids,
                    int kclusters,
                    int dims,
                    int n_vals){

    for (int i =0; i<n_vals; i++){
        
        double min_dist = DBL_MAX;
        int min_label = -1;

        for (int k = 0; k<kclusters; k++){
            double temp_dist = distance(inputs, centroids, i, k, dims);

            if(min_dist > temp_dist){
                min_dist = temp_dist;
                min_label = k;
            }
        }


    outputs[i] = min_label;
    }
}

void generateNewCentroids(double* inputs,
                    int* outputs,
                    double* centroids,
                    int kclusters,
                    int dims,
                    int n_vals){

    double *holdCentroid =  new double[kclusters*dims]{0};
    int *numberCluster = new int[kclusters]{0};

    for (int i =0; i<n_vals; i++){
        int label = outputs[i];
        // std::cout << "label: " << label << std::endl;
        for (int k = 0; k<kclusters; k++){
            if(label == k){
                numberCluster[k] = numberCluster[k] +  1;

                for (int d = 0; d<dims; d++){                    
                    holdCentroid[k*dims + d] += inputs[i*dims + d];
                }
            }
        }
    }

    for(int k=0; k<kclusters; k++){
        //std::cout << "number of points in " << k << ": " << numberCluster[k] << std::endl;
        for (int d=0; d<dims; d++){
          centroids[k*dims + d] = holdCentroid[k*dims + d]/numberCluster[k];
            
        }
    }

    delete [] holdCentroid;
    delete [] numberCluster;

}

bool converged(double* oldCentroids, double* currCentroids, int kclusters, int dims, double thres){

    double dist = 0;
    for (int k = 0; k< kclusters; k++){
        double temp_dist = 0;
        for(int i = 0; i<dims; i++){
            temp_dist = temp_dist + ( (currCentroids[k*dims + i] - oldCentroids[k*dims + i]  ) * (currCentroids[k*dims + i] - oldCentroids[k*dims + i] ));
        }

        temp_dist = sqrt(temp_dist);

        if (dist < temp_dist){
            dist = temp_dist;
        }

    }

    return (bool)(dist < (thres*thres));
}