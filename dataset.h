#include "matrix.h"
#include <iostream>
#include <math.h>
using namespace std;

class Dataset {
    public:
    Matrix X; // features[m x n] becomes [m x n+1] after bias
    Matrix Y; // targets [m x 1]
    int m; // number of samples
    int n; // number of features

};