#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include "Ranges.h"
#include "machin.h"
using namespace std;

int main(int argc, const char *const* argv){
    double sum = 0 ;
    int maxNum = 1<<24;
    double *parts = new double[maxNum]{};
    for ( auto i : IntRange(1,maxNum) ) {
        double s = 4 * machin( 1/5., i ) - machin(1/239.,i);
        parts[i] = s;
    }
    for ( auto si : DoubleMemRange( parts, parts+maxNum )){
        sum+=si;
    }
    cout << "sum: "<< sum<< endl;
    cout << "4*sum: "<< 4*sum<<endl;
    cout << "π - π_estimate: " << 4*sum - M_PI<< endl;
    return 0;
}
