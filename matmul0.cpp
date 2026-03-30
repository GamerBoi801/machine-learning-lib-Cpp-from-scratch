#include <iostream>
#include <iomanip>
using namespace std;

class Matrix {
public:
    int rows, cols;
    int** structure;

    Matrix(int r, int c) {
        rows = r; cols = c;
        structure = new int*[rows];
        for(int i = 0; i < rows; i++) 
            structure[i] = new int[cols];
    }

    void fill_matrix() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                cout << "Enter integer at row " << (i+1) << ", col " << (j+1) << ": ";
                cin >> structure[i][j];
            }
        }
    }

    void ini() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                structure[i][j] = 0;
            }
        }
    }

    void print_matrix() {
        for(int i = 0; i < rows; i++) {
            cout << "\n [";
            for(int j = 0; j < cols; j++) {
                cout << setw(4) << structure[i][j] << ", ";
            }
            cout << "],";
        }
        cout << endl;
    }

    ~Matrix() {
        for(int i = 0; i < rows; i++) delete[] structure[i];
        delete[] structure;
    }
};

void matmul(Matrix &A, Matrix &B) {
    if(A.cols != B.rows) {
        cout << "Error: columns A (" << A.cols << ") != rows B (" << B.rows << ")" << endl;
        return;
    }

    Matrix C(A.rows, B.cols);
    C.ini();

    for(int i = 0; i < C.rows; i++) {
        for(int j = 0; j < C.cols; j++) {
            for(int k = 0; k < A.cols; k++) {
                C.structure[i][j] += A.structure[i][k] * B.structure[k][j];
            }
        }
    }

    cout << "\nResult Matrix:\n";
    C.print_matrix();
}

int main() {
    cout << "<==================== CPP matmul ============>\n";

    cout << "Enter the first matrix rows: ";
    int a_rows; cin >> a_rows;
    cout << "Enter the first matrix cols: ";
    int a_cols; cin >> a_cols;

    Matrix A(a_rows, a_cols);
    cout << "\nMatrix A:\n";
    A.fill_matrix();
    A.print_matrix();

    cout << "Enter the second matrix rows: ";
    int b_rows; cin >> b_rows;
    cout << "Enter the second matrix cols: ";
    int b_cols; cin >> b_cols;

    Matrix B(b_rows, b_cols);
    cout << "\nMatrix B: \n";
    B.fill_matrix();
    B.print_matrix();

    matmul(A, B);
    return 0;
}
