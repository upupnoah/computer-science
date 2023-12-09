#CC = /usr/local/cs/bin/gcc
CC = gcc
OFLAGS = -Og
O1FLAGS = -O1
CFLAGS = $(OFLAGS) -Wall

CINC =
F64 =-m64

NCMOV = -fno-if-conversion


.SUFFIXES: .c .64s .64bs .O1-64s .64o .64d


.c.64s:
	$(CC) $(OFLAGS) $(CINC) -S $(F64) $*.c -o $*.64s

.c.O1-64s:
	$(CC) $(O1FLAGS) $(CINC) -S $(F64) $*.c -o $*.O1-64s

.c.64bs:
	$(CC) $(OFLAGS) $(CINC) -S $(F64) $(NCMOV) $*.c -o $*.64bs

.c.64o:
	$(CC) $(OFLAGS) $(CINC) $(F64) -C $*.c -o $*.64o



.c.64d:
	$(CC) $(CFLAGS) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64d
	rm -f $*


all: files

files:	control.64bs control.64s loop.64s loop.O1-64s switch.64s

clean:
	rm -f *~ *.64s *.64d *.64bs *.O1-64s

