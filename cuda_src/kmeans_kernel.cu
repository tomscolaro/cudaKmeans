#include <iostream>
#include "cudakmeans.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <float.h>
#include <cmath>

__device__ double distance(double *d_inputs, double *d_centroids, int idx1, int idx2, int dims)
{
	double dist = 0;

    for(int i = 0; i<dims; i++){
        dist += (d_centroids[idx2*dims + i] - d_inputs[idx1*dims + i] ) * (d_centroids[idx2*dims + i] - d_inputs[idx1*dims + i] );
    }
    
    dist = sqrt(dist);
    return dist;
}


__global__ void clusterAssignment(int *d_outputs, double *d_inputs, double *d_centroids,
								  int k, int n_vals, int dims, double *holdCentroid, double *numberCluster){

     int tid = blockDim.x * blockIdx.x + threadIdx.x;

     if (tid >= n_vals) return;
       
     double min_dist = DBL_MAX;
     int centroid_label = 0;
      
     double dist = 0;
       
        for (int i = 0; i < k; i++){
           dist = distance(d_inputs, d_centroids, tid, i, dims);
           
           if (dist < min_dist){
               min_dist = dist;
               centroid_label = i;     
           } 
        }


       __syncthreads();
       d_outputs[tid] = centroid_label;
	   
       for (int d = 0; d < dims; d++){      
            atomicAdd(&holdCentroid[centroid_label*dims + d],  d_inputs[tid*dims + d]);
       }
   
        atomicAdd(&numberCluster[centroid_label], 1.0);
}



__global__ void avgCentroids(double *d_centroids, int k, int dims, double *holdCentroid, double *numberCluster){

  
     int tid = blockDim.x * blockIdx.x + threadIdx.x;

	 if (tid >= k*dims) return;
	 
	 int clust = tid/dims;
	 d_centroids[tid] = holdCentroid[tid]/numberCluster[clust];   

	 
}



__global__ void warm_up_gpu(){
  unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
  float ia, ib;
  ia = ib = 0.0f;
  ib += ia + tid; 
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




void kernelKmeans(int* outputs, double* inputs, double* centroids, int k, int n_vals, int dims, int iters, double threshold){
    float time;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
	
    int *d_outputs;
    double *d_inputs;
    double *d_centroids;
    double *hold_centroids;
    double *numclusters;
    
    
    
     double prev_centroids[k*dims];
    
    
    
	cudaFree(0); //used to warmup the gpu -- 
    cudaMalloc((void**)&d_outputs , n_vals * sizeof(int));
    cudaMalloc((void**)&d_inputs , n_vals * dims * sizeof(double));
    cudaMalloc((void**)&d_centroids , k * dims * sizeof(double));
    cudaMalloc((void**)&hold_centroids , k * dims * sizeof(double));
    cudaMalloc((void**)&numclusters , k * sizeof(double));
  
    cudaMemcpy(d_outputs, outputs,      n_vals * sizeof(int),         cudaMemcpyHostToDevice);
    cudaMemcpy(d_inputs, inputs,        n_vals * dims * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_centroids, centroids,  k * dims * sizeof(double),    cudaMemcpyHostToDevice);
    
	

	const int T = 1024;
	const int grid_size = (n_vals + T) / T;
	warm_up_gpu<<<grid_size, T>>>();
	

   
    bool done = false;
    int iter = 0;
    while(!done){
         
         cudaMemcpy(prev_centroids, d_centroids, k * dims * sizeof(double), cudaMemcpyDeviceToHost);
         
         
         cudaMemset(hold_centroids, 0,  k * dims * sizeof(double));
         cudaMemset(numclusters, 0,  k * sizeof(double));
    
	
         clusterAssignment<<<grid_size,T>>>(d_outputs, d_inputs, d_centroids, k, n_vals, dims, hold_centroids, numclusters);
         cudaDeviceSynchronize();
          
         avgCentroids<<<1, (k*dims)>>>(d_centroids, k, dims, hold_centroids, numclusters);
         cudaDeviceSynchronize();
         
         
         cudaMemcpy(centroids, d_centroids, k * dims * sizeof(double), cudaMemcpyDeviceToHost);
        
      iter++;   
      done = (iter >= iters) || converged(prev_centroids, centroids, k, dims, threshold);
      }
    
    
    
    iters = iter;

       
    cudaMemcpy(centroids, d_centroids, k * dims * sizeof(double), cudaMemcpyDeviceToHost);
    cudaMemcpy(outputs, d_outputs,     n_vals * sizeof(int),   cudaMemcpyDeviceToHost);

    cudaFree(hold_centroids);
    cudaFree(numclusters);
    cudaFree(d_outputs);
    cudaFree(d_inputs);
    cudaFree(d_centroids);
    
    
    cudaDeviceSynchronize();
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time, start, stop);

    
    printf("%d,%lf\n", iters, time/iter);
       


   
}

