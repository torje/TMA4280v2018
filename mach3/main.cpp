#include <mpi.h>
#include <Ranges.h>
#include <iostream>
#include <cstddef>
#include <omp.h>
#include <cmath>
#include "../mach2/node_function.h"
using namespace std;
int main(int argc, char  *argv[]) {
    MPI_Init(&argc, &argv);
    int world_size;
    MPI_Comm_size( MPI_COMM_WORLD, &world_size );
    int world_rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &world_rank );
    if ( argc == 2 ){
        omp_set_num_threads(4);
        double *gnums = nullptr;
        int glength = stoi(argv[1]);
        int length = glength/world_size;
        double *nums = new double[length];
        if ( world_rank == 0){
            gnums = new double[glength];
            #pragma omp parallel for
            for (size_t i = 0; i < glength; i++) {
                gnums[i] = i+1;
            }
        }
        MPI_Scatter(gnums, length,MPI_DOUBLE, nums,length, MPI_DOUBLE,0,MPI_COMM_WORLD);
        double sum = node_function(nums,length);
        MPI_Gather(&sum,1,MPI_DOUBLE,gnums,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        if ( 0 == world_rank){
            sum = 0;
            #pragma omp parallel for
            for ( int i = 0 ; i < world_size;i++ ){
                sum += gnums[i];
            }
            cout << "π - π_estimate: " <<M_PI- 4*sum << endl;
        }
        MPI_Finalize();
        return 0;
    }else{
        if ( 0==world_rank ){
            cout << "invovation: " << argv[0] << " <steps>"<< endl;
        }
        MPI_Finalize();
        exit(0);
    }

}
