#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include "Ranges.h"
#include "machin.h"
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
    double *parts = new double[length]{};
    for ( auto i : DoubleMemRange(data,data+length) ) {
        sum += 4 * machin( 1/5., i ) - machin(1/239.,i);
    }
    cout << "sum: "<< sum<< endl;
    cout << "4*sum: "<< 4*sum<<endl;
    cout << "π - π_estimate: " << 4*sum - M_PI<< endl;
    return 0;
}
