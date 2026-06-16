#include "dataset.h"
#include "model.h"
#include "matrix.h"
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
    cout << "w0" << ": " << model.weights.matrix[0][0] << endl;
    cout << "w1" << ": " << model.weights.matrix[1][0] << endl;

    // predict and print
    Matrix y_hat = model.predict(ds.X);
    cout << "Predicted values: " << endl;
    for(int i = 0 ; i < ds.m; i++) {
        cout << "Predicted: " << y_hat.matrix[i][0] << ", Actual: " << ds.Y.matrix[i][0] << endl;
    }
// loss
    cout << "MSE Loss: " << model.compute_loss(y_hat, ds.Y) << endl;
    return 0;
}