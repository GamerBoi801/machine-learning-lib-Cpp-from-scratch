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

    LinearRegression (int n) {
        // n: number of features
        alpha = 0.0001;
        epochs = 10;
        Matrix weights(n+1, 1);

        for (int i = 0 ; i < n+1; i++) {
            for(int j = 0 ; j < 1; j++) {
                weights.matrix[i][j] = 0.0;
            }
        }
    }

    Matrix predict(Matrix & X) {
        Matrix y_hat =  X.matmul(weights); // ŷ
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
            cout << "The determinant of the 2x2 matrix is zero. so the matrix is not invertible, returning a zero matrix" << endl;
            Matrix zeroed_matrix(2, 2);
            return zeroed_matrix.fill_zeroes();
        }

        Matrix adj(2, 2);
        adj.matrix = {{d, -b}, {-c, a}};
        Matrix inv = adj.scale(1.0 / det);
        return inv;
    }

    void  fit_simple(Matrix & X, Matrix & Y) {
        Matrix Xt = X.transpose(); //shape : [(n+1) x m]
        Matrix XtX = Xt.matmul(X); // shape : [(n+1) x (n+1)]
        Matrix XtX_inv = invert2x2(XtX); // shape: 2x2 
        Matrix  Xty = Xt.matmul(Y); // shape [(n+1) x 1] , for simple regression [2x1]
        weights = XtX_inv.matmul(Xty); // shape: [(n+1) x 1]
    }
    
};
