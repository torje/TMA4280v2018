#include <iostream>
#include <Ranges.h>
#include "../mach0/machin.h"
using namespace std;
double node_function(double * data, size_t length){
    double sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < length; i++) {
        sum += 4*machin( 1/5., data[i])-machin(1/239.,data[i]);
    }
    return sum;
}
