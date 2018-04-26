#pragma once
#include <mpi.h>
#include <string>

class MPICom{

public:
    MPI_Comm comm;
    size_t nprocs;
    size_t rank;

    MPICom( MPI_Comm comm, size_t nprocs, size_t rank ):comm(comm),nprocs(nprocs),rank(rank){
    }
    MPICom(int *argc, char *** argv){
        int rank;
        int inprocs;
        MPI_Init(argc, argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &inprocs);
        this->comm = MPI_COMM_WORLD;
        this->rank = (size_t)rank;
        this->nprocs = (size_t)inprocs;
    }
    ~MPICom(){

    }

    bool myturn(size_t i)const{
        return i == rank;
    }
    void barrier(){
        if ( MPI_SUCCESS != MPI_Barrier(comm)){
            throw std::string("barrier breakdown!");
        };
    }
    void finalize(){
        MPI_Finalize();
    }
};
