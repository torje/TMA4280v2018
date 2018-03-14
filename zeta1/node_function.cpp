#include <iostream>
#include <Ranges.h>
#include "../zeta0/zeta0.h"

double node_function(double * data, size_t length){
    for ( auto & number : DoubleMemRange(data,data+length)){
        number = zetas2( number);
    }
    double sum = 0;
    for ( auto  number : DoubleMemRange(data,data+length)){
        sum += number;
    }
    return sum;
}
