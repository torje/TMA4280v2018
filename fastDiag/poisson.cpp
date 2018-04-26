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
#include <chrono>
#include "MpiCom.h"
#include "matrix.h"

/*
* This function is used for initializing the right-hand side of the equation.
* Other functions can be defined to swtich between problem definitions.
*/

bool near(double x,double y,double x1,double y1){
    return abs( x1 - x ) < 0.1 && abs( y1 - y ) < 0.1;
}
double electron(double x, double y){
    if ( near(x,y,0.48,0.48)){
        return 1;
    }else{
        return 0;
    }
}
double proton(double x, double y){
    if ( near(x,y,0.52,0.52)){
        return -1;
    }else{
        return 0;
    }
}
double rhs(double x, double y) {
    return proton(x,y)+electron(x,y);
    //return 2*(y - y*y + x - x*x);
}

int main(int argc, char ** argv){
    MPICom mpicom(&argc,&argv);
    using namespace std::chrono;
    int n = atoi(argv[1]);
    int m = n-1;
    double h = 1.0/n;
    double * grid = new double [n+1];
    steady_clock::time_point t[12];
    auto populate = [h](double y, double x){
        return h*h*2*(h*y - h*h*y*y + h*x - h*h*x*x);
        //return h*h*2*(h*y - h*h*y*y + h*x - h*x*h*x);
    };
    string timeDescs[11] =
    {
    "finding eigenvalues:  ",
    "initializing matrix:  ",
    "rowFST:               ",
    "transpose:            ",
    "rowIFST:              ",
    "dividing by diag:     ",
    "rowFST:               ",
    "transpose:            ",
    "rowIFST:              ",
    "finding max:          ",
    "total:                "};
    for( int i = 0 ; i < n+1; i++ ){
        grid[i] = i*h;
    }
    t[0] = steady_clock::now();
    double *diag = new double[m];
    for (size_t i = 0; i < m; i++) {
        diag[i] = 2.0 * (1.0 - cos((i+1) * M_PI / n));
    }
    t[1] = steady_clock::now();
    Matrix b(mpicom,m,m,populate);
    t[2] = steady_clock::now();
    b.rowFST();
    t[3] = steady_clock::now();
    Matrix bt = b.transpose();
    t[4] = steady_clock::now();
    bt.rowIFST();
    t[5] = steady_clock::now();
    bt.divByDiags(diag);
    t[6] = steady_clock::now();
    bt.rowFST();
    t[7] = steady_clock::now();
    b = bt.transpose();
    t[8] = steady_clock::now();
    b.rowIFST();
    t[9] = steady_clock::now();
    double u_max = b.max();
    t[10] = steady_clock::now();
    //ofstream outdata("outdata.py");
    //outdata <<"import matplotlib.pyplot as plt"<<endl;
    //outdata << b.toImshow()<<endl;
    if ( mpicom.rank == 0){
        cout << "Max value: " << u_max<< endl;

        for (size_t i = 1; i < 11; i++) {
            cout << timeDescs[i-1] << duration_cast<duration<double>>(t[i]-t[i-1]).count()<< " seconds" << endl;
        }
        cout << timeDescs[11-1] << duration_cast<duration<double>>(t[10]-t[0]).count()<< " seconds" << endl;
    }

    mpicom.finalize();
    return 0;
}
