#include <iostream>
#include <Ranges.h>
#include "../zeta0/zeta0.h"
using namespace std;
double node_function(double * data, size_t length){
    double sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < length; i++) {
        sum += zetas2( data[i]);
    }
    return sum;
}
