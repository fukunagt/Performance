.PHONY: all
all: clpperfchk clean

clpperfchk: clpperfchk.o 
	cc -o clpperfchk clpperfchk.o
clpperfchk.o: main.c
	cc -c main.c -o clpperfchk.o

clpmdperf.o: mdperf.h

.PHONY: clean
clean:
	rm -f clpperfchk.o 
