CXXFLAGS=--std=c++11 -Iinclude
CXX=mpic++
all: utest vtest test

test: machin zeta
	./machin 16777216
	./zeta 16777216
machin: mach0/machin.o mach0/main.o
	$(CXX) -o $@ $^
zeta: zeta0/zeta0.o zeta0/main.o
	$(CXX) -o $@ $^

zeta_unit: zeta0/unit.o zeta0/zeta0.o
	$(CXX) -o $@ $^
machin_unit: mach0/machin.o mach0/unit.o
	$(CXX) -o $@ $^
utest: zeta_unit machin_unit
	./machin_unit
	./zeta_unit

machinverification: mach0/verification_test.o mach0/machin.o
	$(CXX) -o $@ $^
zetaverification: zeta0/verification_test.o zeta0/zeta0.o
	$(CXX) -o $@ $^
vtest: machinverification zetaverification
	./machinverification
	./zetaverification

zeta1_prog: zeta1/main.o zeta1/node_function.o zeta0/zeta0.o
	$(CXX) -o $@ $^
mach1_prog: mach1/main.o mach1/node_function.o mach0/machin.o
	$(CXX) -o $@ $^

zeta2/main_omp.o: zeta2/main.cpp
	$(CXX) $(CXXFLAGS) -fopenmp -c -o $@ $<
zeta2/node_function_omp.o:zeta2/node_function.cpp zeta2/node_function.h
	$(CXX) $(CXXFLAGS) -fopenmp -c -o $@ $<
zeta0/zeta0_omp.o: zeta0/zeta0.cpp zeta0/zeta0.h
	$(CXX) $(CXXFLAGS) -fopenmp -c -o $@ $<
zeta2_prog: zeta2/main_omp.o zeta2/node_function_omp.o zeta0/zeta0_omp.o
	$(CXX) -flto -O2 -fopenmp -o $@ $^


mach2/main_omp.o: mach2/main.cpp
	$(CXX) $(CXXFLAGS) -fopenmp -c -o $@ $<
mach2/node_function_omp.o:mach2/node_function.cpp mach2/node_function.h
	$(CXX) $(CXXFLAGS) -fopenmp -c -o $@ $<
mach2_prog: mach2/main_omp.o mach2/node_function_omp.o mach0/mach0.o
	$(CXX) -flto -O2 -fopenmp -o $@ $^

zeta3/main_omp.o: zeta3/main.cpp
	$(CXX) $(CXXFLAGS) -fopenmp -c -o $@ $<
zeta3_prog: zeta3/main_omp.o zeta2/node_function_omp.o zeta0/zeta0.o
	$(CXX) -flto -O2 -fopenmp -o $@ $^

mach3/main_omp.o: mach3/main.cpp
	$(CXX) $(CXXFLAGS) -fopenmp -c -o $@ $<
mach3_prog: mach3/main_omp.o mach2/node_function_omp.o mach0/machin.o
	$(CXX) -flto -O2 -fopenmp -o $@ $^

.PHONY clean:
	rm a.out *.o mach1_prog mach2_prog machin machin_unit machinverification zeta zeta1_prog zeta2_prog zeta3_prog zeta_unit zetaverification mach0/*.o mach1/*.o mach2/*.o mach3/*.o zeta0/*.o zeta1/*.o zeta2/*.o zeta3/*.o
