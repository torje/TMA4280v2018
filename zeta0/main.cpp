#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include <Ranges.h>
#include "zeta0.h"
using namespace std;

int main(int argc, const char *const* argv){
    int length;
    if ( argc == 2 ){
        length = stoi(argv[1]);
    }else {
        cout << "no length specified, exiting";
        exit(1);
    }
    double * data = new double[length];
    for ( auto i : IntRange(0,length)) {
        data[i] = i+1;
    }
    double sum = 0 ;
    for ( auto i : DoubleMemRange( data,data+length) ) {
        sum += zetas2( i );
    }
    cout << "sum: "<< sum<< endl;
    cout << "sqrt(6*sum): "<< sqrt(6*sum)<<endl;
    return 0;
}
