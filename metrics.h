#ifndef METRICS_H
#define METRICS_H

#include "matrix.h"
#include <iostream>
#include <math.h>
using namespace std;

double mse(Matrix &y_hat, Matrix &y) {
    double mse = 0 ;
    int m = y.rows;
    
    for(int i = 0; i < m; i++) {
        mse += pow(y_hat.matrix[i][0] - y.matrix[i][0], 2);
    }

    mse = mse * (1.0 / m);
    return mse;
}

double mae(Matrix &y_hat, Matrix &y) {
    double mae = 0;
    int m = y.rows;

    for(int i = 0 ; i < m; i ++ ) {
        mae += abs(y_hat.matrix[i][0] - y.matrix[i][0]);
    }
    mae = mae * (1.0 / m);
    return mae;
}

double r_squared(Matrix &y_hat, Matrix &y) {
    int m = y.rows;

    // mean of y
    double mean_y = 0;
    for(int i = 0 ; i < m; i++) {
        mean_y += y.matrix[i][0];
    }
    mean_y = mean_y * (1.0 / m);

    double sum_of_sq = 0;
    double residual_sum_of_sq = 0;
    for(int i = 0; i < m; i++) {
        sum_of_sq += pow(y.matrix[i][0] - mean_y, 2);
        residual_sum_of_sq += pow(y.matrix[i][0] - y_hat.matrix[i][0], 2);
    }
    double r2 = 1 - (residual_sum_of_sq / sum_of_sq);   
    return r2;
}

#endif 