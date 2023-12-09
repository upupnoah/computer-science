#CC = /usr/local/cs/bin/gcc
CC = gcc
OFLAGS = -Og
CFLAGS = $(OFLAGS) -Wall -g

CINC =
F64 =-m64


.SUFFIXES: .c .64s .64o .64d 


.c.64s:
	$(CC) $(OFLAGS) $(CINC) -S $(F64) $*.c -o $*.64s

.c.64d:
	$(CC) $(CFLAGS) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64d
	rm -f $*

files:	sum.64s	sum.64d	sum64 swap.64s arith.64s


sum64: sum.c
	$(CC) $(CFLAGS) $(CINC) $(F64) sum.c -o sum64

clean:
	rm -f *~ *.64s *.64d sum64
