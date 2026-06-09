#include <math.h>
#include <iostream>
#include "matrix.h"
#include "dataset.h"
using namespace std;


class LinearRegression {
    public:
    Matrix weights; // shape [(n+1)x1], one weight per feature + bias
    double alpha;  // learning rate for one gradient descent e.g 0.0001
    int epochs; // how many gradient descent iterations to run

    // n is no. of features
    LinearRegression(int n): weights(n+1, 1), alpha(0.0001), epochs(1000) {}

    Matrix predict(Matrix &X) {
        // returns a new matrix of shape[m x 1]
        Matrix y_hat = X.matmul(weights);
        // one prediction per sample
        // each val of y_hat[i][0] is the model's guess for the price of house i
        //  given its features are multipled by the current weigths
        return y_hat;
    }

    double compute_loss(Matrix & y_hat, Matrix & y) {
        // calculates the Mean squared error(mse) between the predicted(ŷ) and the actual values (y)
        double mse = 0 ;
        int m = y.rows;
        
        for(int i = 0; i < m; i++) {
            mse += pow(y_hat.matrix[i][0] - y.matrix[i][0], 2);
        }

        mse = mse * (1.0 / m);
        return mse;
    }

    Matrix invert2x2(Matrix & A) {
        /* 
            A = [ a  b ]       A⁻¹ = (1/det) × [  d  -b ]
                [ c  d ]                        [ -c   a ]

            where det = (a×d) - (b×c)    
        */
       auto [a, b, c, d] = std::tuple{A.matrix[0][0], A.matrix[0][1], A.matrix[1][0], A.matrix[1][1]};
        double det = (a*d) - (b*c);
        if (det == 0.0) {
            cout << "The determinant ofck before touching gradient desce the 2x2 matrix is zero. so the matrix is not invertible, returning a zero matrix" << endl;
            Matrix zeroed_matrix(2, 2);
            return zeroed_matrix.fill_zeroes();
        }

        Matrix adj(2, 2);
        adj.matrix = {{d, -b}, {-c, a}};
        Matrix inv = adj.scale(1.0 / det);
        return inv;
    }

    void fit_simple(Matrix & X, Matrix & Y) {
        // sets weigths according to the normal equation 
        // parameters  = (Xt X)^-1 * (Xt Y)
        Matrix Xt = X.transpose(); //shape : [(n+1) x m]
        Matrix XtX = Xt.matmul(X); // shape : [(n+1) x (n+1)]
        Matrix XtX_inv = invert2x2(XtX); // shape: 2x2 
        Matrix  Xty = Xt.matmul(Y); // shape [(n+1) x 1] , for simple regression [2x1]
        weights = XtX_inv.matmul(Xty); // shape: [(n+1) x 1]

        
        // weights[0][0] is the intercept - base prce for 0 sq.ft
        // weights[1][0] is the slope -- how many $ does each additional sqft adds
    }

    void fit_gd(Matrix & X, Matrix & Y) {
        // sets the weigths according to the gradient descent
        int m = X.rows;
        for(int i = 0 ; i < epochs; i++) {
            Matrix y_hat = predict(X);  //forward pass
            Matrix error = y_hat.subtract(Y); // [mx1] vector showing how wrong each prediction is 
            Matrix Xt = X.transpose(); // Xt [(n+1) x m], result [(n+1) x 1]
            Matrix grad = Xt.matmul(error); 
            grad = grad.scale(alpha);
            weights = weights.subtract(grad);

            
            if (i % 100 == 0) {
                cout << "Epoch No.  " <<i << " loss: " << compute_loss(predict(X), Y) << endl; 
            }
        }
    }
 
};
