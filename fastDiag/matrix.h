#include <mpi.h>
#include <string.h>
#include <functional>
#include "MpiCom.h"

using namespace std;
extern "C"{
    void fst_(double *v, int *n, double *w, int *nn);
    void fstinv_(double *v, int *n, double *w, int *nn);
}

class Matrix{
    MPICom mpicom;
    size_t height_offset = 0;
    size_t local_height = 0;
    size_t height = 0 , width = 0;
    double * data = nullptr;
public:
    Matrix(MPICom mpicom, size_t height, size_t width ) : mpicom(mpicom),height(height),width(width){
        if ( ( height * width >= width ) && ( height * width >= height ) ) {
            height_offset = (height / mpicom.nprocs)*mpicom.rank;
            local_height = height / mpicom.nprocs;
            data = new double[local_height*width]{};
            //MPI_Type_vector(mpicom.nprocs, width/mpicom.nprocs, width, MPI_DOUBLE, &sendtype);
            //MPI_Type_commit(&sendtype);
        }else{
            this->height = 0;
            this->width = 0;
            throw std::bad_array_new_length();
        }
    }
    Matrix(MPICom mpicom, size_t height, size_t width, std::function<double(double,double)> populate ) : Matrix(mpicom,height,width){
        for (size_t i = height_offset; i < height_offset+local_height; i++) {
            for (size_t j = 0; j < width; j++) {
                (*this)[i][j] = populate( i+1,j+1);
            }
        }
    }
    Matrix( const Matrix & matrix):Matrix(matrix.mpicom,matrix.height,matrix.width){
        memcpy( data, matrix.data, sizeof(double)*local_height*width);
    }
    size_t index(size_t row)const{
        return (row - height_offset ) * width;
    }
    double * operator[]( size_t row){
        return data+index(row);
    }
    const double * operator[]( size_t row)const{
        return data+index(row);
    }
    void rowFST() {
        for ( int i  = height_offset ; i < height_offset+local_height ; i++){
            int width_ = width+1;
            double *scratch = new double[width_*4*4*4];
            int bufferSize = 4*width_;
            fst_((*this)[i],&width_,scratch,&bufferSize);
            delete [] scratch;
        }
    }
    void rowIFST() {
        for ( int i  = height_offset ; i < height_offset+local_height ; i++){
            int width_ = width+1;
            double *scratch = new double[width_*4];
            int bufferSize = 4*width_;
            fstinv_((*this)[i],&width_,scratch,&bufferSize);
            //delete [] scratch;
        }
    }
    void packForSend(){
        Matrix dup(mpicom,height, width);
        for (size_t i = 0; i < mpicom.nprocs; i++) {
            size_t sourceDisp = local_height*i;
            size_t destDisp   = local_height*local_height*i;
            for (size_t j = 0; j < local_height; j++) {
                size_t rowSourceDisp = width*j;
                size_t rowDestDisp = local_height*j;
                memcpy(&dup.data[rowDestDisp+destDisp],&data[rowSourceDisp+sourceDisp], sizeof(double)*local_height);
            }
        }
        memcpy( data, dup.data, local_height*width*sizeof(double));
    }
    Matrix unpackAfterReceive(){
        Matrix dup(mpicom,height, width);
        size_t step = 0;
        size_t j = 0;
        for (size_t j = 0; j < local_height; j++) {

            for (size_t i = 0; i < width; i++) {
                dup.data[j*width+i] = this->data[j+i*local_height];
            }
        }

        return dup;
    }
    Matrix transpose()  {
        packForSend();
        Matrix matrix(mpicom,height,width);
        MPI_Alltoall( data, local_height*width/mpicom.nprocs, MPI_DOUBLE, matrix.data, local_height*width/mpicom.nprocs,MPI_DOUBLE,mpicom.comm);
        //return unpackAfterReceive();
        return matrix.unpackAfterReceive();
    }

    void divByDiags( double * diag ){
        for (size_t i = height_offset; i < height_offset+local_height; i++) {
            for (size_t j = 0; j < width; j++) {
                (*this)[i][j] /= diag[i]+diag[j];
            }
        }
    }

    double max(){
        double max = 0;
        for (size_t i = 0; i < local_height*width; i++) {
            if ( max < abs(data[i]) )
            max = abs(data[i]);
        }
        MPI_Allreduce(data,&max,1,MPI_DOUBLE,MPI_MAX,mpicom.comm);
        return max;
    }

    std::string toImshow()const{
        std::string str = "plt.imshow([";
        for (size_t i = height_offset; i < height_offset+local_height; i++) {
            str += "[";
            for (size_t j = 0; j < width; j++) {
                str += std::to_string((*this)[i][j])+",";
            }
            str.pop_back();
            str += "],";
        }
        str.pop_back();
        str += "])\nplt.show()";
        return str;
    }

    std::string toString()const{
        //std::string str = "process:" + to_string(mpicom.rank) + "\n";
        std::string str = "";
        for (size_t i = height_offset; i < height_offset+local_height; i++) {
            str += "[";
            for (size_t j = 0; j < width; j++) {
                str += std::to_string((*this)[i][j])+" ";
            }
            str += "]\n";
        }
        return str;
    }

    friend std::ostream & operator<<( std::ostream & out, const Matrix & matrix){
        for (size_t i = 0; i < matrix.  mpicom.nprocs; i++) {
            if ( matrix.mpicom.rank == i ){
                out << matrix.toString();
            }
            MPI_Barrier(matrix.mpicom.comm);
        }
        return out;// << matrix.toString();
    }
};
