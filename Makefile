
cuda:
	nvcc -o ./cuda_src/kmeans.o -c ./cuda_src/cudakmeans.cpp -arch=sm_75 
	nvcc -o ./cuda_src/kmeans_kernel.o -c ./cuda_src/kmeans_kernel.cu -arch=sm_75 
	nvcc -o ./cuda_src/kmeans ./cuda_src/kmeans.o ./cuda_src/kmeans_kernel.o -arch=sm_75 
	
sharedmem:
	nvcc -o ./cudashmem/kmeans.o -c ./cudashmem/cudakmeansShmem.cpp -arch=sm_75 
	nvcc -o ./cudashmem/kmeans_kernel.o -c ./cudashmem/kmeans_kernelShmem.cu -arch=sm_75 
	nvcc -o ./cudashmem/kmeans ./cudashmem/kmeans.o ./cudashmem/kmeans_kernel.o -arch=sm_75 


thrust:
	nvcc -o ./thrustkmeans/kmeans.o -c ./thrustkmeans/cudakmeans.cpp -arch=sm_75 
	nvcc -o ./thrustkmeans/kmeans_kernel.o -c ./thrustkmeans/kmeans_kernel.cu -arch=sm_75 
	nvcc -o ./thrustkmeans/kmeans ./thrustkmeans/kmeans.o ./thrustkmeans/kmeans_kernel.o -arch=sm_75 

seq:
	nvcc ./src/*.cpp --compiler-options -std=c++17 -O3  -I./ -o ./src/kmeans


