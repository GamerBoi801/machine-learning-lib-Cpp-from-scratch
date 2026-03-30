#include "matrix.h"
#include <iostream>
using namespace std;

int main() {
    
    Matrix b =  {
    {1.1, 1.2, 1.3},
    {2.1, 2.2, 2.3},
    {3.1, 3.2, 3.3}
};
    b.print_matrix();
    b.transpose();
    b.print_matrix();
    return 0;
}
