Programs used in lecture to demonstrate signals


csapp.{c,h}
        CS:APP3e code library

execs.c
forks.c
        Demonstrate exec, fork, wait, and signals

shellex.c
        Simple shell example

sigint.c
sigintsafe.c
        Unsafe and safe versions of program that catches SIGINT

signaldeadlock.c

        Demonstrates deadlock caused by using unsafe printf in signal
        handler

procmask1.c
        Race condition caused by assuming parent runs before child
procmask2.c
        Correct version that eliminates the race        

waitforsignal.c
        Wastefull way to wait for signals 
sigsuspend.c
        Correct way to wait for signals using sigsuspend

setjmp.c
        Uses setjmp and longjmp to return from deeply nested function

restart.c
        Restarts when the user types ctrl-c

internal.c 
external.c 
           Programs using signals to respond to internal and external
           events
