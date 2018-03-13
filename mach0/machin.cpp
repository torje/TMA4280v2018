#include <cmath>
double machin( double x, double i){
    double sign = pow(-1,i-1);
    double numerator = pow(x,2*i-1);
    double denominator = 2*i-1;
    return sign*numerator/denominator;
}
