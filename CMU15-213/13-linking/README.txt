
EXTENSIONS
        .s: assembly code
        .d: dissassembled object file
        .o: relocatable object file
        .a: static library
        .so: shared library

FILES
Makefile
        "make clean; make" generates all files

main.c
sum.c
        Simple example program 
prog
        Executable for simple example program

main2.c
addvec.c
multvec.c
vector.h
        Example program for linking at compile time and load time
prog2c
prog2l
        Executables for main2.c linked at compile and load time

dll.c
        Example program for dynamic linking at run time
prog2r
        Executable for dll.c linked at run time

interpose/
        Interpositioning case study
