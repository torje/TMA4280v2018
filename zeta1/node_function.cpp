#include <iostream>
#include <Ranges.h>
#include "../zeta0/zeta0.h"
using namespace std;
double node_function(double * data, size_t length){
    for ( auto & number : DoubleMemRange(data,data+length)){
        number = zetas2( number);
    }
    cout << "passed first part"<<endl;
    double sum = 0;
    for ( auto  number : DoubleMemRange(data,data+length)){
        sum += number;
    }
    return sum;
}
