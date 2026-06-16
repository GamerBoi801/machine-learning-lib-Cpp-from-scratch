#include "dataset.h"
#include "model.h"
#include "matrix.h"
#include "metrics.h"
#include <iostream>
using namespace std;

int main() {
    // loading data
    Dataset ds;
    ds.load_hardcoded();
    ds.add_bias_column();

    // training the model
    LinearRegression model(1);
    model.fit_simple(ds.X, ds.Y);

    // printing weights
    cout << "=== Linear Regression from Scratch (C++) ===" << endl;

    cout << "Training Weights with the normal Equation...." << endl;
    cout << "w0" << ": " << model.weights.matrix[0][0] << endl;
    cout << "w1" << ": " << model.weights.matrix[1][0] << endl;

    // predict and print
    Matrix y_hat = model.predict(ds.X);
    cout << "Predicted VS Actual: : " << endl;
    for(int i = 0 ; i < ds.m; i++) {
        cout << "Predicted: " << y_hat.matrix[i][0] <<
                 ", Actual: " << ds.Y.matrix[i][0] << endl;
    }
    cout << "=== Metrics for Normal Equation Model ===" << endl;
    printf("MSE: %f\n", mse(y_hat, ds.Y));
    printf("MAE: %f\n", mae(y_hat, ds.Y));
    printf("R-squared: %f\n", r_squared(y_hat, ds.Y));


    // training with gradient descent
    LinearRegression model_gd(1);
    
    model_gd.epochs = 5000;
    model_gd.alpha = 1e-9;
    
    model_gd.fit_gd(ds.X, ds.Y);

    // print weights for gradient descent model
    cout << "Training Weights with Gradient Descent...." << endl;
    cout << "w0" << ": " << model_gd.weights.matrix[0][0] << endl;
    cout << "w1" << ": " << model_gd.weights.matrix[1][0] << endl;
    
    // predict and print
    Matrix y_hat_gd = model_gd.predict(ds.X);
    cout << "Predicted VS Actual (GD): : " << endl;
    for(int i = 0 ; i < ds.m; i++) {
        cout << "Predicted: " << y_hat_gd.matrix[i][0] <<
                 ", Actual: " << ds.Y.matrix[i][0] << endl;
    }
    cout << "=== Metrics for Gradient Descent Model ===" << endl;
    printf("MSE (GD): %f\n", mse(y_hat_gd, ds.Y));
    printf("MAE (GD): %f\n", mae(y_hat_gd, ds.Y));
    printf("R-squared (GD): %f\n", r_squared(y_hat_gd, ds.Y));

    return 0;
}