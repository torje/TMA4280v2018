#include <iostream>
#include <Ranges.h>
#include "../mach0/machin.h"
using namespace std;
double node_function(double * data, size_t length){
    for ( auto & number : DoubleMemRange(data,data+length)){
        number = 4 * machin( 1/5., number ) - machin(1/239.,number);
    }
    double sum = 0;
    for ( auto  number : DoubleMemRange(data,data+length)){
        sum += number;
    }
    return sum;
}
