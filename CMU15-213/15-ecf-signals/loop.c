/* 
 * external.c - A program that responds to externally 
 *              generated events (ctrl-c) 
 */
#include "csapp.h"

void handler(int sig) { 
    printf("Received SIGINT\n"); 
} 
 
int main() { 
    signal(SIGINT, handler); /* installs ctl-c handler */
    while(1) { 
	continue;
    } 
} 
