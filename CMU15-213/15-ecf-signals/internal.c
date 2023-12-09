/*
 * internal.c - A program that responds to internally 
 *              generated events (SIGALARM signals)
 */
#include "csapp.h"

int beeps = 0; 
 
/* SIGALRM handler */
void handler(int sig) { 
    sio_puts("BEEP\n"); 

    if (++beeps < 5)   
	alarm(1); 
    else { 
	sio_puts("BOOM!\n"); 
	_exit(0); 
    } 
} 

int main() { 
    signal(SIGALRM, handler);  
    alarm(1); /* send SIGALRM in
		 1 second */
 
    while (1) { 
	continue; /* handler returns here */ 
    } 
} 
