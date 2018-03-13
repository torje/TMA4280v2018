#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include <Ranges.h>
#include "zeta0.h"

using namespace std;

bool verification(){
    double sum{0};
    bool failure{false};
    {
        double step_prev = 10;
        for( int i : IntRange(1,1<<24)){
            double step_curr =  zetas2(i);
            sum += step_curr;

            /* The Machin estimate rapidly diminishes and will go below the
            treshold at which comparison with M_PI is meaningful. We can still
            check that the magnitude i decreasing. */
            if (  copysign(step_prev,1) < copysign(step_curr,1) ) {
                failure = true;
                cout << "At step "<< i << " magnitude increased"<<endl;
                cout << "|"<<step_prev<< "| < |"<< step_curr <<"|"<<endl;
            }
            step_prev = step_curr;
        }
    }
    double est_pi = sqrt(6*sum);
    if ( copysign(M_PI - est_pi,1) > 0.0000001 ){
        failure = 1;
    }
    cout << "Zeta π estimate: "<< M_PI-est_pi << " [ " << (failure?"failure":"pass" )<<" ]"<<endl;
    return failure;
}

int main(int argc, char const *argv[]) {
    bool failure = verification();
    return failure;
}
