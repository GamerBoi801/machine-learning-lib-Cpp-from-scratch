#include <iostream>
#include <vector>
using namespace std;

class Matrix {
    public:
    int rows, cols;
    vector<vector<double>> matrix; // this is references as A to simplify explanations
    
// constructor
    Matrix (int r, int c) {
        rows =r; cols=c;
    }
    // transposes the matrix
    void transpose() {
        vector<vector<double>> copy(cols, vector<double>(rows));
        for (int i = 0; i < cols; i++) {
            for(int j =0 ; j < rows;j++) {
                copy[i][j] = matrix[j][i];
            }
            
        }
        matrix = copy;
        swap(rows, cols); //changes dimension from (rows, cols) -> (cols, rows)
    }

    void print_matrix() {
        for(int i =0 ; i < rows; i++) {
            for(int j =0; j < cols; j++){
                if (j == 0) cout << matrix[i][j];
                cout << matrix[i][j] << ", ";
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

    }

     // performs subtraction between 2 matrices
    Matrix subtraction(Matrix & B) {
        Matrix r(rows, cols);
        // checking for the same dimensions
        if (!(rows == B.rows && cols == B.cols)) {
            cout << "Invalid Operation: Dimensions of both the matrices r not the same" << endl;
            return r;
        }
        for(int i = 0 ; i < rows; i ++ ) {
            for(int j =0; j < cols; j++) {
                r.matrix[i][j] = (matrix[i][j] - B.matrix[i][j]);
            }
        }

    }
    Matrix matmul(Matrix & B) {
        // performs matrix multiplication
        // valid operation when cols of A are equal to rows of B 
        Matrix r(rows, B.cols); // resultant matrix
        if (!(cols == B.rows)){
            cout << "Invalid Operation: This is not a valid operation as A.columns != B.rows" << endl;
        }


    }

};
