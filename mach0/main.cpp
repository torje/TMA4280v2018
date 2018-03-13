#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include "Ranges.h"
#include "machin.h"
using namespace std;

int main(int argc, const char *const* argv){
    double sum = 0 ;
    int maxNum = 4;
    double *parts = new double[maxNum]{};
    for ( auto i : IntRange(1,maxNum) ) {
        double s = 4 * machin( 1/5., i ) - machin(1/239.,i);
        sum += s;
        parts[i] = s;
        cout <<i<<": "<< 4 * machin( 1/5.,i )- machin(1/239.,i)<< endl;
    }
    cout << "sum: "<< sum<< endl;
    cout << "4*sum: "<< 4*sum<<endl;
    return 0;
}
