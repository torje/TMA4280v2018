#include <iostream>
#include <cmath>
#include "zeta0.h"
#include <Ranges.h>

using namespace std;
int unittest_zetas2( ){
    double sum_m1 = 0;
    double sum = 0;
    bool failure = 0;
    for( auto i : IntRange(1,4)){
        double step_i = zetas2( i);
        sum_m1 = sum;
        sum += step_i;
        if ( sum < sum_m1){
            failure = 1;
        }
    }
    double est_pi = sqrt(6*sum);
    /* as all evaluations will we positive, the estimate will forever be below π
    and M_PI > estimate will hold at least untill the error drops below ~ 10e-16
    */
    if ( M_PI < est_pi){
        failure = 1;
    }
    /* This π estimate converges horribly slow, 2.85774 is the result we get */
    if ( 2.8577 > est_pi ){
        failure = 1;
    }
    cout << "Zeta function π estimate: "<< est_pi << ", error: "<< M_PI-est_pi<<" [ " << (failure?"failure":"pass" )<<" ]"<<endl;
    return failure;
}

int main(int argc, const char *const* argv){
    int failure = unittest_zetas2();
    return failure;
}
