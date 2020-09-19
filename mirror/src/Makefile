.PHONY: all
all: clpmdperf clean

clpmdperf: clpmdperf.o 
	cc -o clpmdperf clpmdperf.o
clpmdperf.o: main.c
	cc -c main.c -o clpmdperf.o

clpmdperf.o: mdperf.h

.PHONY: clean
clean:
	rm -f clpmdperf.o 
