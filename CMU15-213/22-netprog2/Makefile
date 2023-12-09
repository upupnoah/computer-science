CC=gcc
CFLAGS=-g -O1 -Wall
LDLIBS=-lpthread

all: hostinfo echoclient echoserveri 

echoclient: echoclient.c csapp.h csapp.c
echoserveri: echoserveri.c echo.c csapp.h csapp.c
hostinfo: hostinfo.c csapp.h csapp.c

clean:
	rm -f *.o *~ *.exe hostinfo echoclient echoserveri csapp.o

