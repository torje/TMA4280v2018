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

.PHONY clean:
	rm -rf a.out *.o
