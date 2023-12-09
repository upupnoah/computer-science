CC = gcc
CFLAGS=-O2 -Wall
LDLIBS = -lpthread

all: echoclient echoserveri echoserverp echoservert race

echoclient: echoclient.c csapp.c csapp.h
echoserveri: echoserveri.c echo.c csapp.c csapp.h
echoserverp: echoserverp.c echo.c csapp.c csapp.h
echoservert: echoservert.c echo.c csapp.c csapp.h
race: race.c csapp.c

clean:
	rm -rf *~ echoclient echoserveri echoserverp echservert race *.o
