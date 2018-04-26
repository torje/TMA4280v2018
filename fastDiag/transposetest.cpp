#define use_math_defines
#include <ctgmath> // C library, typegeneric math: specialized functions for
//float and doubles.
#include <iostream>
#include <fstream>
#include <exception>
#include <new>
#include <mpi.h>
#include <string>
#include <Ranges.h>
#include "MpiCom.h"
#include "matrix.h"

void foo(int argc, char**argv){

    MPICom mpicom(&argc,&argv);
    double h = 1./8;
    auto populate = [h](double y, double x){
        return y*10+x;
        //return h*h*2*(h*y - h*h*y*y + h*x - h*x*h*x);
    };
    Matrix b(mpicom,8,8,populate);
    cout << b;
    Matrix bt = b.transpose();
    mpicom.barrier();
    cout << bt;
    mpicom.finalize();
}
int main(int argc, char**argv){
    foo(argc,argv);
}
