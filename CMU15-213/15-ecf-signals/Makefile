CC = gcc
CFLAGS = -Og
LDLIBS = -lpthread

PROGS = forks execs internal shellex signaldeadlock loop sigint sigintsafe procmask1 procmask2 waitforsignal sigsuspend setjmp restart

all: $(PROGS)

signaldeadlock: signaldeadlock.c csapp.c
shellex: shellex.c csapp.c
forks: forks.c csapp.c
execs: execs.c
internal: internal.c csapp.c
sigint: sigint.c csapp.c
sigintsafe: sigintsafe.c csapp.c
loop: loop.c
procmask1: procmask1.c csapp.c
procmask2: procmask2.c csapp.c
waitforsignal: waitforsignal.c csapp.c
sigsuspend: sigsuspend.c csapp.c
setjmp: setjmp.c csapp.c
restart: restart.c csapp.c

clean:
	rm -rf *~ $(PROGS)

