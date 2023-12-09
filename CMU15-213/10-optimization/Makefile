CC=gcc
CFLAGS= -Og -Wall
COFLAGS = -O1 -Wall
CINC =
F64 =-m64

.SUFFIXES: .c .64s .O1-64s .64o .64d


.c.64s:
	$(CC) $(CFLAGS) $(CINC) -S $(F64) $*.c -o $*.$(MACHINE)64s


.c.O1-64s:
	$(CC) $(COFLAGS) $(CINC) -S $(F64) $*.c -o $*.O1-64s

.c.64d:
	$(CC) $(CFLAGS) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64d
	rm -f $*

files:	set_row.O1-64s subexpr.64s subexpr.O1-64s sum_rows.O1-64s

clean:
	rm -f *~
	rm -f *.64s *.O1-64s *.64o *.64d
