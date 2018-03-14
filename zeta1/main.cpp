
#include <mpi.h>
#include <Ranges.h>
#include <iostream>
#include <cstddef>
#include <cmath>
#include "node_function.h"
using namespace std;
int main(int argc, char  *argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size( MPI_COMM_WORLD, &world_size );

    int world_rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &world_rank );
    //cout << world_size<< endl;
    //cout << world_rank<< endl;
    cout << argv[1]<<endl;
    if ( argc == 2 ){
        int glength = stoi(argv[1]);
        int length = glength/world_size;
        cout << "lenght and glength is consistent: "<<(length*world_size == glength)<<endl;
        double *gnums = nullptr;
        double *nums = new double[length];
        if ( world_rank == 0){
            gnums = new double[glength];
            for ( auto num :  IntDoubleRange(0,length,1,1.0,201.0,1.0)){
                gnums[num.first] = num.second;
            }
        }
        MPI_Scatter(gnums, length,MPI_DOUBLE, nums,length, MPI_DOUBLE,0,MPI_COMM_WORLD);
        double sum = node_function(nums,length);
        cout << "Node: "<< world_rank<< "sum: "<< sum<< endl;
        cout << "Node: "<< world_rank<< "sqrt(6*sum): "<< sqrt(6*sum)<<endl;
        MPI_Finalize();
        return 0;

    }else{
        cout << "no length specified, exiting"<< endl;
        MPI_Finalize();
        exit(1);
    }

}
