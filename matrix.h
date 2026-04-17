#include <iostream>
#include <vector>
using namespace std;

class Matrix {
    public:
    int rows, cols;
    vector<vector<double>> matrix; // this is references as A to simplify explanations
    
// constructor
    Matrix(int r, int c) : rows(r), cols(c), matrix(r, vector<double>(c, 0.0)) {
    // Use member initializer list - properly initializes matrix member
}   

    Matrix (const vector<vector<double>> & init) {
        if (init.empty()) {
            rows=cols= 0;
            matrix = {};
            return ;
        }
        // takes a 2d vector as input
        rows = init.size();
        cols = init[0].size();
        matrix = init;
    }

    
    // transposes the matrix
    Matrix  transpose() {
        Matrix C(cols, rows); ; //changes dimension from (rows, cols) -> (cols, rows)
        for (int i = 0; i < rows; i++) {
            for(int j =0 ; j < cols;j++) {
                cout << matrix[i][j] << endl;
                C.matrix[j][i] = matrix[i][j];
            }
            
        }
        return C;
    }

    Matrix fill_zeroes() {
        // converts the matrix to a zero matrix of given dimensions 
        Matrix r(rows, cols);
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols;j++) {
                r.matrix[i][j] = 0;
            }
        }
        return r;

    }

    void print_matrix() {
        for(int i =0 ; i < rows; i++) {
            for(int j =0; j < cols; j++){
                if (j == 0) cout << matrix[i][j];
                else cout << matrix[i][j] << ", ";
            }
            cout << endl;
        }
    }
    
    
    
    // performs addition between 2 matrices
    Matrix add(Matrix & B) {
        Matrix r(rows, cols);
        // checking for the same dimensions
        if (!(rows == B.rows && cols == B.cols)) {
            cout << "Invalid Operation: Dimensions of both the matrices r not the same" << endl;
            return r;
        }
        for(int i = 0 ; i < rows; i ++ ) {
            for(int j =0; j < cols; j++) {
                r.matrix[i][j] = (matrix[i][j] + B.matrix[i][j]);
            }
        }
        return r;

    }

     // performs subtraction between 2 matrices
    Matrix subtract(Matrix & B) {
        Matrix C(rows, cols);
        // checking for the same dimensions
        if (!(rows == B.rows && cols == B.cols)) {
            cout << "Invalid Operation: Dimensions of both the matrices C not the same" << endl;
            return C;
        }
        for(int i = 0 ; i < rows; i ++ ) {
            for(int j =0; j < cols; j++) {
                C.matrix[i][j] = (matrix[i][j] - B.matrix[i][j]);
            }
        }
        return C;
    }
    // matrix multiplication
    Matrix matmul(Matrix & B) {
        // performs matrix multiplication
        // valid operation when cols of A are equal to rows of B 
        Matrix C(rows, B.cols); // resultant matrix
        if (!(cols == B.rows)){
            cout << "Invalid Operation: This is not a valid operation as A.columns != B.rows" << endl;
            return C;
        }

        for(int i = 0; i < C.rows;i++) {
            for(int j = 0; j < C.cols; j++) {
                for(int k = 0 ; k < cols; k++) {
                    C.matrix[i][j] += matrix[i][k] * B.matrix[k][j];
                }
            }

        }
        return C;
    }

    Matrix scale(double alpha) {
        Matrix C(rows, cols);
        for(int i = 0; i < rows; i++) {
            for(int j = 0 ; j < cols; j++ ) {
                C.matrix[i][j] = alpha * matrix[i][j];
            }
        }
        return C;
    }

};
