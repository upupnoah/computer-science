#CC = /usr/local/cs/bin/gcc
CC = gcc
CFLAGS= -Og
CINC =
F64 =-m64
FNOSP=-fno-stack-protector


.SUFFIXES: .c .64s .64d .64ns .64nd

.c.64s:
	$(CC) $(CFLAGS) $(CINC) -S $(F64) $*.c -o $*.64s

.c.64d:
	$(CC) $(CFLAGS) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64d
	rm -f $*

.c.64ns:
	$(CC) $(CFLAGS) $(FNOSP) $(CINC) -S $(F64) $*.c -o $*.64ns

.c.64nd:
	$(CC) $(CFLAGS) $(FNOSP) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64nd
	rm -f $*

files:	locate64 bufdemo.64s bufdemo.64d bufdemo.64ns bufdemo.64nd bufdemo-sp bufdemo-nsp

locate64: locate.c
	$(CC) $(CFLAGS) $(CINC) $(F64) locate.c -o locate64

bufdemo-sp: bufdemo.c
	$(CC) $(CFLAGS) $(CINC) $(F64) bufdemo.c -o bufdemo-sp

bufdemo-nsp: bufdemo.c
	$(CC) $(CFLAGS) $(FNOSP) $(CINC) $(F64) bufdemo.c -o bufdemo-nsp

clean:
	rm -f *~ bufdemo-* *.64s *.64d *.64ns *.64nd



