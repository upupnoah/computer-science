CC=gcc
CFLAGS= -Og -Wall
COFLAGS = -O1 -Wall
CINC =
F64 =-m64


.SUFFIXES: .c .64s .O1-64s .64o .64d


.c.64s:
	$(CC) $(CFLAGS) $(CINC) -S $(F64) $*.c -o $*.64s

.c.O1-64s:
	$(CC) $(COFLAGS) $(CINC) -S $(F64) $*.c -o $*.O1-64s

.c.64d:
	$(CC) $(CFLAGS) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64d
	rm -f $*

files:	incr.64s mstore-exe.64d recurse.64s

mstore-exe.64d: mstore.c main.c
	$(CC) $(CFLAGS) $(CINC) $(F64) mstore.c main.c -o mstore
	objdump -d mstore > mstore-exe.64d
	rm -f mstore

clean:
	rm -f *~
	rm -f *.64s *.64o *.64d
	rm -f incr mstore

