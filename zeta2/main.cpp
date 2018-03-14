#include <Ranges.h>
#include <iostream>
#include <cstddef>
#include <omp.h>
#include <cmath>
#include "node_function.h"
using namespace std;
int main(int argc, char  *argv[]) {
    if ( argc == 2 ){
        omp_set_num_threads(4);
        int length = stoi(argv[1]);
        double *nums = new double[length];
        #pragma omp parallel for
        for (size_t i = 0; i < length; i++) {
            nums[i] = i+1;
        }
        double sum = node_function(nums,length);
        cout << "π - π_estimate: " << M_PI - sqrt(6*sum)<< endl;
    return 0;

    }else{
        cout << "no length specified, exiting"<< endl;
        exit(0);
    }

}
