
#include <Ranges.h>
#include <iostream>
#include <cstddef>
#include <cmath>
#include "node_function.h"
using namespace std;
int main(int argc, char const *argv[]) {
    int length;
    if ( argc == 2 ){
        length = stoi(argv[1]);
    }else {
        cout << "no length specified, exiting";
        exit(1);
    }
    double *nums = new double[length];
    for ( auto num :  IntDoubleRange(0,length,1,1.0,201.0,1.0)){
        nums[num.first] = num.second;
    }
    double sum = node_function(nums,length);

    cout << "sum: "<< sum<< endl;
    cout << "sqrt(6*sum): "<< sqrt(6*sum)<<endl;
    return 0;
}
