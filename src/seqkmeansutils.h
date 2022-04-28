#pragma once

#include <stdlib.h>
#include <iostream>


double distance(double* inputs,  double* centroids, int idx1, int idx2, int dims);
void nearestCentroid(double* inputs, int* outputs, double* centroids, int kclusters, int dims, int n_vals);
void generateNewCentroids(double* inputs, int* outputs, double* centroids, int kclusters, int dims, int n_vals);
bool converged(double* oldCentroids, double* currCentroids, int kclusters, int dims, double thres);
