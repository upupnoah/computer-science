CC = gcc

CFLAGS = -O1 -Wall -I .
LDLIBS = -lpthread -lm

PROGS =	echoservert_pre\
	race\
	norace\

all: $(PROGS)

echoservert_pre: echoservert_pre.c echo_cnt.c sbuf.c csapp.c csapp.h
race: race.c csapp.c csapp.h
norace: norace.c csapp.c csapp.h

clean:
	rm -f $(PROGS) *.o *~
