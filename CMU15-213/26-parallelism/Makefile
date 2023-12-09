CC = gcc

CFLAGS = -Og -Wall 
LDLIBS = -lpthread -lm

all: psum-mutex psum-array psum-local

psum-mutex: psum-mutex.c csapp.o
psum-array: psum-array.c csapp.o
psum-local: psum-local.c csapp.o

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

realtimer.o: realtimer.c realtimer.h
	$(CC) $(CFLAGS) -c realtimer.c

run-psum-mutex:
	/usr/bin/time ./psum-mutex 1 31 
	/usr/bin/time ./psum-mutex 2 31 
	/usr/bin/time ./psum-mutex 4 31 
	/usr/bin/time ./psum-mutex 8 31 
	/usr/bin/time ./psum-mutex 16 31

run-psum-array:
	/usr/bin/time ./psum-array 1 31 
	/usr/bin/time ./psum-array 2 31 
	/usr/bin/time ./psum-array 4 31 
	/usr/bin/time ./psum-array 8 31 
	/usr/bin/time ./psum-array 16 31 

run-psum-local:
	/usr/bin/time ./psum-local 1 31 
	/usr/bin/time ./psum-local 2 31 
	/usr/bin/time ./psum-local 4 31 
	/usr/bin/time ./psum-local 8 31 
	/usr/bin/time ./psum-local 16 31 

clean:
	rm -rf psum-mutex psum-local psum-array *.o *~

