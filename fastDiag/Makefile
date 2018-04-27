CXXFLAGS=--std=c++11 -I../include -O2 -fopenmp
CXX=mpic++
CXXLFLAGS=-O2 -flto -fopenmp
#CXXLFLAGS=

all:poisson
poisson.o: poisson.cpp MpiCom.h matrix.h
	${CXX} ${CXXFLAGS} -c $<

poisson: poisson.o fst.o
	${CXX} ${CXXLFLAGS} $^ -o $@

transposetest.o: transposetest.cpp matrix.h MpiCom.h
	${CXX} ${CXXFLAGS} -c $<

transposetest: transposetest.o
	${CXX} ${CXXLFLAGS} $^ -o $@

smalltest: poisson 
	mpirun -np 4 ./poisson 256 > test256
	mpirun -np 4 ./poisson 512 > test512
	mpirun -np 4 ./poisson 1024 > test1024
	mpirun -np 4 ./poisson 2048 > test2048
	mpirun -np 4 ./poisson 4096 > test4096
