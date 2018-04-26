CXXFLAGS=--std=c++11 -I../include -O2 -g
CXX=mpic++
#CXXLFLAGS=-O2 -flto -g
CXXLFLAGS=-g

all:poisson
poisson.o: poisson.cpp MpiCom.h matrix.h
	${CXX} ${CXXFLAGS} -c $<

poisson: poisson.o fst.o
	${CXX} ${CXXLFLAGS} $^ -o $@

transposetest.o: transposetest.cpp matrix.h MpiCom.h
	${CXX} ${CXXFLAGS} -c $<

transposetest: transposetest.o
	${CXX} ${CXXLFLAGS} $^ -o $@
