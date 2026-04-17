#include "matrix.h"
#include <iostream>
using namespace std;

int main() {
    vector<vector<double>> data = {
        {1.1, 1.2, 1.3},
        {2.1, 2.2, 2.3},
        {3.1, 3.2, 3.3}
    };
    Matrix b(data);
    b.print_matrix();

    Matrix b_t = b.transpose();
    b_t.print_matrix();

    return 0;
}