#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include <Ranges.h>
#include "zeta0.h"
using namespace std;

int main(int argc, const char *const* argv){
    double sum = 0 ;
    for ( auto i : IntRange(1,6) ) {
        sum += zetas2( i );
    }
    cout << "sum: "<< sum<< endl;
    cout << "sqrt(6*sum): "<< sqrt(6*sum)<<endl;
    return 0;
}
