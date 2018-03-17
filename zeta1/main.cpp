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
    if ( 0 == world_rank ){
	system("date");
    }
    if ( argc == 2 ){
        int glength = stoi(argv[1]);
        int length = glength/world_size;
        double *gnums = nullptr;
        double *nums = new double[length];
        if ( world_rank == 0){
            gnums = new double[glength];
            for ( auto num :  IntDoubleRange(0,glength,1,1.0,201.0,1.0)){
                gnums[num.first] = num.second;
            }
        }
        MPI_Scatter(gnums, length,MPI_DOUBLE, nums,length, MPI_DOUBLE,0,MPI_COMM_WORLD);
        double sum = node_function(nums,length);
        MPI_Gather(&sum,1,MPI_DOUBLE,gnums,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        if ( 0 == world_rank){
            sum = 0;
            for ( auto i : IntRange(0,world_size)){
                sum += gnums[i];
            }
	    if ( 0 == world_rank ){
		    system("date");
	    }
	    cout << "π - π_estimate: " << M_PI - sqrt(6*sum)<< endl;
	}
	MPI_Finalize();
	return 0;

    }else{
	    cout << "no length specified, exiting"<< endl;
	    MPI_Finalize();
	    exit(0);
    }

}
