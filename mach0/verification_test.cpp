#include <iostream>
#include <cmath>
#include "machin.h"
#include "Ranges.h"

using namespace std;

bool quadraticErrorLess( double value, double compare, double maxError ){
    return pow( value - compare,2) < maxError;

}

int verification_machin( ){
    double sum_m1 = 0;
    double sum = 0;
    bool failure = false;
    bool failure_quad_error = false;

    {
        double step_prev = 10;
        for( auto i : IntRange(1,1<<24)){
            double step_curr = 4*machin(1./5,i) - machin(1./239,i);
            sum += step_curr;

            /* The Machin estimate rapidly diminishes and will go below the
            treshold at which comparison with M_PI is meaningful. We can still
            check that the magnitude i decreasing. */
            if (  copysign(step_prev,1) < copysign(step_curr,1) ) {
                failure_quad_error = true;
                cout << "At step "<< i << " magnitude increased"<<endl;
                cout << "|"<<step_prev<< "| < |"<< step_curr <<"|"<<endl;
            }
            step_prev = step_curr;
        }
    }
    double est_pi = sum*4;
    if ( copysign(M_PI - est_pi,1) > 0.00000000000001 ){
        failure = 1;
    }
    cout << " π - Pi estimate : "<<  M_PI-est_pi << " [ " << (failure|failure_quad_error?"failure":"pass" )<<" ]"<<endl;
    cout << (failure_quad_error? "not decreasing correctly\n":"");
    return failure;
}

int main(int argc, const char *const* argv){
    int failure = verification_machin();
    return failure;
}
