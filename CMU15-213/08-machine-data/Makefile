CC=gcc
CFLAGS= -Og -Wall -msse3
COFLAGS = -O1 -Wall -msse3
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

files:	zip.64s array.64s structure.64s struct_eg.64s float.64s

zip: zip.c
	$(CC) $(CFLAGS) $(CINC) zip.c -o zip

float: float.c
	$(CC) $(CFLAGS) $(CINC) float.c -o float

clean:
	rm -f *~
	rm -f *.64s *.64o *.64d *.O1-64s
	rm -f zip float

