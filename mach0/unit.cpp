#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include <Ranges.h>
#include "machin.h"
using namespace std;
bool unittest_machin(){
    double sum{0};
    for( auto i : IntRange(1,4)){
        sum += 4*machin(1./5,i)-machin(1./239,i);
    }
    bool failure{false};
    double est_pi = 4*sum;
    if ( pow(est_pi-M_PI,2) > 0.001  ){
        failure = true;
    }
    cout << "Machin π estimate "<< est_pi <<", error: " << M_PI - est_pi<<" [ " << (failure?"fail":"pass") <<" ]"<<endl;
    return failure;
}
int main(int argc, char const *argv[]) {
    unittest_machin();
    return 0;
}
