#include "matrix.h"
#include <iostream>
#include <math.h>
using namespace std;

class Dataset {
    public:
    Matrix X;
    Matrix Y;
    int m;
    int n;

    // CHANGE: default constructor needed because Matrix has no default constructor
    Dataset() : X(1,1), Y(1,1), m(0), n(0) {}

    void load_hardcoded() {
        m = 10;
        n = 1;

        X = Matrix(10, 1);
        Y = Matrix(10, 1);

        X.matrix = {{800.0}, {1050.0}, {1200.0}, {1400.0}, {1600.0}, {1750.0}, {2000.0}, {2100.0}, {2300.0}, {2500.0}};
        Y.matrix = {{152.0}, {183.0}, {214.0}, {248.0}, {271.0}, {305.0}, {340.0}, {358.0}, {391.0}, {410.0}};
    }

    void add_bias_column() {
        Matrix Copy(X.rows, X.cols + 1);
        for (int i = 0; i < X.rows; i++) {
            Copy.matrix[i][0] = 1.0;
            // CHANGE: loop over all existing columns instead of hardcoding [i][0]
            // this future-proofs for multi-feature: copies col 0, 1, 2... into col 1, 2, 3...
            for (int j = 0; j < X.cols; j++) {
                Copy.matrix[i][j + 1] = X.matrix[i][j];
            }
        }
        X = Copy;         // CHANGE: was missing — Copy was built but never assigned back to X
        n = X.cols;       // CHANGE: was missing — n must reflect the new column count
    }

    void normalize() {
        double mean_j = 0;
        double std_j = 0;

        // CHANGE: loop variable is j (feature columns), not i — you were iterating rows as if they were columns
        for (int j = 0; j < n; j++) {
            double total = 0;
            double mse = 0;

            // CHANGE: inner loop variable is i (samples/rows), not j
            for (int i = 0; i < m; i++) {
                total += X.matrix[i][j];   // CHANGE: was X.matrix[i][j] with swapped i/j — now correctly row i, col j
            }
            mean_j = (1.0 / m) * total;

            for (int i = 0; i < m; i++) {
                mse += pow(X.matrix[i][j] - mean_j, 2);  // CHANGE: same index fix
            }
            std_j = sqrt((1.0 / m) * mse);

            for (int i = 0; i < m; i++) {
                X.matrix[i][j] = (X.matrix[i][j] - mean_j) / std_j;  // CHANGE: same index fix
            }
        }
    }
};