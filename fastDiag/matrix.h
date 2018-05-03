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
    size_t height = 0;
    size_t width = 0;
    size_t easy_height = 0;
    size_t easy_width = 0;
    double * data = nullptr;
public:
    Matrix(MPICom mpicom, size_t height, size_t width ) : mpicom(mpicom),height(height),width(width){
        if ( ( height * width >= width ) && ( height * width >= height ) ) {
            easy_height = ( ( (height-1)/mpicom.nprocs)+1)*mpicom.nprocs;
            easy_width = ( ( (width-1)/mpicom.nprocs)+1)*mpicom.nprocs;
            local_height = easy_height / mpicom.nprocs;
            height_offset = local_height*mpicom.rank;
            data = new double[local_height*easy_width]{};
        }else{
            this->height = 0;
            this->width = 0;
            throw std::bad_array_new_length();
        }
    }
    Matrix(MPICom mpicom, size_t height, size_t width, std::function<double(double,double)> populate ) : Matrix(mpicom,height,width){
        #pragma omp parallel for
        for (size_t i = height_offset; i < height_offset+local_height; i++) {
            for (size_t j = 0; j < width; j++) {
                (*this)[i][j] = populate( i+1,j+1);
            }
        }
    }
    Matrix( const Matrix & matrix):Matrix(matrix.mpicom,matrix.height,matrix.width){
        memcpy( data, matrix.data, sizeof(double)*local_height*width);
    }
    Matrix & operator=( Matrix matrix){
        swap(mpicom,matrix.mpicom);
        swap(height,matrix.height);
        swap(width,matrix.width);
        swap(easy_height,matrix.easy_height);
        swap(easy_width,matrix.easy_width);
        swap(local_height,matrix.local_height);
        swap(data,matrix.data);
        swap(height_offset,matrix.height_offset);
    }
    ~Matrix(){
        delete [] data;
        easy_height = 0;
        easy_width = 0;
        height_offset = 0;
        local_height = 0;
        height = 0 ;
        width = 0;
        data = nullptr;
    }
    size_t index(size_t row)const{
        return (row - height_offset ) * easy_width;
    }
    double * operator[]( size_t row){
        return data+index(row);
    }
    const double * operator[]( size_t row)const{
        return data+index(row);
    }
    bool within(size_t a ){
        return a> height_offset && a < height_offset+local_height ;
    }
    void rowFST() {
        #pragma omp parallel for
        for (size_t i = height_offset; i < height_offset+local_height; i++) {
            int width_ = width+1;
            double *scratch = new double[width_*4];
            int bufferSize = 4*width_;
            fst_((*this)[i],&width_,scratch,&bufferSize);
            delete [] scratch;
        }
    }
    void rowIFST() {
        #pragma omp parallel for
        for (size_t i = height_offset; i < height_offset+local_height; i++) {
            int width_ = width+1;
            double *scratch = new double[width_*4];
            int bufferSize = 4*width_;
            fstinv_((*this)[i],&width_,scratch,&bufferSize);
            delete [] scratch;
        }
    }
    void packForSend(){
        Matrix dup(mpicom,height, width);
        for (size_t i = 0; i < mpicom.nprocs; i++) {
            size_t sourceDisp = local_height*i;
            size_t destDisp   = local_height*local_height*i;
            for (size_t j = 0; j < local_height; j++) {
                size_t rowSourceDisp = easy_width*j;
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
                dup.data[j*easy_width+i] = this->data[j+i*local_height];
            }
        }
        return dup;
    }
    Matrix transpose()  {
        packForSend();
        Matrix matrix(mpicom,height,width);
        MPI_Alltoall( data, local_height*easy_width/mpicom.nprocs, MPI_DOUBLE, matrix.data, local_height*easy_width/mpicom.nprocs,MPI_DOUBLE,mpicom.comm);
        return matrix.unpackAfterReceive();
    }

    void divByDiags( double * diag ){
        #pragma omp parallel for
        for (size_t i = height_offset; i <std::min(height_offset+local_height,height) ; i++) {
            for (size_t j = 0; j < width; j++) {
                (*this)[i][j] /= diag[i]+diag[j];
            }
        }
    }

    double max(){
        double max = 0;
        #pragma omp parallel for
        for (size_t i = height_offset ; i < std::min(height_offset+local_height,height); i++) {
            for (size_t j = 0 ; j < width ; j++){
                if ( max < abs((*this)[i][j]) ){
                    max = abs((*this)[i][j]);
                }
            }
        }
        double allmax;
        MPI_Allreduce(&max,&allmax,1,MPI_DOUBLE,MPI_MAX,mpicom.comm);
        return allmax;
    }
    void printImshow(ostream & out)const{
        if ( mpicom.rank == 0){
            double h = 1.0/(width+1);
            out << "import matplotlib.pyplot as plt"<<endl
            << "import numpy as np"<< endl
            <<"from matplotlib import cm"<<endl
            <<"from mpl_toolkits.mplot3d import axes3d"<<endl
            <<"fig = plt.figure()"<<endl
            << "xs,ys  = np.meshgrid(np.linspace("<<h<<","<<1-h<<","<<width<<"),np.linspace("<<h<<","<<1-h<<","<<height<<"))"<<endl
            << "print(xs.shape)"<<endl
            <<"ax  = fig.add_subplot(111,projection='3d')"<<endl
            << "try:"<<endl
            <<"\tax.plot_surface(xs,ys,[";
            Matrix matrix (mpicom, height,width);
            out << this->toImshow();
            for (size_t i = 1; i < mpicom.nprocs; i++) {
                MPI_Status status;
                MPI_Recv(matrix.data, local_height*easy_width, MPI_DOUBLE, i,0,mpicom.comm,&status);
                MPI_Recv(&matrix.height_offset,1,MPI_DOUBLE,i,0,mpicom.comm,&status);
                string strrep = ","+matrix.toImshow();
                out << strrep;
            }
            out <<"],cmap=cm.coolwarm)"<<endl
            << "\tax.view_init(30, 45)"<<endl
            <<"\tplt.show()" <<endl
            << "except:"<<endl
            << "\tprint('dieded:',xs)"<<endl;
        }
        else{
            MPI_Send( data,local_height*easy_width, MPI_DOUBLE,0,0,mpicom.comm);
            MPI_Send(&height_offset,1,MPI_DOUBLE,0,0,mpicom.comm);
        }
    }
    std::string toImshow()const{
        std::string str = "";
        for (size_t i = height_offset; i < std::min(height_offset+local_height,height); i++) {
            str += "[";
            for (size_t j = 0; j < width; j++) {
                str += std::to_string((*this)[i][j])+",";
            }
            str.pop_back();
            str += "],";
        }

        str.pop_back();
        return str;
    }

    std::string toString()const{
        //std::string str = "process:" + to_string(mpicom.rank) + "\n";
        std::string str = "p"+to_string(mpicom.rank)+ "size:" +to_string(height)+ " x " +to_string(width);
        for (size_t i = height_offset; i < std::min(height_offset+local_height,height); i++) {
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
